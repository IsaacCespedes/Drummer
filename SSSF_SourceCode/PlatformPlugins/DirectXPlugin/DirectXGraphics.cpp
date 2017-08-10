/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	DirectXGraphics.cpp

	See DirectXGraphics.h for a class description.
*/

#include "stdafx.h"
#include "SSSF_SourceCode\game\Game.h"
#include "SSSF_SourceCode\graphics\GameGraphics.h"
#include "SSSF_SourceCode\graphics\TextureManager.h"
#include "SSSF_SourceCode\gui\GameGUI.h"
#include "SSSF_SourceCode\gui\Viewport.h"
#include "SSSF_SourceCode\PlatformPlugins\DirectXPlugin\DirectXGraphics.h"
#include "SSSF_SourceCode\PlatformPlugins\DirectXPlugin\DirectXTextureManager.h"
#include "SSSF_SourceCode\text\GameText.h"

/*
	DirectXGraphics - Default constructor, it doesn't initialize anything.
	To setup all the DirectX objects call initGraphics after construction.
*/
DirectXGraphics::DirectXGraphics(Game *initGame)
{
	game = initGame;
}

/*
	~DirectXGraphics - Destructor, it destroys all of the DirectX pointers.
	This would be called only when the application closes, unless someone 
	decides to use different rendering technologies during the game.
*/
DirectXGraphics::~DirectXGraphics()
{
	displayOptions->clear();
	delete displayOptions;
	delete indexBuffer;
	delete modelVertexBuffer;
	delete (DirectXTextureManager*)worldTextureManager;
	delete (DirectXTextureManager*)guiTextureManager;
	delete textFont;
	delete spriteHandler;
	delete graphicsDevice;
	delete d3d;
	//delete worldVertexBuffer;
	//delete guiVertexBuffer;
	//delete (DirectXTextureManager*) guiTextureManager;
	//delete (DirectXTextureManager*) worldTextureManager;
}


/*
	containsDisplayMode - This method looks at the vector of display modes that
	were presumably retrieved from the GPU, and tests to see if our desired
	color format and screen resolution are inside. If found, true is returned,
	otherwise false.
*/
bool DirectXGraphics::containsDisplayMode(vector<D3DDISPLAYMODE> *displayModes,
										  D3DFORMAT testColorFormat,
										  int testScreenWidth,
										  int testScreenHeight)
{
	vector<D3DDISPLAYMODE>::iterator iterator;

	// START WITH THE FIRST ELEMENT
	iterator = displayModes->begin();

	// LOOK THROUGH THE WHOLE VECTOR
	while (iterator != displayModes->end())
	{
		// GET THE CURRENT MODE
		D3DDISPLAYMODE testMode = (*iterator);

		// IS IT THE ONE WE'RE LOOKING FOR?
		if ((testMode.Format == testColorFormat)
			&& (testMode.Width == testScreenWidth)
			&& (testMode.Height == testScreenHeight))
			return true;

		// GO ONTO THE NEXT ONE
		iterator++;
	}
	return false;
}

/*
	createDirectXDeviceAndSpriteHandler - THIS METHOD CREATES OUR GPU AND
	SPRITE HANDLER (used for batch rendering textures) USING THE COLOR
	FORMAT AND SCREEN RESOLUTION OF OUR CHOICE.
*/
HRESULT DirectXGraphics::createDirectXDeviceAndSpriteHandler()
{
	HRESULT result;
	GameText *text = game->getText();

	// CREATE OUR GPU
    result = d3d->CreateDevice(D3DADAPTER_DEFAULT,
						D3DDEVTYPE_HAL,
						presentParameters.hDeviceWindow,
						D3DCREATE_HARDWARE_VERTEXPROCESSING,
						&presentParameters,
						&graphicsDevice);
	
	// IF GPU CREATION WAS SUCCESSFUL
	if (SUCCEEDED(result))
	{
		text->writeDebugOutput("SUCCEEDED");

		text->writeDebugOutput("\nD3DXCreateSprite(): ");

		// CREATE OUR SPRITE HANDLER
	    result = D3DXCreateSprite(graphicsDevice, &spriteHandler);
		if (SUCCEEDED(result))
		{
			text->writeDebugOutput("SUCCEEDED");
		}
		else
			text->writeDebugOutput("FAILED");
	}
	else
		text->writeDebugOutput("FAILED");

    graphicsDevice->SetRenderState(D3DRS_LIGHTING, TRUE);    // turn off the 3D lighting
	graphicsDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(50, 50, 50)); // ambient light
	graphicsDevice->SetRenderState(D3DRS_ZENABLE, TRUE); 

	/*graphicsDevice->SetVertexShader(NULL);
	graphicsDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//graphicsDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); // both sides of the triangles
	graphicsDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	graphicsDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	graphicsDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);*/

	return result;
}

