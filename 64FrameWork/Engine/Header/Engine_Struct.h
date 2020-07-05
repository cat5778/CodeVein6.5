#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef	struct tagVertexColor
	{
		_vec3			vPos;
		_ulong			dwColor;

	}VTXCOL;



	const _ulong		FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	typedef	struct tagVertexTexture
	{
		_vec3			vPos;
		_vec3			vNormal;
		_vec2		 	vTexUV;

	}VTXTEX;

	const _ulong		FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef	struct tagVertexCubeTexture
	{
		_vec3			vPos;
		_vec3		 	vTex;

	}VTXCUBE;

	const _ulong		FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

	typedef struct tagScreenVertex
	{
		_vec4				vPos;
		_vec2				vTexUV;

	}VTXSCREEN;

	const _ulong		FVF_SCREEN = D3DFVF_XYZRHW | D3DFVF_TEX1;

	typedef	struct tagIndex16
	{
		_ushort	_0, _1, _2;

	}INDEX16;

	typedef	struct tagIndex32
	{
		_ulong	_0, _1, _2;

	}INDEX32;
	typedef struct tagResolution
	{
		_float fWidth, fHeigth;
	}RESOLUTION;
	typedef struct tagSphere
	{
		_vec3 vPosition;
		_float fRadius;
	}SPHERE_DATA;
	typedef struct D3DXFRAME_DERIVED : public D3DXFRAME
	{
		_matrix				CombinedTransformationMatrix;

	}D3DXFRAME_DERIVED;

	typedef struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER
	{
		LPDIRECT3DTEXTURE9*		ppTexture;
		LPDIRECT3DTEXTURE9*		ppNormalTexture;

		LPD3DXMESH				pOriMesh;		// �ִϸ��̼� ��Ű���� ���� ���� �޽� ������ �����ϱ� ����(������ �޽��� ���� ���� ���� �ִ� �޽� ���� ��ü(���� �Һ�))

		_ulong					dwNumBones;		// ���� ����

		
		
		_matrix*				pFrameOffsetMatrix;			// ������ ������� ���� �ִ� ���(�ִϸ��̼��� ������ ���)


		_matrix**				ppFrameCombinedMatrix;		// ������ ���� �ִ� CombinedTransformationMatrix�� �ּҰ��� �����ϱ� ���� ���� ������(�� �����Ӹ��� ������ �ؾ��ϱ� ������ �Ź� ���� �������� ����� ������ ������ �� �ո����̴�. �׷��� �Ѳ����� �����Ϸ� �ϴ� ���̴�.)
		_matrix*				pRenderingMatrix;		   // �ᱹ ���� ���� ���� ���� �����ϴ� ���(pFrameOffsetMatrix * ppFrameCombinedMatrix[i])


	}D3DXMESHCONTAINER_DERIVED;

	typedef	struct tagVertexPointTexture
	{
		_vec3			vPos;
		_float			fPSize;
		_vec2			vTexUV;

	}VTXPTTEX;

	typedef	struct tagInstanceVertexTexture
	{
		_vec3			vPos;
		_vec2		 	vTexUV;

	}VTXINTEX;


	typedef	struct tagVertexMatrix
	{
		_vec4		vRight;
		_vec4		vUp;
		_vec4		vLook;
		_vec4		vPos;

	}VTXMATRIX;


	typedef struct Navi_Data
	{
		_vec3 vPosition1;
		_vec3 vPosition2;
		_vec3 vPosition3;
		_uint uiIdx;
	}NAVI_DATA;

	typedef struct Coll_data
	{
		_float	fRadius;
		_vec3	vPosition;
	}COLL_DATA;
}


#endif // Engine_Struct_h__
