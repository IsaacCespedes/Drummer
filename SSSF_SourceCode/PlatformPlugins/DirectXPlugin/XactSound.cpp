#include "stdafx.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\PlatformPlugins\DirectXPlugin\XactSound.h"

XactSound::XactSound()
{	
	HRESULT hr;

	if( FAILED( hr = prepareXACT() ) )
    {
        if( hr == HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND ) )
            bool noMedia = true;
        else
            bool fail = true;
        shutdown();
    }
}

XactSound::~XactSound()
{
}

HRESULT	XactSound::createBank()
{
	HRESULT hr;
	WCHAR str[MAX_PATH];
    HANDLE hFile;
	DWORD dwFileSize;
    DWORD dwBytesRead;
    HANDLE hMapFile;

	if( FAILED( hr = FindMediaFileCch( str, MAX_PATH, L"sound/Win/wavebank.xwb" ) ) )
        return hr;

	// Create an "in memory" XACT wave bank file using memory mapped file IO
    // Memory mapped files tend to be the fastest for most situations assuming you 
    // have enough virtual address space for a full map of the file
    hr = E_FAIL; // assume failure
    hFile = CreateFile( str, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
    if( hFile != INVALID_HANDLE_VALUE )
    {
        dwFileSize = GetFileSize( hFile, NULL );
        if( dwFileSize != -1 )
        {
            hMapFile = CreateFileMapping( hFile, NULL, PAGE_READONLY, 0, dwFileSize, NULL );
            if( hMapFile )
            {
                g_audioState.pbWaveBank = MapViewOfFile( hMapFile, FILE_MAP_READ, 0, 0, 0 );
                if( g_audioState.pbWaveBank )
                {
                    hr = g_audioState.pEngine->CreateInMemoryWaveBank( g_audioState.pbWaveBank, dwFileSize, 0,
                                                                       0, &g_audioState.pWaveBank );
                }
                CloseHandle( hMapFile ); // pbWaveBank maintains a handle on the file so close this unneeded handle
            }
        }
        CloseHandle( hFile ); // pbWaveBank maintains a handle on the file so close this unneeded handle
    }
    if( FAILED( hr ) )
        return E_FAIL; // CleanupXACT() will cleanup state before exiting
		

	// Read and register the sound bank file with XACT.  Do not use memory mapped file IO because the 
    // memory needs to be read/write and the working set of sound banks are small.
    if( FAILED( hr = FindMediaFileCch( str, MAX_PATH, L"sound/Win/soundbank.xsb" ) ) )
        return hr;
    hr = E_FAIL; // assume failure
    hFile = CreateFile( str, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
    if( hFile != INVALID_HANDLE_VALUE )
    {
        dwFileSize = GetFileSize( hFile, NULL );
        if( dwFileSize != -1 )
        {
            // Allocate the data here and free the data when recieving the sound bank destroyed notification
            g_audioState.pbSoundBank = new BYTE[dwFileSize];
            if( g_audioState.pbSoundBank )
            {
                if( 0 != ReadFile( hFile, g_audioState.pbSoundBank, dwFileSize, &dwBytesRead, NULL ) )
                {
                    hr = g_audioState.pEngine->CreateSoundBank( g_audioState.pbSoundBank, dwFileSize, 0,
                                                                0, &g_audioState.pSoundBank );
                }
            }
        }
        CloseHandle( hFile );
    }
    if( FAILED( hr ) )
        return E_FAIL; // CleanupXACT() will cleanup state before exiting

	  // Get indices to XACT categories 
    //g_audioState.iMusicCategory = g_audioState.pEngine->GetCategory( "Music" );
    //g_audioState.iGlobalCategory = g_audioState.pEngine->GetCategory( "Global" );

    return S_OK;

}

bool XactSound::DoesCommandLineContainAuditionSwitch()
{
    const WCHAR* strAuditioning = L"-audition"; size_t nArgLen; int nNumArgs;
    LPWSTR* pstrArgList = CommandLineToArgvW( GetCommandLine(), &nNumArgs );
    for( int iArg = 1; iArg < nNumArgs; iArg++ )
    {
        StringCchLength( pstrArgList[iArg], 256, &nArgLen );
        if( _wcsnicmp( pstrArgList[iArg], strAuditioning, nArgLen ) == 0 && nArgLen == 9 )
            return true;
    }
    LocalFree( pstrArgList );
    return false;
}

//--------------------------------------------------------------------------------------
// Helper function to try to find the location of a media file
//--------------------------------------------------------------------------------------
HRESULT XactSound::FindMediaFileCch( WCHAR* strDestPath, int cchDest, LPCWSTR strFilename )
{
    bool bFound = false;

    if( NULL == strFilename || strFilename[0] == 0 || NULL == strDestPath || cchDest < 10 )
        return E_INVALIDARG;

    // Get the exe name, and exe path
    WCHAR strExePath[MAX_PATH] = {0};
    WCHAR strExeName[MAX_PATH] = {0};
    WCHAR* strLastSlash = NULL;
    GetModuleFileName( NULL, strExePath, MAX_PATH );
    strExePath[MAX_PATH - 1] = 0;
    strLastSlash = wcsrchr( strExePath, TEXT( '\\' ) );
    if( strLastSlash )
    {
        StringCchCopy( strExeName, MAX_PATH, &strLastSlash[1] );

        // Chop the exe name from the exe path
        *strLastSlash = 0;

        // Chop the .exe from the exe name
        strLastSlash = wcsrchr( strExeName, TEXT( '.' ) );
        if( strLastSlash )
            *strLastSlash = 0;
    }

    StringCchCopy( strDestPath, cchDest, strFilename );
    if( GetFileAttributes( strDestPath ) != 0xFFFFFFFF )
        return S_OK;

    // Search all parent directories starting at .\ and using strFilename as the leaf name
    WCHAR strLeafName[MAX_PATH] = {0};
    StringCchCopy( strLeafName, MAX_PATH, strFilename );

    WCHAR strFullPath[MAX_PATH] = {0};
    WCHAR strFullFileName[MAX_PATH] = {0};
    WCHAR strSearch[MAX_PATH] = {0};
    WCHAR* strFilePart = NULL;

    GetFullPathName( L".", MAX_PATH, strFullPath, &strFilePart );
    if( strFilePart == NULL )
        return E_FAIL;

    while( strFilePart != NULL && *strFilePart != '\0' )
    {
        StringCchPrintf( strFullFileName, MAX_PATH, L"%s\\%s", strFullPath, strLeafName );
        if( GetFileAttributes( strFullFileName ) != 0xFFFFFFFF )
        {
            StringCchCopy( strDestPath, cchDest, strFullFileName );
            bFound = true;
            break;
        }

        StringCchPrintf( strFullFileName, MAX_PATH, L"%s\\Tutorials\\%s\\%s", strFullPath, strExeName, strLeafName );
        if( GetFileAttributes( strFullFileName ) != 0xFFFFFFFF )
        {
            StringCchCopy( strDestPath, cchDest, strFullFileName );
            bFound = true;
            break;
        }

        StringCchPrintf( strSearch, MAX_PATH, L"%s\\..", strFullPath );
        GetFullPathName( strSearch, MAX_PATH, strFullPath, &strFilePart );
    }
    if( bFound )
        return S_OK;

    // On failure, return the file as the path but also return an error code
    StringCchCopy( strDestPath, cchDest, strFilename );

    return HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND );
}

bool XactSound::isThereEngine()
{
	return g_audioState.pEngine;
}


HRESULT XactSound::prepareXACT()
{
	HRESULT hr;
	WCHAR str[MAX_PATH];
	HANDLE hFile;
   

	// Clear struct
    ZeroMemory( &g_audioState, sizeof( AUDIO_STATE ) );
	//g_audioState.fGlobalVolume = 1.0f;
    //g_audioState.fMusicVolume = 0.3f;

	//INITIALIZE XACT

	// COINIT_APARTMENTTHREADED will work too
    hr = CoInitializeEx( NULL, COINIT_MULTITHREADED ); 


	//1. create xact engine instance
	if( SUCCEEDED( hr ) )
    {
        // Switch to auditioning mode based on command line.  Change if desired
        bool bAuditionMode = DoesCommandLineContainAuditionSwitch();
        bool bDebugMode = false;

        DWORD dwCreationFlags = 0;
        if( bAuditionMode ) dwCreationFlags |= XACT_FLAG_API_AUDITION_MODE;
        if( bDebugMode ) dwCreationFlags |= XACT_FLAG_API_DEBUG_MODE;

        hr = XACT3CreateEngine( dwCreationFlags, &g_audioState.pEngine );
    }
    if( FAILED( hr ) || g_audioState.pEngine == NULL )
        return E_FAIL;

	// Load the global settings file and pass it into XACTInitialize
    VOID* pGlobalSettingsData = NULL;
    DWORD dwGlobalSettingsFileSize = 0;
    bool bSuccess = false;
    DWORD dwBytesRead;
    if( SUCCEEDED(FindMediaFileCch( str, MAX_PATH, L"sound/Win/drumsounds.xgs" ) ) )
    {
        hFile = CreateFile( str, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
        if( hFile )
        {
            dwGlobalSettingsFileSize = GetFileSize( hFile, NULL );
            if( dwGlobalSettingsFileSize != INVALID_FILE_SIZE )
            {
                // Using CoTaskMemAlloc so that XACT can clean up this data when its done
                pGlobalSettingsData = CoTaskMemAlloc( dwGlobalSettingsFileSize );
                if( pGlobalSettingsData )
                {
                    if( 0 != ReadFile( hFile, pGlobalSettingsData, dwGlobalSettingsFileSize, &dwBytesRead, NULL ) )
                    {
                        bSuccess = true;
                    }
                }
            }
            CloseHandle( hFile );
        }
    }
    if( !bSuccess )
    {
        if( pGlobalSettingsData )
            CoTaskMemFree( pGlobalSettingsData );
        pGlobalSettingsData = NULL;
        dwGlobalSettingsFileSize = 0;
    }

	//2. populate xact runtime params
    XACT_RUNTIME_PARAMETERS xrParams = {0};
	xrParams.pGlobalSettingsBuffer = pGlobalSettingsData;
    xrParams.globalSettingsBufferSize = dwGlobalSettingsFileSize;
    xrParams.globalSettingsFlags = XACT_FLAG_GLOBAL_SETTINGS_MANAGEDATA;
	xrParams.lookAheadTime = XACT_ENGINE_LOOKAHEAD_DEFAULT;

	//3. call initialize
    hr = g_audioState.pEngine->Initialize( &xrParams );
    if( FAILED( hr ) )
        return hr;


	createBank();
}

void XactSound::playSound(PCSTR soundName)
{
	g_audioState.pSoundBank->Play( g_audioState.pSoundBank->GetCueIndex(soundName), 0, 0, NULL );
}

void XactSound::pause()
{
	g_audioState.pEngine->Pause(g_audioState.pEngine->GetCategory("Default"), true);
}

void XactSound::resume()
{
		g_audioState.pEngine->Pause(g_audioState.pEngine->GetCategory("Default"), false);
}

void XactSound::shutdown()
{
	// Shutdown XACT
    //
    // Note that pEngine->ShutDown is synchronous and will take some time to complete 
    // if there are still playing cues.  Also pEngine->ShutDown() is generally only 
    // called when a game exits and is not the preferred method of changing audio 
    // resources. To know when it is safe to free wave/sound bank data without 
    // shutting down the XACT engine, use the XACTNOTIFICATIONTYPE_SOUNDBANKDESTROYED 
    // or XACTNOTIFICATIONTYPE_WAVEBANKDESTROYED notifications 
   
	if(g_audioState.pSoundBank)
		g_audioState.pSoundBank->Destroy();

	if(g_audioState.pWaveBank)
		g_audioState.pWaveBank->Destroy();

	

	if( g_audioState.pEngine )
    {
		g_audioState.pEngine->Stop(g_audioState.pEngine->GetCategory("Default"), 0);
        g_audioState.pEngine->ShutDown();
        g_audioState.pEngine->Release();
    }

    if( g_audioState.pbSoundBank )
		UnmapViewOfFile(g_audioState.pbSoundBank);
        //delete[] g_audioState.pbSoundBank;
    g_audioState.pbSoundBank = NULL;

    // After pEngine->ShutDown() returns it is safe to release memory mapped files
    if( g_audioState.pbWaveBank )
        UnmapViewOfFile( g_audioState.pbWaveBank );
    g_audioState.pbWaveBank = NULL;

    CoUninitialize();
}

void XactSound::UpdateAudio()
{
    // It is important to allow XACT to do periodic work by calling pEngine->DoWork().  
    // However this must function be call often enough.  If you call it too infrequently, 
    // streaming will suffer and resources will not be managed promptly.  On the other hand 
    // if you call it too frequently, it will negatively affect performance. Calling it once 
    // per frame is usually a good balance.
    if( g_audioState.pEngine )
        g_audioState.pEngine->DoWork();
}

void XactSound::work()
{
	UpdateAudio();
}


