#include "stdafx.h"
#include "DisplayWeapon.h"

#include "Export_Function.h"

CDisplayWeapon::CDisplayWeapon(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrMeshName)
	: Engine::CGameObject(pGraphicDev), m_wstrMeshName(wstrMeshName)
{
	m_bEnable = false;
}

CDisplayWeapon::~CDisplayWeapon(void)
{

}

HRESULT CDisplayWeapon::Ready_GameObject(const _uint& iFlag)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//m_pTransformCom->Rotation(Engine::ROT_X, D3DXToRadian(90.f));
	m_pTransformCom->Rotation(Engine::ROT_Y, D3DXToRadian(180.f));
	m_pTransformCom->Set_Scale(0.002f, 0.002f, 0.002f);

	m_iFlag = iFlag;

	return S_OK;
}

_int CDisplayWeapon::Update_GameObject(const _float& fTimeDelta)
{

	if (nullptr == m_pParentBoneMatrix)
	{

		Engine::CTransform*	pTargetTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"UI", L"InvenUI_InfoUI", L"Com_Transform", Engine::ID_DYNAMIC));
		NULL_CHECK_RETURN(pTargetTransform, 0);

		m_pParentWorldMatrix = pTargetTransform->Get_WorldMatrixPointer();
	}

	m_pTransformCom->Rotation(Engine::ROT_Y, D3DXToRadian(1.f));


	Engine::CGameObject::Update_GameObject(fTimeDelta);
	_vec3 vPos;
	memcpy(&vPos, &m_pParentWorldMatrix->_41, sizeof(_vec3));

	//m_pTransformCom->Set_ParentMatrix(m_pParentWorldMatrix);
	m_pTransformCom->Set_Pos(&vPos);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);


	return 0;
}

void CDisplayWeapon::Render_GameObject(void)
{
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransformCom->m_matWorld);

	//
	//m_pMeshCom->Render_Meshes();

	//_matrix		matWorld;
	//m_pTransformCom->Get_WorldMatrix(&matWorld);

	//m_pColliderCom->Render_Collider(Engine::COLLTYPE(false), &matWorld);


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


}

HRESULT CDisplayWeapon::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
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



HRESULT CDisplayWeapon::Add_Component(void)
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

	//// collider
	//pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev, 
	//														m_pMeshCom->Get_VtxPos(),
	//														m_pMeshCom->Get_NumVtx(), 
	//														m_pMeshCom->Get_Stride());
	//NULL_CHECK_RETURN(pComponent, E_FAIL);

	//m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Mesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);



	return S_OK;
}

_bool CDisplayWeapon::Collision_ToObject(const _tchar* pLayerTag, const _tchar* pObjTag)
{
	Engine::CCollider*	pPlayerColliderCom = dynamic_cast<Engine::CCollider*>(Engine::Get_Component(pLayerTag, pObjTag, L"Com_Collider", Engine::ID_STATIC));
	NULL_CHECK_RETURN(pPlayerColliderCom, false);
	

	if(0 == m_iFlag)
		return m_pCalculatorCom->Collision_OBB(pPlayerColliderCom->Get_Min(),
											pPlayerColliderCom->Get_Max(),
											pPlayerColliderCom->Get_ColliderMatrix(),
											m_pColliderCom->Get_Min(),
											m_pColliderCom->Get_Max(),
											m_pColliderCom->Get_ColliderMatrix());


	else
		return m_pCalculatorCom->Collision_AABB(pPlayerColliderCom->Get_Min(),
											pPlayerColliderCom->Get_Max(),
											pPlayerColliderCom->Get_ColliderMatrix(), 
											m_pColliderCom->Get_Min(), 
											m_pColliderCom->Get_Max(), 
											m_pColliderCom->Get_ColliderMatrix());
}

CDisplayWeapon* CDisplayWeapon::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrMeshName, const _uint& iFlag)
{
	CDisplayWeapon*	pInstance = new CDisplayWeapon(pGraphicDev, wstrMeshName);

	if (FAILED(pInstance->Ready_GameObject(iFlag)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CDisplayWeapon::Free(void)
{
	Engine::CGameObject::Free();
}


