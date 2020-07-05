#include "stdafx.h"
#include "SkySphere.h"

#include "Export_Function.h"

CSkySphere::CSkySphere(LPDIRECT3DDEVICE9 pGraphicDev,_uint uiStageIdx)
	: Engine::CGameObject(pGraphicDev)
{
	m_uiStageIdx = uiStageIdx;
}


CSkySphere::~CSkySphere(void)
{

}

HRESULT CSkySphere::Ready_GameObject()
{
	Ready_Stage();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(m_fScale, m_fScale, m_fScale);
	//m_pTransformCom->Rotation(Engine::ROT_Z, D3DXToRadian(23.5f)); //나중에 라이팅 태양처럼  같이옮겨주기 
	m_pTransformCom->Rotation(Engine::ROT_Y, D3DXToRadian(210.f)); //나중에 라이팅 태양처럼  같이옮겨주기 

	if (m_uiStageIdx == 1)
	{
		Engine::Get_Light(0)->Position = _vec3(0.f, -400.f, 0.f);
		Engine::Get_Light(0)->Range = 1000.f;
		Engine::Get_Light(0)->Diffuse = D3DXCOLOR(0.8f, 0.8f, 1.0f, 0.35f);

		Engine::Get_Light(1)->Position = _vec3(0.f, 400.f, 0.f);
		Engine::Get_Light(1)->Range = 1000.f;
		Engine::Get_Light(1)->Diffuse = D3DXCOLOR(0.8f, 0.8f, 1.0f, 0.35f);


		for (int i = 2; i < 4; i++)
		{
			Engine::Get_Light(i)->Position = _vec3(INIT_VEC3);
			Engine::Get_Light(i)->Range = 0.f;
			Engine::Get_Light(i)->Diffuse = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		}
	}
	return S_OK;
}


_int CSkySphere::Update_GameObject(const _float& fTimeDelta)
{
	if (m_fRot >= 360.f) //24 360 15
		m_fRot = 0.f;
	else
		m_fRot+=1.f;
	m_pTransformCom->Rotation(Engine::ROT_Y, D3DXToRadian(1.f)); //나중에 라이팅 태양처럼  같이옮겨주기 
	
	//태양
	_matrix matLightX, matLightY, matLightZ, matLight;
	//D3DXMatrixRotationX(&matLightX, D3DXToRadian(23.5f));
	D3DXMatrixRotationY(&matLightY, D3DXToRadian(m_fRot));
	matLight =/*matLightX**/matLightY;
	_vec3 vLightPos = { 4.f, 1.f, 0.f };
	D3DXVec3TransformNormal(&vLightPos, &vLightPos, &matLight);
	_vec3 vPos = vLightPos*m_fLength;
	if(m_uiStageIdx==0)
		Engine::Get_Light(0)->Position = vLightPos*228.f;
	else if (m_uiStageIdx == 1)
	{
		if (CKeyMgr::GetInstance()->KeyPressing(KEY_K))
			m_fLength++;
		

			//cout << m_fLength << endl;
	}
	_matrix	matCamWorld;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matCamWorld);

	D3DXMatrixInverse(&matCamWorld, NULL, &matCamWorld);
	m_pTransformCom->Set_Pos(matCamWorld._41, matCamWorld._42 + 3.f, matCamWorld._43);



	//if (CKeyMgr::GetInstance()->KeyPressing(KEY_F))
	//{
	//	m_fScale += 0.01f;
	//	m_pTransformCom->Set_Scale(m_fScale, m_fScale, m_fScale);

	//}
	//if (CKeyMgr::GetInstance()->KeyPressing(KEY_G))
	//{
	//	m_fScale -= 0.01f;
	//	m_pTransformCom->Set_Scale(m_fScale, m_fScale, m_fScale);
	//}

	//cout << m_fScale << endl;
	//_vec3 vPos =*m_pTargetTransformCom->Get_Info(Engine::INFO_POS);

	//m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	//_matrix mat=	m_pTransformCom->m_matWorld;

	Engine::CGameObject::Update_GameObject(fTimeDelta);
	//RENDER_PRIORITY
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);

	return 0;
}

void CSkySphere::Render_GameObject(void)
{

	//
	//m_pMeshCom->Render_Meshes();

	//_matrix		matWorld;
	//m_pTransformCom->Get_WorldMatrix(&matWorld);

	//m_pColliderCom->Render_Collider(Engine::COLLTYPE(false), &matWorld);
	
	//m_pGraphicDev->GetRenderState(D3DRS_CULLMODE,)
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
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
	//m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);


}

HRESULT CSkySphere::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
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

	return E_NOTIMPL;
}

void CSkySphere::Ready_Stage()
{
	switch (m_uiStageIdx)
	{
	case 0:
		m_fScale = 0.5f;
		m_wstrMeshName = L"SM_Sky_ba01";
		break;
	case 1:
		m_fScale = 0.04f;
		m_wstrMeshName = L"SM_SkySphere_st09a1";

		break;

	default:
		break;
	}
}

HRESULT CSkySphere::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(Engine::Clone(RESOURCE_STAGE, m_wstrMeshName.c_str()));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);

	pComponent = m_pCalculatorCom = Engine::CCalculator::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Calculator", pComponent);

	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Mesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}




CSkySphere* CSkySphere::Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint uiStageIdx)
{
	CSkySphere*	pInstance = new CSkySphere(pGraphicDev, uiStageIdx);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CSkySphere::Free(void)
{
	Engine::CGameObject::Free();
}