HRESULT DirectXGraphics::createVertexBuffers()
{
	 // Create the vertex buffer. Here we are allocating enough memory
    // (from the default pool) to hold all our 3 custom vertices. We also
    // specify the FVF, so the vertex buffer knows what data it contains.
    if( FAILED( graphicsDevice->CreateVertexBuffer(24*sizeof(VertexItem),
                                                  D3DUSAGE_WRITEONLY, D3DFVF_XYZ | D3DFVF_NORMAL,
												  D3DPOOL_DEFAULT, &modelVertexBuffer, NULL ) ) )
    {
        return E_FAIL;
    }

	graphicsDevice->CreateIndexBuffer(36*sizeof(short),D3DUSAGE_WRITEONLY,D3DFMT_INDEX16,D3DPOOL_DEFAULT,&indexBuffer,NULL);
}

/*
	createTextureManager - This method constructs a technology-specific
	TextureManager. Since this DirectXGraphics class uses the DirectX
	library, this method creates a DirectXTextureManager.
*/
TextureManager* DirectXGraphics::createTextureManager()
{
	TextureManager *textureManager = (TextureManager*)(new DirectXTextureManager());
	textureManager->setGraphics(this);
	return textureManager;
}

/*
	endDirectXFrameRendering - This method should be called after rendering a frame
	so that we can display what we've drawn on the GPU onto the monitor. It also
	releases a lock on the GPU so other threads may use it.
*/
void DirectXGraphics::endDirectXFrameRendering()
{
	// ALL DONE DRAWING ONTO THE GPU FOR THIS FRAME
    if (FAILED(graphicsDevice->EndScene()))
		game->getText()->writeDebugOutput("\ngraphicsDevice->EndScene(): FAILED");

	// PUT WHAT WE JUST RENDERED ONTO THE SCREEN
	if (FAILED(graphicsDevice->Present(NULL, NULL, NULL, NULL)))
		game->getText()->writeDebugOutput("\ngraphicsDevice->Present(): FAILED");
}

HRESULT DirectXGraphics::fillVertexBuffer()
{

	VertexItem cubevertices[] =
    {
        { -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, },    // side 1
        { 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, },
        { -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, },
        { 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, },

        { -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, },    // side 2
        { -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, },
        { 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, },
        { 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, },

        { -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, },    // side 3
        { -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, },
        { 1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, },
        { 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, },

        { -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, },    // side 4
        { 1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, },
        { -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, },
        { 1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, },

        { 1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, },    // side 5
        { 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, },
        { 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, },
        { 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, },

        { -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, },    // side 6
        { -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, },
        { -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, },
        { -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, },
    };

	// Now we fill the vertex buffer. To do this, we need to Lock() the VB to
    // gain access to the vertices. This mechanism is required becuase vertex
    // buffers may be in device memory.
    VOID* pVertices;
	VOID* pIndices;
    if( FAILED( modelVertexBuffer->Lock( 0, 0, ( void** )&pVertices, 0 ) ) )
	{
        return E_FAIL;
	}
	memcpy( pVertices, cubevertices, sizeof(cubevertices));
    modelVertexBuffer->Unlock();

	short indices[] =
    {
        0, 1, 2,    // side 1
        2, 1, 3,
        4, 5, 6,    // side 2
        6, 5, 7,
        8, 9, 10,    // side 3
        10, 9, 11,
        12, 13, 14,    // side 4
        14, 13, 15,
        16, 17, 18,    // side 5
        18, 17, 19,
        20, 21, 22,    // side 6
        22, 21, 23,
    };

	// lock i_buffer and load the indices into it 
	indexBuffer->Lock(0, 0, (void**)&pIndices, 0); 
	memcpy(pIndices, indices, sizeof(indices)); 
	indexBuffer->Unlock();

	 D3DLIGHT9 light; // create the light struct 
	 D3DMATERIAL9 material; // create the material struct
	 ZeroMemory(&light, sizeof(light)); // clear out the light struct for use 
	 light.Type = D3DLIGHT_DIRECTIONAL; // make the light type 'directional light' 
	 light.Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f); // set the light's color 
	 light.Direction = D3DXVECTOR3(-1.0f, -0.3f, -1.0f); 
	 graphicsDevice->SetLight(0, &light); // send the light struct properties to light #0 
	 graphicsDevice->LightEnable(0, TRUE); // turn on light #0 
	 ZeroMemory(&material, sizeof(D3DMATERIAL9)); // clear out the struct for use 
	 material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); // set diffuse color to white 
	 material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); // set ambient color to white 
	 graphicsDevice->SetMaterial(&material); // set the globably-used material to &material

	return S_OK;
}

