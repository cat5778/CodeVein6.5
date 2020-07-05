#include "stdafx.h"
#include "3DUI.h"
#include "3DButton.h"
#include "Export_Function.h"
#include "ThirdPersonCamera.h"
#include "Player.h"
#include "Image.h"
C3DUI::C3DUI(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, _float fLength, _float fRotY, _bool bIsRight ,UISTATE eUIState)
	: Engine::CGameObject(pGraphicDev), m_wstrTexName(wstrTexName), m_fLength(fLength), m_fRotY(fRotY), m_bIsRight(bIsRight), m_eUIState(eUIState)
{
}

C3DUI::C3DUI(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, _float fLength)
	: Engine::CGameObject(pGraphicDev), m_wstrTexName(wstrTexName), m_fLength(fLength), m_fRotY(0.f), m_bIsRight(false)
{
}

C3DUI::~C3DUI(void)
{

}

HRESULT C3DUI::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_vScale.x = m_pTextureCom->Get_ImageInfo().Width*0.001f;
	m_vScale.y = m_pTextureCom->Get_ImageInfo().Height*0.001f;
	m_vScale.z = m_pTextureCom->Get_ImageInfo().Width*0.001f;
	
	m_pTransformCom->Set_Scale(m_vScale.x, m_vScale.y, m_vScale.z);

	return S_OK;
}

HRESULT C3DUI::LateReady_GameObject(void)
{

	m_pCam = dynamic_cast<CThirdPersonCamera*>(Engine::Get_GameObject(L"UI", L"ThirdPersonCamera"));
	if (m_eUIState >= UI_SHOP_SUB)
	{
		CGameObject* pGameObject = nullptr;
		pGameObject = m_pButton = C3DButton::Create(m_pGraphicDev, L"Select", m_pTransformCom, m_eUIState);
		wstring wstrButton = m_wstrInstName + L"_BT";
		Engine::Get_Layer(L"UI")->Add_GameObject(wstrButton.c_str(), pGameObject);

	}
	else if(m_eUIState<= UI_PORTAL)
	{
		CGameObject* pGameObject = nullptr;
		pGameObject = m_pButton = C3DButton::Create(m_pGraphicDev, L"Select2",m_pTransformCom , m_eUIState);
		wstring wstrButton = m_wstrInstName + L"_BT2";
		Engine::Get_Layer(L"UI")->Add_GameObject(wstrButton.c_str(), pGameObject);
	}


	return S_OK;
}

_int C3DUI::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsOn)
		return 0;
	if (!m_bIsMid)
	{
		if (m_eUIState < UI_SHOP_SUB)
		{
			if (Engine::Get_DIKeyState(DIK_LEFT) || Engine::Get_DIKeyState(DIK_RIGHT))
			{
				m_pTransformCom->Set_Scale(m_vScale.x*1.5f, m_vScale.y*1.5f, m_vScale.z*1.5f);
				//m_pButton->Set_SelectParent(true);


			}
			else
			{
				if (Engine::Get_DIKeyState(DIK_UP) || Engine::Get_DIKeyState(DIK_DOWN))
				{
					m_pTransformCom->Set_Scale(m_vScale.x, m_vScale.y, m_vScale.z);
					//m_pButton->Set_SelectParent(false);
				}
			}
		}
		else
		{
			if (Engine::Get_DIKeyState(DIK_LEFT) || Engine::Get_DIKeyState(DIK_RIGHT))
			{
				m_pTransformCom->Set_Scale(m_vScale.x, m_vScale.y, m_vScale.z);
				//m_pButton->Set_SelectParent(false);
			}
			else
			{
				if (Engine::Get_DIKeyState(DIK_UP) || Engine::Get_DIKeyState(DIK_DOWN))
				{
					m_pTransformCom->Set_Scale(m_vScale.x*1.5f, m_vScale.y*1.5f, m_vScale.z*1.5f);
					//m_pButton->Set_SelectParent(true);

				}
			}
		}
	}
	Engine::CGameObject::Update_GameObject(fTimeDelta);
	
	BillBoard();
	
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return 0;
}

