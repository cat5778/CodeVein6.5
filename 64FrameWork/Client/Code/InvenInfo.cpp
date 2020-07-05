#include "stdafx.h"
#include "InvenInfo.h"
#include "3DButton.h"
#include "Export_Function.h"
#include "ThirdPersonCamera.h"
#include "Player.h"
#include "Image.h"
#include "3DIcon.h"
#include "DisplayWeapon.h"
#include "Inven.h"
CInvenInfo::CInvenInfo(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, _float fLength)
	:C3DUI(pGraphicDev,wstrTexName,fLength)
{
	m_wstrInstName = L"InvenInfoUI";
}

CInvenInfo::~CInvenInfo(void)
{

}

HRESULT CInvenInfo::Ready_GameObject(void)
{
	Set_Mid();
	C3DUI::Ready_GameObject();
	m_pTransformCom->Set_Scale(m_vScale.x*1.5f, m_vScale.y*1.5f, m_vScale.z*1.5f);

	return S_OK;
}

HRESULT CInvenInfo::LateReady_GameObject(void)
{
	C3DUI::LateReady_GameObject();

	Engine::CLayer* pLayer = Engine::Get_Layer(L"UI");
	CGameObject* pGameObject = nullptr;

	pGameObject = m_pWeapon[0] = CDisplayWeapon::Create(m_pGraphicDev, L"SM_NormalGreatSwordA_ba01", 0);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Display_SwordA", pGameObject), E_FAIL);

	pGameObject = m_pWeapon[1] = CDisplayWeapon::Create(m_pGraphicDev, L"SM_NormalGreatSwordB_ba01", 0);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Display_SwordB", pGameObject), E_FAIL);

	pGameObject = m_pWeapon[2] = CDisplayWeapon::Create(m_pGraphicDev, L"SK_NormalHalberdB", 0);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Display_Halberd", pGameObject), E_FAIL);


	m_pInven = dynamic_cast<CInven*> (Engine::Get_GameObject(L"UI", L"InvenUI"));
	return S_OK;
}

_int CInvenInfo::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsOn)
		return 0;
	
	if (m_pInven->Get_ItemName().find(L"왕의") != wstring::npos)
	{
		m_pWeapon[0]->Set_Enable(true);
		m_pWeapon[1]->Set_Enable(false);
		m_pWeapon[2]->Set_Enable(false);

	}
	else if (m_pInven->Get_ItemName().find(L"기사의") != wstring::npos)
	{
		m_pWeapon[0]->Set_Enable(false);
		m_pWeapon[1]->Set_Enable(true);
		m_pWeapon[2]->Set_Enable(false);
	}
	else if (m_pInven->Get_ItemName().find(L"병사의") != wstring::npos)
	{
		m_pWeapon[0]->Set_Enable(false);
		m_pWeapon[1]->Set_Enable(false);
		m_pWeapon[2]->Set_Enable(true);
	}
	Engine::CGameObject::Update_GameObject(fTimeDelta);
	BillBoard();
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return 0;
}

void CInvenInfo::Render_GameObject(void)
{

	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_uint	iPassMax = 0;

	SetUp_ConstantTable(pEffect);

	pEffect->Begin(&iPassMax, 0);

	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();

	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT CInvenInfo::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, m_wstrTexName.c_str()));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);
	
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_3DUI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);
	
	return S_OK;
}



HRESULT CInvenInfo::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture", _uint(m_fFrameCnt));
	Engine::SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");



	return S_OK;
}



void CInvenInfo::ChangeEnable(_bool bIsEnable)
{
	m_bIsOn = bIsEnable;
	for (int i = 0; i < 3; i++)
	{
		m_pWeapon[i]->Set_Enable(false);
	}

}

void CInvenInfo::ChangeEnable()
{
	m_bIsOn = !m_bIsOn;
}



CInvenInfo * CInvenInfo::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, _float fLength)
{
	CInvenInfo*	pInstance = new CInvenInfo(pGraphicDev, wstrTexName, fLength);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CInvenInfo::Free(void)
{
	C3DUI::Free();
}