/*
	findAlternativeDisplayMode - If the player's GPU doesn't have the
	display mode we want this method can pick a new one. It does so
	by first seeing if there is another display mode with the resolution
	we want but a different color model. If found, we'll use it. If not it
	simply picks the largest one it can find. This method uses call-by-reference
	to set the formatToSet, screenWidthToSet, & screenHeightToSet parameters
	using the chosen display mode parameters.
*/
void DirectXGraphics::findAlternativeDisplayMode(	vector<D3DDISPLAYMODE> *displayModes,
													D3DFORMAT &formatToSet)
{
	// FIRST FIND ONE WITH THE PREFERRED SCREEN
	// DIMENSIONS, DEFAULT_SCREEN_HEIGHT &
	// DEFAULT_SCREEN_WIDTH, SINCE CHANGING A GUI TO
	// FIT DIFFERENT RESOLUTIONS IS PAINFUL
	bool defaultScreenSizeFound = false;
	vector<D3DDISPLAYMODE>::iterator iterator;
	iterator = displayModes->begin();
	while (iterator != displayModes->end())
	{
		D3DDISPLAYMODE testMode = (*iterator);
		if ((testMode.Width == screenWidth)
			&& (testMode.Height == screenHeight))
		{
			if (testMode.Format == DEFAULT_COLOR_FORMAT)
			{
				formatToSet = DEFAULT_COLOR_FORMAT;
				return;
			}
			else
			{
				defaultScreenSizeFound = true;
				formatToSet = testMode.Format;
			}
		}
		iterator++;
	}

	if (defaultScreenSizeFound)
		return;

	// NONE WERE FOUND IN THE SCREEN SIZE WE WANT, SO
	// NOW JUST FIND THE LARGEST RESOLUTION AVAILABLE
	int totalPixels = 0;
	int maxPixels = 0;
	iterator = displayModes->begin();
	while (iterator != displayModes->end())
	{
		D3DDISPLAYMODE testMode = (*iterator);
		totalPixels = testMode.Width * testMode.Height;
		if (totalPixels > maxPixels)
		{
			formatToSet = testMode.Format;
			screenWidth = testMode.Width;
			screenHeight = testMode.Height;
			maxPixels = totalPixels;
		}
		iterator++;
	}
}

/*
	getDirectXDisplayModes - This method queries the GPU and gets a vector
	of all the display modes available, returning this data structure.
*/
void DirectXGraphics::getDirectXDisplayModes()
{

	// WE'LL FILL THIS WITH THE DISPLAY MODES WE FIND
	displayOptions = new vector<D3DDISPLAYMODE>();

	// WE'LL LOOK AT EACH COLOR MODEL, STARTING WITH ONE AT INDEX 1, IF THERE IS ONE
	int adapterCounter = 1;
	D3DFORMAT format;
	while (adapterCounter < 1000)
	{
		format = D3DFORMAT(adapterCounter);

		// HOW MANY MODES HAVE THIS COLOR MODEL?
		int numAdapters = d3d->GetAdapterModeCount(
										D3DADAPTER_DEFAULT,
										format);
	
		// GET ALL FOR THIS COLOR MODEL

		//FIGURE OUT HOW TO CLEAR THIS
		displayModes = new D3DDISPLAYMODE[numAdapters];
		for (int i = 0; i < numAdapters; i++)
		{
			d3d->EnumAdapterModes(	D3DADAPTER_DEFAULT,
									format,
									i,
									&displayModes[i]);

			// PUT THEM INTO OUR VECTOR
			displayOptions->push_back(displayModes[i]);
		}
		
		delete [] displayModes;
		adapterCounter++;
	}
}

