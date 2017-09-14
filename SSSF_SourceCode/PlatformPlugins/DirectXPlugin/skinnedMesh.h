//////////////////////////////////////////////////////////////////////////
//					Character Animation with Direct3D					//
//						   Author: C. Granberg							//
//							   2008 - 2009								//
//////////////////////////////////////////////////////////////////////////
#include <stdafx.h>

using namespace std;

struct Bone: public D3DXFRAME
{
   D3DXMATRIX exCombinedTransformationMatrix;
};

struct BoneMesh: public D3DXMESHCONTAINER
{
	// The base D3DXMESHCONTAINER has a pMaterials member which is a D3DXMATERIAL structure 
	// that contains a texture filename and material data. It is easier to ignore this and 
	// instead store the data in arrays of textures and materials in this extended structure:
    IDirect3DTexture9**  exTextures;		// Array of texture pointers  
	D3DMATERIAL9*		 exMaterials;		// Array of materials
                                
	// Skinned mesh variables
	ID3DXMesh*           exSkinMesh;			// The skin mesh
	D3DXMATRIX*			 exBoneOffsets;			// The bone matrix Offsets, one per bone
	D3DXMATRIX**		 exFrameCombinedMatrixPointer;	// Array of frame matrix pointers
};

class SkinnedMesh
{
	public:
		SkinnedMesh(LPDIRECT3DDEVICE9 d3dDevice);
		~SkinnedMesh();
		void Load(LPCWSTR fileName);
		void Render(D3DXMATRIX m);
		void SelectAnimation(int l, int r);
		void SetPose(D3DXMATRIX world, float time);

	private:
		void DrawFrame(LPD3DXFRAME frame, D3DXMATRIX m);
		void DrawMeshContainer(LPD3DXMESHCONTAINER meshContainerBase, LPD3DXFRAME frameBase, D3DXMATRIX m);
		void UpdateMatrices(Bone* bone, D3DXMATRIX *parentMatrix);
		void SetupBoneMatrixPointers(Bone *bone, LPD3DXMATRIX pParentMatrix);

		LPDIRECT3DDEVICE9 m_d3dDevice; // note: a pointer copy (not a good idea but for simplicities sake)
		

		LPD3DXFRAME                 m_frameRoot;
		LPD3DXANIMATIONCONTROLLER   m_animController;
		BoneMesh* m_firstMesh;

		// Bone data
		D3DXMATRIX *m_boneMatrices;
		int m_maxBones;
	
		// Animation variables
		unsigned int m_currentAnimationSet;	
		unsigned int m_numAnimationSets;
		unsigned int m_currentTrack;
		float m_currentTime;
		float m_speedAdjust;
		
};