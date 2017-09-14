#include <stdafx.h>
#include "skinnedMesh.h"

#pragma warning(disable:4996)

class BoneHierarchyLoader: public ID3DXAllocateHierarchy
{
	public:
		STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);
		STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR Name, CONST D3DXMESHDATA * pMeshData, CONST D3DXMATERIAL * pMaterials, CONST D3DXEFFECTINSTANCE * pEffectInstances, DWORD NumMaterials, CONST DWORD * pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER * ppNewMeshContainer);
		STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
		STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);
};

HRESULT BoneHierarchyLoader::CreateFrame(LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	*ppNewFrame = NULL;
	Bone *newBone = new Bone;
	
	memset(newBone, 0, sizeof(Bone));

	 D3DXMatrixIdentity(&newBone->TransformationMatrix);
    D3DXMatrixIdentity(&newBone->exCombinedTransformationMatrix);

	newBone->pMeshContainer = 0;
    newBone->pFrameSibling = 0;
    newBone->pFrameFirstChild = 0;

	*ppNewFrame = (D3DXFRAME*)newBone;
	//Copy name
	if(Name)
	{
		newBone->Name = new char[strlen(Name)+1];
		strcpy(newBone->Name, Name);
	}

	return S_OK;
}

HRESULT BoneHierarchyLoader::CreateMeshContainer(LPCSTR Name,
											CONST D3DXMESHDATA *pMeshData,
											CONST D3DXMATERIAL *pMaterials,
											CONST D3DXEFFECTINSTANCE *pEffectInstances,
											DWORD NumMaterials,
											CONST DWORD *pAdjacency,
											LPD3DXSKININFO pSkinInfo,
											LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	*ppNewMeshContainer = NULL;

	//Create new Bone Mesh
	BoneMesh *boneMesh = new BoneMesh;
	memset(boneMesh, 0, sizeof(BoneMesh));

	if(Name)
	{
		boneMesh->Name = new char[strlen(Name)+1];
		strcpy(boneMesh->Name, Name);
	}

	boneMesh->MeshData.Type = D3DXMESHTYPE_MESH;
	
	// Adjacency data - holds information about triangle adjacency, required by the ID3DMESH object
	DWORD dwFaces = pMeshData->pMesh->GetNumFaces();
	boneMesh->pAdjacency = new DWORD[dwFaces*3];
	memcpy(boneMesh->pAdjacency, pAdjacency, sizeof(DWORD) * dwFaces*3);
	
	// Get the Direct3D device, luckily this is held in the mesh itself (Note: must release it when done with it)
	LPDIRECT3DDEVICE9 pd3dDevice = 0;
	pMeshData->pMesh->GetDevice(&pd3dDevice);

	// Changed 24/09/07 - can just assign pointer and add a ref rather than need to clone
	boneMesh->MeshData.pMesh=pMeshData->pMesh;
	boneMesh->MeshData.pMesh->AddRef();

	// Create material and texture arrays. Note that I always want to have at least one
	boneMesh->NumMaterials = max(NumMaterials,1);
	boneMesh->exMaterials = new D3DMATERIAL9[boneMesh->NumMaterials];
	boneMesh->exTextures  = new LPDIRECT3DTEXTURE9[boneMesh->NumMaterials];

	ZeroMemory(boneMesh->exTextures, sizeof(LPDIRECT3DTEXTURE9) * boneMesh->NumMaterials);

	if (NumMaterials>0)
	{
		// Load all the textures and copy the materials over		
		for(DWORD i = 0; i < NumMaterials; ++i)
		{
			boneMesh->exTextures[i] = 0;	
			boneMesh->exMaterials[i]=pMaterials[i].MatD3D;

			if(pMaterials[i].pTextureFilename)
			{
				LPCWSTR ws = L"mesh/"+(*(pMaterials[i].pTextureFilename));
				//Load texture
				D3DXCreateTextureFromFile(pd3dDevice, ws, &boneMesh->exTextures[i]);
			}
		}
	}
	else    
	// make a default material in the case where the mesh did not provide one
    {
		ZeroMemory(&boneMesh->exMaterials[0], sizeof( D3DMATERIAL9 ) );
        boneMesh->exMaterials[0].Diffuse.r = 0.5f;
        boneMesh->exMaterials[0].Diffuse.g = 0.5f;
        boneMesh->exMaterials[0].Diffuse.b = 0.5f;
		boneMesh->exMaterials[0].Specular = boneMesh->exMaterials[0].Diffuse;
		boneMesh->exTextures[0]=0;
    }

	// If there is skin data associated with the mesh copy it over
	if (pSkinInfo)
	{
		// save off the SkinInfo
	    boneMesh->pSkinInfo = pSkinInfo;
	    pSkinInfo->AddRef();

	    // Need an array of offset matrices to move the vertices from the figure space to the bone's space
	    UINT numBones = pSkinInfo->GetNumBones();
	    boneMesh->exBoneOffsets = new D3DXMATRIX[numBones];

		// Create the arrays for the bones and the frame matrices
		boneMesh->exFrameCombinedMatrixPointer = new D3DXMATRIX*[numBones];

	    // get each of the bone offset matrices so that we don't need to get them later
	    for (UINT i = 0; i < numBones; i++)
	        boneMesh->exBoneOffsets[i] = *(boneMesh->pSkinInfo->GetBoneOffsetMatrix(i));

		//CUtility::DebugString("Mesh has skinning info.\n Number of bones is: "+ToString(numBones)+"\n");
        // Note: in the Microsoft samples a GenerateSkinnedMesh function is called here in order to prepare
		// the skinned mesh data for optimial hardware acceleration. As mentioned in the notes this sample
		// does not do hardware skinning but instead uses software skinning.
	}
	else	
	{
		// No skin info so 0 all the pointers
		boneMesh->pSkinInfo = 0;
		boneMesh->exBoneOffsets = 0;
		boneMesh->exSkinMesh = 0;
		boneMesh->exFrameCombinedMatrixPointer = 0;
	}

	// When we got the device we caused an internal reference count to be incremented
	// So we now need to release it
	pd3dDevice->Release();

	
	// Set the output mesh container pointer to our newly created one
	*ppNewMeshContainer = boneMesh;    

	return S_OK;
}

