//#include "Renderer.h"
//#include "GraphicDev.h"

#include "Export_Utility.h"
#include "GraphicDev.h"

USING(Engine)
IMPLEMENT_SINGLETON(CRenderer)

Engine::CRenderer::CRenderer(void)
	: m_pVB(nullptr)
	, m_pIB(nullptr)
{
}

Engine::CRenderer::~CRenderer(void)
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);


	//Safe_Release(m_pVB2);
	//Safe_Release(m_pIB2);
	Free();
}

void CRenderer::Set_Perspective(const _matrix & matPerspective)
{
	m_matPerspective = matPerspective;
}

HRESULT CRenderer::Ready_Renderer(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	FAILED_CHECK_RETURN(pGraphicDev->CreateVertexBuffer(sizeof(VTXSCREEN) * 4,
		0,
		FVF_SCREEN,
		D3DPOOL_MANAGED,
		&m_pVB,
		NULL),
		E_FAIL);


	FAILED_CHECK_RETURN(pGraphicDev->CreateIndexBuffer(sizeof(INDEX16) * 2,   // ������ �ε��� ������ ũ��
		0,               // ������ ����(���ڰ� 0�� ��� ���� ����)
		D3DFMT_INDEX16,         // �����ϴ� �ε����� �Ӽ���
		D3DPOOL_MANAGED,   // �޸� Ǯ�� ����
		&m_pIB,
		NULL),
		E_FAIL);

	D3DVIEWPORT9      ViewPort;
	pGraphicDev->GetViewport(&ViewPort);

	VTXSCREEN*      pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = _vec4(0.f, 0.f, 0.f, 1.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPos = _vec4(_float(ViewPort.Width), 0.f, 0.f, 1.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPos = _vec4(_float(ViewPort.Width), _float(ViewPort.Height), 0.f, 1.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPos = _vec4(0.f, _float(ViewPort.Height), 0.f, 1.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();

	INDEX16*      pIndex = nullptr;

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


	//Ready_Renderer(pGraphicDev, &m_pVB2, &m_pIB2);



	return S_OK;
}
//
//HRESULT CRenderer::Ready_Renderer(LPDIRECT3DDEVICE9& pGraphicDev, LPDIRECT3DVERTEXBUFFER9* _vb, LPDIRECT3DINDEXBUFFER9* _ib)
//{
//   FAILED_CHECK_RETURN(pGraphicDev->CreateVertexBuffer(sizeof(VTXSCREEN) * 4,
//      0,
//      FVF_SCREEN,
//      D3DPOOL_MANAGED,
//      _vb,
//      NULL),
//      E_FAIL);
//
//
//   FAILED_CHECK_RETURN(pGraphicDev->CreateIndexBuffer(sizeof(INDEX16) * 2,   // ������ �ε��� ������ ũ��
//      0,               // ������ ����(���ڰ� 0�� ��� ���� ����)
//      D3DFMT_INDEX16,         // �����ϴ� �ε����� �Ӽ���
//      D3DPOOL_MANAGED,   // �޸� Ǯ�� ����
//      _ib,
//      NULL),
//      E_FAIL);
//
//   D3DVIEWPORT9      ViewPort;
//   pGraphicDev->GetViewport(&ViewPort);
//
//   VTXSCREEN*      pVertex = nullptr;
//
//   (*_vb)->Lock(0, 0, (void**)&pVertex, 0);
//
//   pVertex[0].vPos = _vec4(0.f, 0.f, 0.f, 1.f);
//   pVertex[0].vTexUV = _vec2(0.f, 0.f);
//
//   pVertex[1].vPos = _vec4(_float(ViewPort.Width), 0.f, 0.f, 1.f);
//   pVertex[1].vTexUV = _vec2(1.f, 0.f);
//
//   pVertex[2].vPos = _vec4(_float(ViewPort.Width), _float(ViewPort.Height), 0.f, 1.f);
//   pVertex[2].vTexUV = _vec2(1.f, 1.f);
//
//   pVertex[3].vPos = _vec4(0.f, _float(ViewPort.Height), 0.f, 1.f);
//   pVertex[3].vTexUV = _vec2(0.f, 1.f);
//
//   (*_vb)->Unlock();
//
//   INDEX16*      pIndex = nullptr;
//
//   (*_ib)->Lock(0, 0, (void**)&pIndex, 0);
//
//   // 0
//   pIndex[0]._0 = 0;
//   pIndex[0]._1 = 1;
//   pIndex[0]._2 = 2;
//
//   // 1
//   pIndex[1]._0 = 0;
//   pIndex[1]._1 = 2;
//   pIndex[1]._2 = 3;
//
//   (*_ib)->Unlock();
//
//   return S_OK;
//}

void Engine::CRenderer::Add_RenderGroup(RENDERID eGroup, CGameObject* pGameObject)
{
	if (RENDER_END <= eGroup)
		return;
	m_RenderGroup[eGroup].push_back(pGameObject);
	pGameObject->AddRef();
}

void Engine::CRenderer::Render_GameObject(void)
{
	// ���� ���� ����
	RenderState_Projection(PROJ_PERSPECTIVE);

	for (_uint i = 0; i < RENDER_END; ++i)
	{
		// ���� ���� ����
		if (RENDER_UI == i)
			RenderState_Projection(PROJ_ORTHOGRAPH);

		for (auto& iter : m_RenderGroup[i])
		{
			iter->Render_GameObject();
			Safe_Release(iter);
		}
		m_RenderGroup[i].clear();
	}
	RenderState_Projection(PROJ_PERSPECTIVE);
}

void CRenderer::Render_GameObject(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	//Render_Priority(pGraphicDev);
	Render_Defferd(pGraphicDev);
	
	Render_Distortion(pGraphicDev);

	Render_LightAcc(pGraphicDev);

	Render_Blend(pGraphicDev);
	
	Render_Final(pGraphicDev);

	Render_Alpha(pGraphicDev);
	Render_UI(pGraphicDev);

	if (m_bIsDebugMode)
	{
		Engine::Render_DebugBuffer(L"MRT_Defferd");
		Engine::Render_DebugBuffer(L"MRT_LightAcc");
		Engine::Render_DebugBuffer(L"MRT_Blend");
		Engine::Render_DebugBuffer(L"MRT_Distortion");
	}
	Clear_RenderGroup();
}

void Engine::CRenderer::Clear_RenderGroup(void)
{
	for (_uint i = 0; i < RENDER_END; ++i)
	{
		for_each(m_RenderGroup[i].begin(), m_RenderGroup[i].end(), CDeleteObj());
		m_RenderGroup[i].clear();
	}
}

void CRenderer::Change_RenderMode()
{
	m_bIsDebugMode = !m_bIsDebugMode;
}

void CRenderer::RenderState_Projection(PROJECTIONTYPE eProjType)
{
	//switch (eProjType)
	//{
	//case PROJ_PERSPECTIVE:
	//   CGraphicDev::GetInstance()->Get_Device()->SetTransform(D3DTS_PROJECTION, &m_matPerspective);
	//   break;
	//case PROJ_ORTHOGRAPH:
	//   RESOLUTION tResolution;
	//   CGraphicDev::GetInstance()->Get_Resolution(tResolution);
	//   LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_Device();

	//   _matrix matWorld, matView, matProj;

	//   D3DXMatrixIdentity(&matWorld);
	//   matProj = matView = matWorld;

	//   D3DXMatrixOrthoLH(&matProj, tResolution.fWidth, tResolution.fHeigth, 0.f, 1.f);

	//   pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	//   pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
	//   pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
	//   break;
	//}

}


void CRenderer::Render_Priority(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	//pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	//pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	for (auto& iter : m_RenderGroup[RENDER_PRIORITY])
		iter->Render_GameObject();

	//pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void CRenderer::Render_NonAlpha(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	for (auto& iter : m_RenderGroup[RENDER_NONALPHA])
		iter->Render_GameObject();
}

_bool   Compare_Z(CGameObject* pDest, CGameObject* pSour)
{
	return pDest->Get_ViewZ() > pSour->Get_ViewZ();
}

void CRenderer::Render_Alpha(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	//pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	//pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_RenderGroup[RENDER_ALPHA].sort(Compare_Z);

	for (auto& iter : m_RenderGroup[RENDER_ALPHA])
		iter->Render_GameObject();

	//pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void CRenderer::Render_UI(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	//pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	//pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	//pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	//pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_RenderGroup[RENDER_UI].sort(Compare_Z);

	for (auto& iter : m_RenderGroup[RENDER_UI])
		iter->Render_GameObject();

	//pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	//pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
}

void CRenderer::Render_Blend(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	Engine::Begin_MRT(L"MRT_Blend");

	CShader*   pShader = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Blend"));
	NULL_CHECK(pShader);

	LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();
	pEffect->AddRef();

	pEffect->CommitChanges(); //�߰� 

	Engine::SetUp_OnShader(pEffect, L"Target_Albedo", "g_AlbedoTexture");
	Engine::SetUp_OnShader(pEffect, L"Target_Light", "g_LightTexture");
	Engine::SetUp_OnShader(pEffect, L"Target_Specular", "g_SpecularTexture");

	//Engine::SetUp_OnShader(pEffect, L"Target_Normal", "g_NormalTexture");
	//Engine::SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(FVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
	Safe_Release(pShader);

	Engine::End_MRT(L"MRT_Blend");
}

void CRenderer::Render_Distortion(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	Engine::Begin_MRT(L"MRT_Distortion");

	m_RenderGroup[RENDER_DISTORTION].sort(Compare_Z); //�߰� 

	for (auto& iter : m_RenderGroup[RENDER_DISTORTION])
		iter->Render_GameObject();

	Engine::End_MRT(L"MRT_Distortion");

}

void CRenderer::Render_Final(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	CShader*   pShader = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Final"));
	NULL_CHECK(pShader);

	LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();
	pEffect->AddRef();

	Engine::SetUp_OnShader(pEffect, L"Target_Distortion", "g_DistortionTexture");
	Engine::SetUp_OnShader(pEffect, L"Target_Blend", "g_BlendTexture");

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(FVF_SCREEN);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pEffect);
	Safe_Release(pShader);
}

void CRenderer::Render_LightAcc(LPDIRECT3DDEVICE9 & pGraphicDev)
{
	Engine::Begin_MRT(L"MRT_LightAcc");

	CShader*   pShader = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Light"));
	NULL_CHECK(pShader);

	LPD3DXEFFECT pEffect = pShader->Get_EffectHandle();
	pEffect->AddRef();

	Engine::SetUp_OnShader(pEffect, L"Target_Normal", "g_NormalTexture");
	Engine::SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");
	Engine::SetUp_OnShader(pEffect, L"Target_WorldPos", "g_WorldTexture");

	//pEffect->SetVector()

	pEffect->Begin(NULL, 0);

	Engine::Render_Light(pEffect);

	pEffect->End();

	Engine::End_MRT(L"MRT_LightAcc");

	Safe_Release(pEffect);
	Safe_Release(pShader);
}

void Engine::CRenderer::Render_Defferd(LPDIRECT3DDEVICE9& pGraphicDev)
{
	Engine::Begin_MRT(L"MRT_Defferd");

	Render_NonAlpha(pGraphicDev);

	Engine::End_MRT(L"MRT_Defferd");
}

void Engine::CRenderer::Free(void)
{
	Clear_RenderGroup();
}
