#include "stdafx.h"
#include "RussianHatHont.h"
#include "Export_Function.h"
#include "Player.h"

CRussainHatHone::CRussainHatHone(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos,_float fAngle)
	: Engine::CGameObject(pGraphicDev),m_fAngle(fAngle)
{
	m_vShootPos = vPos;
}

CRussainHatHone::~CRussainHatHone()
{
}

HRESULT CRussainHatHone::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Set_Scale(0.001f, 0.001f, 0.001f);
	m_pTargetTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Transform", Engine::ID_DYNAMIC));
	
	
	m_pTransformCom->Set_Pos(m_vShootPos.x, m_vShootPos.y, m_vShootPos.z);
	m_pTransformCom->Rotation(Engine::ROT_Y, m_fAngle);
	return S_OK;
}

void CRussainHatHone::Ready_SphereMesh()
{
	LPD3DXMESH	pSphereMesh = nullptr;
	Engine::VTXCOL* pVertices = nullptr;

	D3DXCreateSphere(m_pGraphicDev, 1.f, 10, 10, &pSphereMesh, nullptr);

	LPDIRECT3DVERTEXBUFFER9 pSphereBuffer;

	pSphereMesh->CloneMeshFVF(0, Engine::FVF_COL, m_pGraphicDev, &m_pSphereMesh);
	m_pSphereMesh->GetVertexBuffer(&pSphereBuffer);
	_uint uiNumVtx = m_pSphereMesh->GetNumVertices();

	pSphereBuffer->Lock(0, 0, (void**)&pVertices, 0);

	for (_uint i = 0; i < uiNumVtx; i++)
		pVertices[i].dwColor = D3DCOLOR_RGBA(0, 255, 0, 255);

	pSphereBuffer->Unlock();
	pSphereBuffer->Release();

	pSphereMesh->Release();

}

HRESULT CRussainHatHone::LateReady_GameObject()
{
	//m_vDir = m_pTargetTransformCom->m_vInfo[Engine::INFO_POS] - m_vShootPos;
	//D3DXVec3Normalize(&m_vDir, &m_vDir);

	return S_OK;
}

_int CRussainHatHone::Update_GameObject(const _float & fTimeDelta)
{

	if (m_bIsColl)
	{
		return S_OK;

	}
	else
	{
		if (m_fScale < 50.f)
			m_fScale += fTimeDelta*800.f;
		else
		{
			m_fScale = 80.f;
			Is_Coll();

		}
	}
	m_pTransformCom->Set_Scale(0.001f*m_fScale, 0.001f*m_fScale, 0.001f*m_fScale);
	//cout << m_fScale << endl;
	//if (m_fTime >= 10.f)
	//	m_bEnable = false;
	//else
	//	m_fTime += fTimeDelta;
	//m_pTransformCom->Move_Pos(&(m_vDir*m_fSpeed*fTimeDelta));
	CGameObject::Update_GameObject(fTimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);


	return S_OK;
}

void CRussainHatHone::Render_GameObject(void)
{

	_matrix mat = m_pTransformCom->m_matWorld;

	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_uint	iPassMax = 0;

	SetUp_ConstantTable(pEffect);

	pEffect->Begin(&iPassMax, 0);

	pEffect->BeginPass(0);

	m_pMeshCom->Render_Meshes(pEffect);

	pEffect->EndPass();

	pEffect->End();

	Safe_Release(pEffect);


#ifdef _DEBUG
	if (m_pSphereMesh != nullptr)
	{
		m_pGraphicDev->BeginScene();

		//_matrix matOldWorld;
		//m_pTransformCom->Get_WorldMatrix(&matOldWorld);

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);

		m_pGraphicDev->GetTransform(D3DTS_WORLD, &m_matSphereWorld);


		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME); //Test
		D3DMATERIAL9 mtrl, Oldmtrl;
		m_pGraphicDev->GetMaterial(&Oldmtrl);

		ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
		if (m_bIsColl)
		{
			mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
			mtrl.Diffuse.g = mtrl.Ambient.g = 0.0f;
			mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
			mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
		}
		else
		{
			mtrl.Diffuse.r = mtrl.Ambient.r = 0.0f;
			mtrl.Diffuse.g = mtrl.Ambient.g = 0.0f;
			mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
			mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
		}

		m_pGraphicDev->SetMaterial(&mtrl);

		m_pSphereMesh->DrawSubset(0);

		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		//m_pGraphicDev->SetTransform(D3DTS_WORLD, &matOldWorld);

		m_pGraphicDev->EndScene();
}
#endif

}

HRESULT CRussainHatHone::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	const D3DLIGHT9*		pLight = Engine::Get_LightInfo(0);
	NULL_CHECK_RETURN(pLight, S_OK);
	pEffect->SetVector("g_vLightDir", &_vec4(pLight->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLight->Diffuse);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLight->Specular);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLight->Ambient);

	D3DMATERIAL9		tMtrlInfo;
	ZeroMemory(&tMtrlInfo, sizeof(D3DMATERIAL9));

	tMtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	tMtrlInfo.Power = 100.f;

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&tMtrlInfo.Diffuse);
	pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&tMtrlInfo.Specular);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&tMtrlInfo.Ambient);

	_vec4		vCamPos;
	D3DXMatrixInverse(&matView, NULL, &matView);
	memcpy(&vCamPos, &matView.m[3][0], sizeof(_vec4));

	pEffect->SetVector("g_vCamPos", &vCamPos);
	pEffect->SetFloat("g_fPower", tMtrlInfo.Power);

	return S_OK;
}

_bool CRussainHatHone::Is_Coll()
{

	_vec3 vDir= *m_pTransformCom->Get_Info(Engine::INFO_POS)- *m_pTargetTransformCom->Get_Info(Engine::INFO_POS);
	_float fLength=D3DXVec3Length(&vDir);
	if (fLength <= 4.f)
	{
		m_bIsColl = true;
		if (!dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"GameLogic", L"Player"))->Get_Dodge())
		{
			dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"GameLogic", L"Player"))
				->Hurt(*m_pTargetTransformCom->Get_Info(Engine::INFO_POS), *m_pTransformCom->Get_Info(Engine::INFO_POS), 10.f);
			return true;
		}

		
		return m_bIsColl = false;
	}
	else
	{
		return m_bIsColl = false;

	}
}

HRESULT CRussainHatHone::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	
	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);

	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(RESOURCE_STAGE, L"RussianHone"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Mesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

#ifdef _DEBUG

	Ready_SphereMesh();
#endif // _DEBUG
	return S_OK;
}


CRussainHatHone * CRussainHatHone::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _float fAngle)
{
	CRussainHatHone*	pInstance = new CRussainHatHone(pGraphicDev, vPos, fAngle);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CRussainHatHone::Free(void)
{
	Safe_Release(m_pSphereMesh);
	Engine::CGameObject::Free();
}
