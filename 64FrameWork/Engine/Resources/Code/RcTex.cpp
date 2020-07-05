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

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);		// 1. 메모리 잠그는 역할, 2. 3인자를 통해서 저장된 버텍스 접근 권한을 얻음
													// 오른쪽 위 
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
		// 0번 스트림을 사용할 버텍스의 구성 요소
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,	0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	0 },

		// 1번 스트림을 사용할 버텍스의 구성 요소

		{ 1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,	D3DDECLUSAGE_TEXCOORD,	1 },
		{ 1, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	2 },
		{ 1, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	3 },
		{ 1, 48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,	4 },
		D3DDECL_END()

	};

	// 위에서 생성한 정점 정보를 기준으로 정점을 대표하는 컴 객체 생성
	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexDeclaration(Element, &m_pDeclaration), E_FAIL);

	return S_OK;
}

void CRcTex::Render_InstanceBuffer(void)
{
	m_pGraphicDev->SetVertexDeclaration(m_pDeclaration);	// setFVF와 마찬가지로 정점 정보를 세팅해주는 함수, 
	m_pGraphicDev->SetIndices(m_pIB);


	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);// 원본 정점을 0번 스트림에 바인딩 시키는 상태
	m_pGraphicDev->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | m_iNumInstance);	// 장치의 스트림에 어떤 값을 바인딩시킬지 결정하는 함수(1인자 : 스트림 번호, 2인자 : 인스턴싱 개수만큼 D3DSTREAMSOURCE_INDEXEDDATA 와 OR 연산을 하여 설정하면 렌더링하는 인스턴스 값이 논리적으로 결합, 파티클 시에 보편적으로 1을 사용)

																							// 복제본 정점을 1번 스트림에 바인딩 시키는 상태
	m_pGraphicDev->SetStreamSource(1, m_pVBTransform, 0, sizeof(VTXMATRIX));
	m_pGraphicDev->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);	// 장치의 스트림에 어떤 값을 바인딩시킬지 결정하는 함수(1인자 : 스트림 번호, 2인자 : 인스턴싱 개수만큼 D3DSTREAMSOURCE_INSTANCEDATA 와 OR 연산을 하여 설정하면 렌더링하는 인스턴스 값이 논리적으로 결합, 파티클 시에 보편적으로 1을 사용)

	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);

	// 인스턴스 데이터를 렌더링으로 마치고 난 뒤, 스트림 주파수를 기본 상태로 돌려놔야 한다. 
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