/*
	getScreenHeight - This method gets the screen height being used for rendering.
*/
int DirectXGraphics::getScreenHeight()
{
	// ASK THE GRAPHICS CARD
/*	LPDIRECT3DSURFACE9 backbuffer;
	graphicsDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	D3DSURFACE_DESC surfaceDescription;
	backbuffer->GetDesc(&surfaceDescription);
	return surfaceDescription.Height;
	*/
	return screenHeight;
}

/*
	getScreenWidth - This method gets the screen width being used for rendering.
*/
int DirectXGraphics::getScreenWidth()
{
	// ASK THE GRAPHICS CARD
/*	LPDIRECT3DSURFACE9 backbuffer;
	graphicsDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	D3DSURFACE_DESC surfaceDescription;
	backbuffer->GetDesc(&surfaceDescription);
	return surfaceDescription.Width;
	*/
	return screenWidth;
}

/*
	init - This is the entry point for the application setting up the
	DirectX objects. It will get all available display modes and pick one,
	then use it to make a GPU device. Once this method is called, rendering
	can begin. It only needs to be called once at the start of the application.
	Even if we lose the graphics card (ALT-TAB), we don't have to re-init. We
	would have to re-init if we wished to change from fullscreen mode to 
	windowed mode, or if we want to change the screen resolution or color model.
*/
void DirectXGraphics::initGraphics(	HWND hWnd, bool isFullscreen)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	// WHAT ARE THE CAPABILITIES OF THE PLAYER'S GPU?
	getDirectXDisplayModes();

	// THESE WILL MAKE UP OUR DESIRED DISPLAY MODE
	D3DFORMAT formatToUse;

	// DOES THE PLAYER HAVE OUR DESIRED FORMAT?
	if (containsDisplayMode(displayOptions,
							DEFAULT_COLOR_FORMAT,
							screenWidth,
							screenHeight))
	{
		// THE GPU HAS OUR DESIRED FORMAT
		formatToUse = DEFAULT_COLOR_FORMAT;
	}
	else
	{
		// THE GPU DOESN'T HAVE OUR DESIRED FORMAT, WE NEED TO PICK ANOTHER ONE
		findAlternativeDisplayMode(	displayOptions,
									formatToUse);
	}

	// WE NEED TO FILL OUT A D3DPRESENT_PARAMETERS STRUCTURE WITH OUR
	// PREFERENCES FOR CREATING OUR GPU DEVICE

	// FIRST MAKE SURE OUR STRUCTURE IS EMPTY
    ZeroMemory(&presentParameters, sizeof(presentParameters));

	// WINDOWED MODE OR FULLSCREEN?
    presentParameters.Windowed = !isFullscreen;

	// DISCARD OLD FRAMES
	presentParameters.SwapEffect = D3DSWAPEFFECT_FLIP;

	// THE WINDOW HANDLE
    presentParameters.hDeviceWindow = hWnd;

	// THE DISPLAY MODE WE WILL BE USING
    presentParameters.BackBufferFormat	= formatToUse;
    presentParameters.BackBufferWidth	= screenWidth;
    presentParameters.BackBufferHeight	= screenHeight;

	presentParameters.EnableAutoDepthStencil = TRUE; 
	presentParameters.AutoDepthStencilFormat = D3DFMT_D16;

	// OK, NOW WE CAN MAKE OUR GPU & SPRITE HANDLER.
	createDirectXDeviceAndSpriteHandler();
}

/*
	initTextFont - This method will initialize our font object, which
	we need to do all text rendering. It only needs to be done at the
	start of the application unless we want to change the font we
	are using.
*/
void DirectXGraphics::initTextFont(int fontSize)
{
	HRESULT result = D3DXCreateFont(
		graphicsDevice,				// OUR GPU
		fontSize,					// EG FONT SIZE FOR HEIGHT
		0,							// 0 FOR FONT WIDTH, USE DEFAULT ADJUST BASED ON HEIGHT
		FW_BOLD,					// FONT WEIGHT
		0,							// MIP LEVELS
		FALSE,						// ITALICIZED?
		DEFAULT_CHARSET,			// CHARACTER SET
		OUT_DEFAULT_PRECIS,			// RENDERING PRECISION
		DEFAULT_QUALITY,			// RENDERING QUALITY 
		DEFAULT_PITCH | FF_MODERN,	// FONT FAMILY NAME
		TEXT(""),					// FONT FACE NAME
		&textFont );				// THE FONT WE ARE CREATING
}

