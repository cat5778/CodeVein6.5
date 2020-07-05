#include "stdafx.h"
#include "Portal.h"
#include "3DButton.h"
#include "Export_Function.h"
#include "ThirdPersonCamera.h"
#include "Player.h"
#include "Image.h"
#include "3DIcon.h"

#define	ITEM_NUM 3
CPortal::CPortal(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, _float fLength, _float fRotY, _bool bIsRight ,UISTATE eUIState)
	:C3DUI(pGraphicDev,wstrTexName,fLength,fRotY,bIsRight,eUIState)
{
	m_wstrInstName = L"PortalUI";
}

CPortal::~CPortal(void)
{

}

HRESULT CPortal::Ready_GameObject(void)
{
	C3DUI::Ready_GameObject();

	m_fGap = 0.2f;
	return S_OK;
}

HRESULT CPortal::LateReady_GameObject(void)
{

	C3DUI::LateReady_GameObject();

	CGameObject* pGameObject = nullptr;
	wstring wstrButton;
	pGameObject = m_pIcon = C3DIcon::Create(m_pGraphicDev, L"Waypoint", m_pTransformCom, m_eUIState);
	wstrButton = m_wstrInstName + L"_Icon1";
	Engine::Get_Layer(L"UI")->Add_GameObject(wstrButton.c_str(), pGameObject);
	


	return S_OK;
}

_int CPortal::Update_GameObject(const _float& fTimeDelta)
{
	//현재 포탈에 아이콘 박는중 현재 위치 0,1000 로되어있음 나중에 확인


	if (!m_bIsOn)
		return 0;


	if (Engine::Get_DIKeyState(DIK_LEFT) || Engine::Get_DIKeyState(DIK_RIGHT))
	{
		m_pTransformCom->Set_Scale(m_vScale.x*1.5f, m_vScale.y*1.5f, m_vScale.z*1.5f);
		if (Engine::Get_DIKeyState(DIK_LEFT))
			m_vTempTest.x -= 0.01f;
		if (Engine::Get_DIKeyState(DIK_RIGHT))
			m_vTempTest.x += 0.01f;
	}
	else
	{
		if (Engine::Get_DIKeyState(DIK_UP) || Engine::Get_DIKeyState(DIK_DOWN))
		{
			m_pTransformCom->Set_Scale(m_vScale.x, m_vScale.y, m_vScale.z);

			if (Engine::Get_DIKeyState(DIK_UP))
				m_vTempTest.y+= 0.01f;
			if (Engine::Get_DIKeyState(DIK_DOWN))
				m_vTempTest.y -= 0.01f;
			m_vTempTest.z = -0.001f;
		}
	}

	if (CKeyMgr::GetInstance()->KeyDown(KEY_LEFT))
	{
		if (m_uiWaypointIdx >0)
			m_uiWaypointIdx--;
	}
	if (CKeyMgr::GetInstance()->KeyDown(KEY_RIGHT))
	{
		m_uiWaypointIdx++;
		if (m_uiWaypointIdx > 3)
			m_uiWaypointIdx = 3;
	}
	switch (m_uiWaypointIdx)
	{
	case 0:
		m_pIcon->Set_ButtonPos(_vec3(0.38f, 0.5f, -0.001f));
		break;
	case 1:
		m_pIcon->Set_ButtonPos(_vec3(0.12f, 0.5f, -0.001f));
		break;
	case 2:
		m_pIcon->Set_ButtonPos(_vec3(-0.13f, 0.5f, -0.001f));
		break;
	case 3:
		m_pIcon->Set_ButtonPos(_vec3(-0.38f, 0.5f, -0.001f));
		break;
	}

	//m_pTransformCom->Set_Pos(m_vTempTest.x, m_vTempTest.y, m_vTempTest.z);

	Engine::CGameObject::Update_GameObject(fTimeDelta);
	C3DUI::BillBoard();

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return 0;
}

void CPortal::Render_GameObject(void)
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

_uint CPortal::Get_PortalIdx()
{
	return m_uiWaypointIdx;
}

HRESULT CPortal::Add_Component(void)
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



HRESULT CPortal::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
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

void CPortal::ChangeEnable(_bool bIsEnable)
{
	m_bIsOn = bIsEnable;
	if (m_pButton != nullptr)
	{
		m_pButton->ChangeEnable(m_bIsOn);
		m_pIcon->ChangeEnable(m_bIsOn);
	}
}

void CPortal::ChangeEnable()
{
	m_bIsOn = !m_bIsOn;
	if (m_pButton != nullptr)
	{
		m_pButton->ChangeEnable(m_bIsOn);
	}
	m_pIcon->ChangeEnable(m_bIsOn);
	/*	for (int i = 0; i < 2; i++)
			m_pIcon[i]->ChangeEnable(m_bIsOn);*/
}

CPortal* CPortal::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, _float fLength, _float fRotY, _bool bIsLeft, UISTATE eUIState)
{
	CPortal*	pInstance = new CPortal(pGraphicDev, wstrTexName, fLength,fRotY,bIsLeft, eUIState);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CPortal::Free(void)
{
	C3DUI::Free();
}