HRESULT BoneHierarchyLoader::DestroyFrame(LPD3DXFRAME pFrameToFree) 
{
	//Convert the frame
	Bone* pFrame = (Bone*)pFrameToFree;
	if(!pFrame) return S_OK;
	// Delete the name
	if(pFrame->Name)
		delete []  pFrame->Name;
	
	// Delete the frame
    if(pFrame)
		delete pFrame;

	// Returns an HRESULT so give it the AOk result
    return S_OK; 
}

HRESULT BoneHierarchyLoader::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	BoneMesh *boneMesh = (BoneMesh*)pMeshContainerBase;

	if(boneMesh->Name)
		delete [] boneMesh->Name;

	//Release mesh data
	if(boneMesh->MeshData.pMesh)
		boneMesh->MeshData.pMesh->Release();

	if(boneMesh->pMaterials)
		delete [] boneMesh->pMaterials;

	// material array
	if (boneMesh->exMaterials)
	{
		delete []boneMesh->exMaterials;
		boneMesh->exMaterials=0;
	}

	//Release textures
if(boneMesh->exTextures)
	{
		for(UINT i = 0; i < boneMesh->NumMaterials; ++i)
		{
			if (boneMesh->exTextures[i])
				boneMesh->exTextures[i]->Release();
		}
	}

	if (boneMesh->exTextures)
		delete []boneMesh->exTextures;

	if(boneMesh->pSkinInfo)
		boneMesh->pSkinInfo->Release();
	// adjacency data
	if (boneMesh->pAdjacency)
		delete []boneMesh->pAdjacency;
	
	// bone parts
	if (boneMesh->exBoneOffsets)
	{
		delete []boneMesh->exBoneOffsets;
		boneMesh->exBoneOffsets=0;
	}
	
	// frame matrices
	if (boneMesh->exFrameCombinedMatrixPointer)
	{
		delete []boneMesh->exFrameCombinedMatrixPointer;
		boneMesh->exFrameCombinedMatrixPointer=0;
	}
	
	// release skin mesh
	if (boneMesh->exSkinMesh)
	{
		boneMesh->exSkinMesh->Release();
		boneMesh->exSkinMesh=0;
	}
	
	delete boneMesh;
	boneMesh = 0;

    return S_OK;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