void DirectXGraphics::initVertices()
{
	createVertexBuffers();
	fillVertexBuffer();
}

void DirectXGraphics::pipeline(Game * game)
{
	Model* model = game->getGSM()->getModel();
	Vertex3D camera = model->getCamera();
	Vertex3D focus = model->getFocus();
	Vertex3D up = model->getUp();

	D3DXMATRIX matView;    // the view transform matrix

    D3DXMatrixLookAtLH(&matView,
	&D3DXVECTOR3 (camera.x, camera.y, camera.z),    // the camera position
                       &D3DXVECTOR3 (focus.x, focus.y, focus.z),    // the look-at position
                       &D3DXVECTOR3 (up.x, up.y, up.z));    // the up direction

	graphicsDevice->SetTransform(D3DTS_VIEW, &matView);    // set the view transform to matView

	D3DXMATRIX matProjection;     // the projection transform matrix

    D3DXMatrixPerspectiveFovLH(&matProjection,
                               D3DXToRadian(45),    // the horizontal field of view
                               (FLOAT)1024/ (FLOAT)768, // aspect ratio
                               0.1f,    // the near view-plane
                               100.0f);    // the far view-plane

    graphicsDevice->SetTransform(D3DTS_PROJECTION, &matProjection);    // set the projection
	
	
	D3DXMATRIX matRotateY; // a matrix to store the rotation for each triangle 
	static float index = 0.0f; index+=0.05f; // an ever-increasing float value // build MULTIPLE matrices to translate the model and one to rotate 
	D3DXMatrixRotationY(&matRotateY, index); // the front side // tell Direct3D about each world transform, and then draw another triangle 
	graphicsDevice->SetTransform(D3DTS_WORLD, &(matRotateY) ); 

	// select the vertex buffer to display
    graphicsDevice->SetStreamSource(0, modelVertexBuffer, 0, sizeof(VertexItem));
	graphicsDevice->SetIndices(indexBuffer);

	graphicsDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 24, 0, 12);
}

/*
	reloadGraphics - This method recreates the GPU and sprite handler and
	then reloads all the textures in the current texture managers. This would
	be called after regaining the GPU.
*/
void DirectXGraphics::reloadGraphics()
{
	createDirectXDeviceAndSpriteHandler();
	initTextFont(20);
	guiTextureManager->reloadAllTextures();
	worldTextureManager->reloadAllTextures();
}

/*
	renderDirectXRenderList - This method renders a render list of game
	elements to the screen. It can process render lists for the game
	world or the gui. Note that GUI render lists use screen coordinates
	and so don't have to be offset, but game world lists use world
	coordinates, and so they will need to be offset.
*/
void DirectXGraphics::renderGUIRenderList()
{
	guiRenderList->resetIterator();
	RenderItem itemToRender;
	LPDIRECT3DTEXTURE9 texture;
	RECT *rect = NULL;

	// GO THROUGH EACH ITEM IN THE LIST
	while (guiRenderList->hasNext())
	{
		if (rect != NULL)
			delete rect;
		rect = NULL;
		itemToRender = guiRenderList->next();


		// LET'S GET THE TEXTURE WE WANT TO RENDER
		int id = itemToRender.id;
		texture = ((DirectXTextureManager*)guiTextureManager)->getTexture(id);
		D3DXVECTOR3 position = D3DXVECTOR3(	(FLOAT)(itemToRender.x),
											(FLOAT)(itemToRender.y),
											0);



		// RENDER THE OPAQUE ITEMS
		if (itemToRender.a == 255)
		{
			if (FAILED(spriteHandler->Draw(
					texture, 
					rect,
			        NULL,
					&position,
					DEFAULT_ALPHA_COLOR)))
			{
				game->getText()->writeDebugOutput("\nspriteHandler->Draw: FAILED");
			}
		}
			
	}

	// NOW EMPTY THE LIST, WE'RE ALL DONE WITH IT
	guiRenderList->clear();
	if (rect != NULL)
		delete rect;
}

