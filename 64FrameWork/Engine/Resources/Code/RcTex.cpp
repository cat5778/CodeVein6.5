#include "RcTex.h"

USING(Engine)

Engine::CRcTex::CRcTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
	, m_pVBTransform(nullptr)
	, m_pDeclaration(nullptr)
{

}

Engine::CRcTex::CRcTex(const CRcTex& rhs)
	: CVIBuffer(rhs)
	, m_iNumInstance(rhs.m_iNumInstance)
	, m_pVBTransform(rhs.m_pVBTransform)
	, m_pDeclaration(rhs.m_pDeclaration)
{
	if (nullptr != m_pVBTransform)
		m_pVBTransform->AddRef();

	if (nullptr != m_pDeclaration)
		m_pDeclaration->AddRef();
}

Engine::CRcTex::~CRcTex(void)
{

}

HRESULT Engine::CRcTex::Ready_Buffer(void)
{
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = 4;
	m_dwVtxFVF = FVF_TEX;
	m_dwTriCnt = 2;

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXTEX*			pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);		// 1. �޸� ��״� ����, 2. 3���ڸ� ���ؼ� ����� ���ؽ� ���� ������ ����
													// ������ �� 
	pVertex[0].vPos = _vec3(-0.5f, 0.5f, 0.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPos = _vec3(0.5f, 0.5f, 0.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPos = _vec3(0.5f, -0.5f, 0.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPos = _vec3(-0.5f, -0.5f, 0.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();

	INDEX16*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// 0
	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	// 1
	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CRcTex::Ready_Buffer(_uint iNumInstance)
{
	m_iNumInstance = iNumInstance;

	m_dwVtxSize = sizeof(VTXINTEX);
	m_dwVtxCnt = 4;
	m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_TEX1;
	m_dwTriCnt = 2 * iNumInstance;

	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXINTEX*			pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = _vec3(-0.5f, 0.5f, 0.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPos = _vec3(0.5f, 0.5f, 0.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPos = _vec3(0.5f, -0.5f, 0.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPos = _vec3(-0.5f, -0.5f, 0.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(sizeof(VTXMATRIX) * iNumInstance,
		0,
		m_dwVtxFVF,
		D3DPOOL_MANAGED,
		&m_pVBTransform,
		NULL),
		E_FAIL);

	D3DXMATRIX	matPivot;
	D3DXMatrixIdentity(&matPivot);


	VTXMATRIX* pVtxMatrix = NULL;

	m_pVBTransform->Lock(0, 0, (void**)&pVtxMatrix, 0);

	for (_uint i = 0; i < iNumInstance; ++i)
	{
		D3DXMatrixTranslation(&matPivot, rand() % 20, 0.f, rand() % 20);

		pVtxMatrix[i].vRight = *(_vec4*)&matPivot.m[0][0];
		pVtxMatrix[i].vUp = *(_vec4*)&matPivot.m[1][0];
		pVtxMatrix[i].vLook = *(_vec4*)&matPivot.m[2][0];
		pVtxMatrix[i].vPos = *(_vec4*)&matPivot.m[3][0];
	}

	m_pVBTransform->Unlock();

	INDEX16*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	_ulong dwTriCnt = 0;

	for (_uint i = 0; i < iNumInstance; ++i)
	{
		// 0
		pIndex[dwTriCnt]._0 = 0;
		pIndex[dwTriCnt]._1 = 1;
		pIndex[dwTriCnt]._2 = 2;
		++dwTriCnt;

		// 1
		pIndex[dwTriCnt]._0 = 0;
		pIndex[dwTriCnt]._1 = 2;
		pIndex[dwTriCnt]._2 = 3;
		++dwTriCnt;
	}

	m_pIB->Unlock();

	D3DVERTEXELEMENT9		Element[MAX_FVF_DECL_SIZE] = {
		// 0�� ��Ʈ���� ����� ���ؽ��� ���� ���
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0 },

		// 1�� ��Ʈ���� ����� ���ؽ��� ���� ���

		{ 1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_TEXCOORD,	1 },
		{ 1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	2 },
		{ 1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	3 },
		{ 1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	4 },
		D3DDECL_END()

	};

	// ������ ������ ���� ������ �������� ������ ��ǥ�ϴ� �� ��ü ����
	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexDeclaration(Element, &m_pDeclaration), E_FAIL);

	return S_OK;
}

void CRcTex::Render_InstanceBuffer(void)
{
	m_pGraphicDev->SetVertexDeclaration(m_pDeclaration);	// setFVF�� ���������� ���� ������ �������ִ� �Լ�, 
	m_pGraphicDev->SetIndices(m_pIB);


	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);// ���� ������ 0�� ��Ʈ���� ���ε� ��Ű�� ����
	m_pGraphicDev->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | m_iNumInstance);	// ��ġ�� ��Ʈ���� � ���� ���ε���ų�� �����ϴ� �Լ�(1���� : ��Ʈ�� ��ȣ, 2���� : �ν��Ͻ� ������ŭ D3DSTREAMSOURCE_INDEXEDDATA �� OR ������ �Ͽ� �����ϸ� �������ϴ� �ν��Ͻ� ���� �������� ����, ��ƼŬ �ÿ� ���������� 1�� ���)

																							// ������ ������ 1�� ��Ʈ���� ���ε� ��Ű�� ����
	m_pGraphicDev->SetStreamSource(1, m_pVBTransform, 0, sizeof(VTXMATRIX));
	m_pGraphicDev->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);	// ��ġ�� ��Ʈ���� � ���� ���ε���ų�� �����ϴ� �Լ�(1���� : ��Ʈ�� ��ȣ, 2���� : �ν��Ͻ� ������ŭ D3DSTREAMSOURCE_INSTANCEDATA �� OR ������ �Ͽ� �����ϸ� �������ϴ� �ν��Ͻ� ���� �������� ����, ��ƼŬ �ÿ� ���������� 1�� ���)

	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);

	// �ν��Ͻ� �����͸� ���������� ��ġ�� �� ��, ��Ʈ�� ���ļ��� �⺻ ���·� �������� �Ѵ�. 
	m_pGraphicDev->SetStreamSourceFreq(0, 1);
	m_pGraphicDev->SetStreamSourceFreq(1, 1);
}

Engine::CResources* Engine::CRcTex::Clone(void)
{
	return new CRcTex(*this);
}

Engine::CRcTex* Engine::CRcTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRcTex*	pInstance = new CRcTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
		Safe_Release(pInstance);

	return pInstance;
}

CRcTex * CRcTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint & iNumInstance)
{
	CRcTex*	pInstance = new CRcTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(iNumInstance)))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CRcTex::Free(void)
{
	Safe_Release(m_pVBTransform);
	Safe_Release(m_pDeclaration);

	CVIBuffer::Free();

}