//									SKINNED MESH												//
//////////////////////////////////////////////////////////////////////////////////////////////////
SkinnedMesh::SkinnedMesh(LPDIRECT3DDEVICE9 d3dDevice): m_d3dDevice(d3dDevice),m_speedAdjust(1.0f),m_firstMesh(0),
	m_currentTrack(0),m_currentTime(0),m_numAnimationSets(0),m_currentAnimationSet(0),m_maxBones(0),m_boneMatrices(0)
{
}

SkinnedMesh::~SkinnedMesh()
{
	if (m_animController)
	{
		m_animController->Release();
		m_animController=0;
	}

	if (m_frameRoot)
	{
		// Create a mesh heirarchy class to control the removal of memory for the frame heirarchy
		BoneHierarchyLoader memoryAllocator;
		D3DXFrameDestroy(m_frameRoot, &memoryAllocator);
		m_frameRoot=0;
	}

	if (m_boneMatrices)
	{
		delete []m_boneMatrices;
		m_boneMatrices=0;
	}

	if(m_firstMesh)
	{
		//delete m_firstMesh;
		m_firstMesh = 0;
	}
}

void SkinnedMesh::DrawFrame(LPD3DXFRAME frame, D3DXMATRIX m)
{
	// Draw all mesh containers in this frame
    LPD3DXMESHCONTAINER meshContainer = frame->pMeshContainer;
    while (meshContainer)
    {
        DrawMeshContainer(meshContainer, frame, m);
        meshContainer = meshContainer->pNextMeshContainer;
    }

	// Recurse for sibblings
    if (frame->pFrameSibling != NULL)
        DrawFrame(frame->pFrameSibling, m);

    // Recurse for children
	if (frame->pFrameFirstChild != NULL)
        DrawFrame(frame->pFrameFirstChild, m );
}
void SkinnedMesh::DrawMeshContainer(LPD3DXMESHCONTAINER meshContainerBase, LPD3DXFRAME frameBase, D3DXMATRIX m) 
{
	// Cast to our extended frame type
	Bone *frame = (Bone*)frameBase;		

	// Cast to our extended mesh container
	BoneMesh *meshContainer = (BoneMesh*)meshContainerBase;
	
	// Set the world transform
	if(meshContainer->pSkinInfo == NULL)
		m_d3dDevice->SetTransform(D3DTS_WORLD, &(frame->exCombinedTransformationMatrix * m));


	// Loop through all the materials in the mesh rendering each subset
    for (unsigned int iMaterial = 0; iMaterial < meshContainer->NumMaterials; iMaterial++)
    {
		// use the material in our extended data rather than the one in meshContainer->pMaterials[iMaterial].MatD3D
		m_d3dDevice->SetMaterial( &meshContainer->exMaterials[iMaterial] );
		m_d3dDevice->SetTexture( 0, meshContainer->exTextures[iMaterial] );

		// Select the mesh to draw, if there is skin then use the skinned mesh else the normal one
		LPD3DXMESH pDrawMesh = (meshContainer->pSkinInfo) ? meshContainer->exSkinMesh: meshContainer->MeshData.pMesh;

		// Finally Call the mesh draw function
        pDrawMesh->DrawSubset(iMaterial);
    }
}