void DirectXGraphics::renderWorldRenderList()
{
	worldRenderList->resetIterator();
	RenderItem itemToRender;
	LPDIRECT3DTEXTURE9 texture;
	RECT *rect = NULL;
	GameGUI *gui = game->getGUI();
	Viewport *viewport = gui->getViewport();
	int counter = 0;
	// GO THROUGH EACH ITEM IN THE LIST
	while (worldRenderList->hasNext())
	{
		if (counter != 204)
			counter++;
		if (rect != NULL)
			delete rect;
		rect = NULL;
		itemToRender = worldRenderList->next();

//		if (itemToRender != NULL)
		{
			// LET'S GET THE TEXTURE WE WANT TO RENDER
			int id = itemToRender.id;
			texture = ((DirectXTextureManager*)worldTextureManager)->getTexture(id);
			D3DXVECTOR3 position = D3DXVECTOR3(	(FLOAT)(itemToRender.x),
												(FLOAT)(itemToRender.y),
												0);
		

			position.x += viewport->getViewportOffsetX();
			position.y += viewport->getViewportOffsetY();
			
			// ADJUST FOR THE GUI OFFSET
			if ((position.x < viewport->getViewportOffsetX())
			||  (position.y < viewport->getViewportOffsetY()))
			{
				IDirect3DSurface9 *surface;
					UINT level = 0;
					HRESULT result = texture->GetSurfaceLevel(level, &surface);
					D3DSURFACE_DESC surfaceDescription;
					surface->GetDesc(&surfaceDescription);
					rect = new RECT();
					rect->left = 0;
					rect->top = 0;
					rect->right = surfaceDescription.Width;
					rect->bottom = surfaceDescription.Height;
					if (position.x < viewport->getViewportOffsetX())
					{
						int xDiff = viewport->getViewportOffsetX() - (int)position.x;
						rect->left = xDiff;
						position.x += xDiff;
					}
					if (position.y < viewport->getViewportOffsetY())
					{
						int yDiff = viewport->getViewportOffsetY() - (int)position.y;
						rect->top = yDiff;
						position.y += yDiff;
					}	

					if(surface)
					{
						int refs = surface->Release();
						surface = NULL;
					}
			}

			// RENDER THE OPAQUE ITEMS
			if (itemToRender.a == 255)
			{
				if (FAILED(spriteHandler->Draw(
						texture, 
						rect,
				        NULL,
						&position,
						DEFAULT_ALPHA_COLOR)))
				{
					game->getText()->writeDebugOutput("\nspriteHandler->Draw: FAILED");
				}
			}
			

		}
	
	}

	// NOW EMPTY THE LIST, WE'RE ALL DONE WITH IT
	worldRenderList->clear();
	if (rect != NULL)
		delete rect;
}

void DirectXGraphics::renderVertexList(Game * game)
{
	// select which vertex format we are using
    graphicsDevice->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL);

	pipeline(game);

    // copy the vertex buffer to the back buffer
   // graphicsDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

	// NOW EMPTY THE LIST, WE'RE ALL DONE WITH IT
	//modelVertexList->clear();

}


void DirectXGraphics::renderGame(Game *game)
{
	GameStateManager *gsm = game->getGSM();
	World *world = gsm->getWorld();
	GameText *text = game->getText();

	// CHECK TO SEE IF WE STILL HAVE THE GPU
	HRESULT result = graphicsDevice->TestCooperativeLevel();

	// IF WE HAVE THE GPU, RENDER THE GAME
	if (SUCCEEDED(result)) 
	{
		// NOW PREPARE TO RENDER THE LISTS USING
		// BATCH TEXTURE RENDERING
		startDirectXFrameRendering();	

		renderVertexList(game);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		// RENDER THE WORLD RENDER LIST
		renderWorldRenderList();

		// RENDER THE GUI RENDER LIST
		renderGUIRenderList();

		// RENDER THE TEXT
		renderText(text);

		// WRAP UP RENDERING RESOURCES
		if (FAILED(spriteHandler->End()))
		{
			text->writeDebugOutput("\nspriteHandler->End(): FAILED");
		}

		endDirectXFrameRendering();

	}

	// WE'VE LOST THE GPU, SLEEP UNTIL WE GET IT BACK
	else if (result == D3DERR_DEVICELOST) 
	{ 
		spriteHandler->OnLostDevice();
		textFont->OnLostDevice();
		Sleep(100); 
	}

	// WE'VE GOT IT BACK, RELOAD EVERYTHING. NOTE THAT
	// WE'LL ONLY GET THIS MESSAGE ONCE.
	else if (result == D3DERR_DEVICENOTRESET)
	{ 
		if (FAILED(graphicsDevice->Reset(&presentParameters)))
		{
			game->getText()->writeDebugOutput("\ngraphicsDevice->Reset: FAILED - Reloading GPU images");
			game->reloadAllDevices();
		}
		else
		{
			spriteHandler->OnResetDevice();
			textFont->OnResetDevice();
		}
	}
}


