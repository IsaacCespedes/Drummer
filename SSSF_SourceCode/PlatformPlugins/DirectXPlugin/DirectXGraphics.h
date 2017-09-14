/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	DirectXGraphics.h

	This class is a GameGraphics class that does all graphics
	management using DirectX. 
*/

#pragma once
#include "stdafx.h"
#include "SSSF_SourceCode\graphics\GameGraphics.h"
#include "SSSF_SourceCode\text\GameText.h"
#include "SSSF_SourceCode\PlatformPlugins\DirectXPlugin\skinnedMesh.h"


// THIS IS OUR DESIRED COLOR FORMAT. HOPEFULLY THE PLAYER'S
// GRAPHICS CARD WILL HAVE IT
const D3DFORMAT DEFAULT_COLOR_FORMAT = D3DFMT_X8R8G8B8;

// THIS IS THE COLOR WE WILL USE TO CLEAR THE SCREEN WITH
const D3DCOLOR	BACKGROUND_COLOR = D3DCOLOR_XRGB(96, 96, 96);

// BY DEFAULT, WE WILL USE THIS FOR TEXTURE DRAWING, IT USES NO TRANSPARENCY
const D3DCOLOR	DEFAULT_ALPHA_COLOR = D3DCOLOR_ARGB(255, 255, 255, 255);


class DirectXGraphics: public GameGraphics
{
private:
	D3DDISPLAYMODE* displayModes;
	vector<D3DDISPLAYMODE> *displayOptions;

	// DIRECT3D STUFF FOR RENDERING
	LPDIRECT3D9				d3d;				
	LPDIRECT3DDEVICE9		graphicsDevice;
	D3DPRESENT_PARAMETERS	presentParameters;
	LPD3DXSPRITE			spriteHandler;
	D3DCOLOR				colorKey;
	D3DCOLOR				fontColor;
	LPD3DXFONT				textFont;
	RECT					textRect;

	SkinnedMesh * drummer;
	LPD3DXMESH mesh;

	// INTERNAL METHODS DEFINED IN DirectXGraphics.cpp
	HRESULT						createDirectXDeviceAndSpriteHandler();
	void						endDirectXFrameRendering();
	void						getDirectXDisplayModes();
	void						renderGUIRenderList();
	void						renderWorldRenderList();
	void						startDirectXFrameRendering();

public:
	// INLINED ACCESSOR METHODS
	D3DCOLOR				getColorKey()				{ return colorKey;			}
	D3DCOLOR				getFontColor()				{ return fontColor;			}
	LPDIRECT3DDEVICE9		getGraphicsDevice()			{ return graphicsDevice;	}
	D3DPRESENT_PARAMETERS	getPresentParameters()		{ return presentParameters;	}
	LPD3DXSPRITE			getSpriteHandler()			{ return spriteHandler;		}

	// METHODS DEFINED IN DirectXGraphics.cpp
	DirectXGraphics(Game *initGame);
	~DirectXGraphics();

	// OVERRIDDEN GameGraphics METHODS DEFINED IN DirectXGraphics.cpp
	void			animate(int l, int r);
	bool			containsDisplayMode(vector<D3DDISPLAYMODE> *displayModes, D3DFORMAT testColorFormat, int testScreenWidth, int testScreenHeight);
	TextureManager* createTextureManager();//wchar_t *initTexturesPath);
	void			findAlternativeDisplayMode(vector<D3DDISPLAYMODE>* displayModes, D3DFORMAT &formatToSet);
	int				getScreenHeight();
	int				getScreenWidth();
	void			initGraphics(	HWND hWnd, bool isFullscreen);
	void			initTextFont(int fontSize);
	void			reloadGraphics();
	void			renderGame(Game *game);
	void			renderTextToDraw(TextToDraw textToDraw);
	void			setColorKey(int r, int g, int b);
	void			setFontColor(int r, int g, int b);
	void			shutdown();
};