void SkinnedMesh::Load(LPCWSTR fileName)
{
	BoneHierarchyLoader boneHierarchy;

	D3DXLoadMeshHierarchyFromX(fileName, D3DXMESH_MANAGED, 
							   m_d3dDevice, &boneHierarchy,
							   NULL, &m_frameRoot, &m_animController);

	SetupBoneMatrixPointers((Bone*)m_frameRoot, NULL);

	m_boneMatrices  = new D3DXMATRIX[m_maxBones];
	ZeroMemory(m_boneMatrices, sizeof(D3DXMATRIX)*m_maxBones);

	ID3DXAnimationSet *anim = NULL;
	m_animController->GetAnimationSet(0, &anim);
	m_animController->SetTrackAnimationSet(0, anim);
	m_animController->SetTrackAnimationSet(1, anim);

	m_animController->SetTrackWeight(0, 1);
	m_animController->SetTrackWeight(1, 1);

	//Set random speed (0 - 200%)
	m_animController->SetTrackSpeed(0, 2);
	m_animController->SetTrackSpeed(1, 2);

	//Set Track priorites
	m_animController->SetTrackPriority(0, D3DXPRIORITY_HIGH);
	m_animController->SetTrackPriority(1, D3DXPRIORITY_HIGH);

	//Enable tracks
	m_animController->SetTrackEnable(0, false);
	m_animController->SetTrackEnable(1, false);
	anim->Release();
}

void SkinnedMesh::SelectAnimation(int l, int r)
{
	if(l >= 0)
		m_animController->SetTrackEnable(0, true);
	if(l == 0)
		m_animController->SetTrackPosition(0, 3.66);
	else if(l == 2)
		m_animController->SetTrackPosition(0, 3);
	else if (l == 4)
		m_animController->SetTrackPosition(0, 2.33);
	else
		m_animController->SetTrackEnable(0, false);

	if(r >= 0)
		m_animController->SetTrackEnable(1, true);
	if(r == 1)
		m_animController->SetTrackPosition(1, 1.33);
	else if(r == 3)
		m_animController->SetTrackPosition(1, .66);
	else if (r == 5)
		m_animController->SetTrackPosition(1, 0);
	else
		m_animController->SetTrackEnable(1, false);
}


void SkinnedMesh::UpdateMatrices(Bone* bone, D3DXMATRIX *parentMatrix)
{

	// If parent matrix exists multiply our frame matrix by it
    if (parentMatrix != NULL)
        D3DXMatrixMultiply(&bone->exCombinedTransformationMatrix, &bone->TransformationMatrix, parentMatrix);
    else
        bone->exCombinedTransformationMatrix = bone->TransformationMatrix;

	// If we have a sibling recurse 
    if (bone->pFrameSibling != NULL)
        UpdateMatrices((Bone*) bone->pFrameSibling, parentMatrix);

	// If we have a child recurse 
    if (bone->pFrameFirstChild != NULL)
        UpdateMatrices((Bone*)bone->pFrameFirstChild, &bone->exCombinedTransformationMatrix);
}

void SkinnedMesh::Render(D3DXMATRIX m)
{
	if (m_frameRoot)
		DrawFrame(m_frameRoot, m);
}