void C3DUI::Render_GameObject(void)
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

HRESULT C3DUI::Add_Component(void)
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



HRESULT C3DUI::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
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

void C3DUI::BillBoard()
{

	_vec3 vUp = { 0.f, 1.f, 0.f };
	_vec3 vLook = m_pCam->Get_Look();
	_vec3 vRight;
	D3DXVec3Normalize(&vLook, &vLook);
	D3DXVec3Cross(&vRight, &vUp, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);

	_vec3 vDir;
	if (!m_bIsMid)
		m_bIsRight ? vDir = vRight*m_fGap + vLook : vDir = vRight*-m_fGap + vLook;
	else
		vDir = vLook;
	D3DXVec3Normalize(&vDir, &vDir);

	_vec3 vTargetPos = m_pCam->Get_CamPos() + (vDir*m_fLength);
	//
	m_pTransformCom->Set_Pos(vTargetPos.x, vTargetPos.y, vTargetPos.z);


	_matrix	matWorld, matView, matBill;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);


	_matrix matY;
	D3DXMatrixRotationY(&matY, D3DXToRadian(m_fRotY));
	m_pTransformCom->Set_WorldMatrix(&(matBill*matY * matWorld));

	Engine::CGameObject::Compute_ViewZ(&m_pTransformCom->m_vInfo[Engine::INFO_POS]);

}

void C3DUI::TestPos()
{

	if (CKeyMgr::GetInstance()->KeyPressing(KEY_LEFT))
	{
		
	}
	if (CKeyMgr::GetInstance()->KeyPressing(KEY_RIGHT))
	{

	}
	if (CKeyMgr::GetInstance()->KeyPressing(KEY_UP))
	{

	}
	if (CKeyMgr::GetInstance()->KeyPressing(KEY_DOWN))
	{

	}

}

void C3DUI::InsertSlot()
{
}


void C3DUI::ChangeEnable()
{
	m_bIsOn = !m_bIsOn;
	if(m_pButton!=nullptr)
		m_pButton->ChangeEnable(m_bIsOn);


}

void C3DUI::ChangeEnable(_bool bIsEnable)
{
	m_bIsOn = bIsEnable;
	if (m_pButton != nullptr)
		m_pButton->ChangeEnable(m_bIsOn);

}

void C3DUI::InteractionUI()
{
}

wstring C3DUI::Get_ItemName()
{
	wstring wstrItem;
	switch (m_eUIState)
	{
	case UI_SHOP:
	{
		switch (m_pButton->Get_ButtonIdx())
		{
		case 0:
			wstrItem = L"왕의대검";
			break;
		case 1:
			wstrItem = L"기사의대검";
			break;
		case 2:
			wstrItem = L"병사의할버드";
			break;
		default:
			wstrItem = L"";
			break;
		} 

	}
		break;
	case UI_INVEN:
		break;
	case UI_PORTAL:
		break;
	case UI_SHOP_SUB:
	{
		switch (m_pButton->Get_ButtonIdx())
		{
		case 0:
			wstrItem = L"구매";
			break;
		case 1:
			wstrItem = L"판매";
			break;
		case 2:
			wstrItem = L"강화";
			break;
		default:
			wstrItem = L"";
			break;
		}
	}
	break;
	case UI_INVEN_SUB:
		break;
	case UI_PORTAL_SUB:
		break;
	case UI_END:
		break;
	default:
		break;
	}

	return wstrItem;
}

void C3DUI::Set_Mid()
{
	m_bIsMid = true;
}

C3DUI* C3DUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, _float fLength, _float fRotY, _bool bIsLeft, UISTATE eUIState)
{
	C3DUI*	pInstance = new C3DUI(pGraphicDev, wstrTexName, fLength,fRotY,bIsLeft, eUIState);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void C3DUI::Free(void)
{


	Engine::CGameObject::Free();
}