/*
	renderTextToDraw - This method renders a single piece of
	text to the screen using our EG font.
*/
void DirectXGraphics::renderTextToDraw(TextToDraw *textToDraw)
{
	textRect.left = textToDraw->x;
	textRect.right = textToDraw->x + textToDraw->width;
	textRect.top = textToDraw->y;
	textRect.bottom = textToDraw->y + textToDraw->height;
 
	wstring txt = textToDraw->getText();
	std::string temp(txt.length(), ' ');  
    std::copy(txt.begin(), txt.end(), temp.begin()); 

	if (FAILED(textFont->DrawTextA(
				spriteHandler, 
				temp.c_str(), 
				-1, 
				&textRect, 
				DT_LEFT, 
				fontColor )))
		game->getText()->writeDebugOutput("\ntextFont->DrawText: FAILED");
}

/*
	setColorKey - This sets the color key to be used for loading images.
*/
void DirectXGraphics::setColorKey(int r, int g, int b)
{
	colorKey = D3DCOLOR_XRGB(r, g, b);
}

/*
	setFontColor - This sets the color to be used for rendering text.
*/
void DirectXGraphics::setFontColor(int r, int g, int b)
{
	fontColor = D3DCOLOR_XRGB(r, g, b);
}

/*
	shutdownGraphics - This method releases the DirectX objects we've created
	so that other applications can use the GPU. This should only be called
	when the application is closing.
*/
void DirectXGraphics::shutdown()
{
	/*if(worldVertexBuffer)
	{
		worldVertexBuffer->Release();
		worldVertexBuffer =  NULL;
	}

	if(guiVertexBuffer)
	{
		guiVertexBuffer->Release();
		guiVertexBuffer =  NULL;
	}*/
	
	if(indexBuffer)
	{
		int d = indexBuffer->Release();
		indexBuffer =  NULL;
	}

	if(modelVertexBuffer)
	{
		int d = modelVertexBuffer->Release();
		modelVertexBuffer =  NULL;
	}

	graphicsDevice->SetTexture(0, NULL);
	worldTextureManager->clear();
	guiTextureManager->clear();

	if (textFont)
	{
		int d = textFont->Release();
		textFont = NULL;
	}

	if (spriteHandler)
	{
		int d = spriteHandler->Release();
		spriteHandler = NULL;
	}
 
	if (graphicsDevice)
	{
		int d = graphicsDevice->Release();
		//if (graphicsDevice)
	
		//graphicsDevice->Release();
		graphicsDevice = NULL;
	}

	if (d3d)
	{
		int d = d3d->Release();
		d3d = NULL;
	}
}

/*
	startDirectXFrameRendering - This does some setup for rendering, like locking
	the GPU. Only one thread at a time can have a lock on the GPU.
*/
void DirectXGraphics::startDirectXFrameRendering()
{
	 
	// CLEAR OUT ALL THE OLD RENDERING
    if (FAILED(graphicsDevice->Clear(0, NULL, D3DCLEAR_TARGET, BACKGROUND_COLOR, 1.0f, 0)))
		game->getText()->writeDebugOutput("\ngraphicsDevice->Clear(): FAILED");

	if(FAILED(graphicsDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(100, 200, 100), 1.0f, 0)))
		game->getText()->writeDebugOutput("\ngraphicsDevice->Clear(): FAILED");


	// ALLOWS DRAWING TO START, LOCKING THE GPU
    if (FAILED(graphicsDevice->BeginScene()))
		game->getText()->writeDebugOutput("\ngraphicsDevice->BeginScene(): FAILED");
}