void SkinnedMesh::SetupBoneMatrixPointers(Bone *bone, LPD3DXMATRIX pParentMatrix)
{
	// Cast to our extended structure first
	BoneMesh* pMesh = (BoneMesh*)bone->pMeshContainer;

	// If this frame has a mesh
	if(pMesh)
	{
		// We need to remember which is the first mesh in the hierarchy for later when we 
		// update (FrameMove)
		if(!m_firstMesh)
			m_firstMesh = pMesh;
		
		// if there is skin info, then setup the bone matrices
		if(pMesh->pSkinInfo && pMesh->MeshData.pMesh)
		{
			// Create a copy of the mesh to skin into later
			D3DVERTEXELEMENT9 Declaration[MAX_FVF_DECL_SIZE];
			if (FAILED(pMesh->MeshData.pMesh->GetDeclaration(Declaration)))
				return;

			pMesh->MeshData.pMesh->CloneMesh(D3DXMESH_MANAGED, 
				Declaration, m_d3dDevice, 
				&pMesh->exSkinMesh);

			// Max bones is calculated for later use (to know how big to make the bone matrices array)
			m_maxBones=max(m_maxBones,(int)pMesh->pSkinInfo->GetNumBones());

			// For each bone work out its matrix
			for (unsigned int i = 0; i < pMesh->pSkinInfo->GetNumBones(); i++)
			{   
				// Find the frame containing the bone
				Bone* pTempFrame = (Bone*)D3DXFrameFind(m_frameRoot, 
						pMesh->pSkinInfo->GetBoneName(i));

				// set the bone part - point it at the transformation matrix
				pMesh->exFrameCombinedMatrixPointer[i] = &pTempFrame->exCombinedTransformationMatrix;
			}

		}
	}

	// Pass on to sibblings
	if(bone->pFrameSibling)
		SetupBoneMatrixPointers((Bone*)bone->pFrameSibling, pParentMatrix);

	// Pass on to children
	if(bone->pFrameFirstChild)
		SetupBoneMatrixPointers((Bone*)bone->pFrameFirstChild, &bone->exCombinedTransformationMatrix);
}

void SkinnedMesh::SetPose(D3DXMATRIX world, float time)

{
	// Adjust animation speed
	time/=m_speedAdjust;

	// Advance the time and set in the controller
    if (m_animController != NULL)
        m_animController->AdvanceTime(time, NULL);

	m_currentTime+=time;

	// Now update the model matrices in the hierarchy
    UpdateMatrices((Bone*)m_frameRoot, &world);

	// If the model contains a skinned mesh update the vertices
	BoneMesh* pMesh = m_firstMesh;
	if(pMesh && pMesh->pSkinInfo)
	{
		unsigned int Bones = pMesh->pSkinInfo->GetNumBones();

		// Create the bone matrices that transform each bone from bone space into character space
		// (via exFrameCombinedMatrixPointer) and also wraps the mesh around the bones using the bone offsets
		// in exBoneOffsetsArray
		for (unsigned int i = 0; i < Bones; ++i)
			D3DXMatrixMultiply(&m_boneMatrices[i],&pMesh->exBoneOffsets[i], pMesh->exFrameCombinedMatrixPointer[i]);

		// We need to modify the vertex positions based on the new bone matrices. This is achieved
		// by locking the vertex buffers and then calling UpdateSkinnedMesh. UpdateSkinnedMesh takes the
		// original vertex data (in pMesh->MeshData.pMesh), applies the matrices and writes the new vertices
		// out to skin mesh (pMesh->exSkinMesh). 

		// UpdateSkinnedMesh uses software skinning which is the slowest way of carrying out skinning 
		// but is easiest to describe and works on the majority of graphic devices. 
		// Other methods exist that use hardware to do this skinning - see the notes and the 
		// DirectX SDK skinned mesh sample for more details
		void *srcPtr=0;
		pMesh->MeshData.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&srcPtr);

		void *destPtr=0;
		pMesh->exSkinMesh->LockVertexBuffer(0, (void**)&destPtr);

		// Update the skinned mesh 
		pMesh->pSkinInfo->UpdateSkinnedMesh(m_boneMatrices, NULL, srcPtr, destPtr);

		// Unlock the meshes vertex buffers
		pMesh->exSkinMesh->UnlockVertexBuffer();
		pMesh->MeshData.pMesh->UnlockVertexBuffer();
	}
}
