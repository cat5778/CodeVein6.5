#include "stdafx.h"
#include "Shop.h"
#include "3DButton.h"
#include "Export_Function.h"
#include "ThirdPersonCamera.h"
#include "Player.h"
#include "Image.h"
#include "3DIcon.h"

#define	ITEM_NUM 3
CShop::CShop(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, _float fLength, _float fRotY, _bool bIsRight ,UISTATE eUIState)
	:C3DUI(pGraphicDev,wstrTexName,fLength,fRotY,bIsRight,eUIState)
{
	m_wstrInstName = L"ShopUI";

}

CShop::~CShop(void)
{

}

HRESULT CShop::Ready_GameObject(void)
{
	C3DUI::Ready_GameObject();
	m_InvenVec = dynamic_cast<CPlayer*>(Engine::Get_GameObject(L"GameLogic", L"Player"))->Get_InvenVec();



	return S_OK;
}

HRESULT CShop::LateReady_GameObject(void)
{

	C3DUI::LateReady_GameObject();

	CGameObject* pGameObject = nullptr;
	wstring wstrButton;
	pGameObject = m_pIcon[0] = C3DIcon::Create(m_pGraphicDev, L"LargeSword_H_Icon", m_pTransformCom, m_eUIState);
	wstrButton = m_wstrTexName + L"_Icon1";
	Engine::Get_Layer(L"UI")->Add_GameObject(wstrButton.c_str(), pGameObject);

	pGameObject = m_pIcon[1] = C3DIcon::Create(m_pGraphicDev, L"LargeSword_I_Icon", m_pTransformCom, m_eUIState);
	wstrButton = m_wstrTexName + L"_Icon2";
	Engine::Get_Layer(L"UI")->Add_GameObject(wstrButton.c_str(), pGameObject);

	pGameObject = m_pIcon[2] = C3DIcon::Create(m_pGraphicDev, L"Halberd_B_Icon", m_pTransformCom, m_eUIState);
	wstrButton = m_wstrTexName + L"_Icon3";
	Engine::Get_Layer(L"UI")->Add_GameObject(wstrButton.c_str(), pGameObject);


	return S_OK;
}

_int CShop::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsOn)
		return 0;
	CheckInventory();
	if (Engine::Get_DIKeyState(DIK_LEFT) || Engine::Get_DIKeyState(DIK_RIGHT))
	{
		m_pTransformCom->Set_Scale(m_vScale.x*1.5f, m_vScale.y*1.5f, m_vScale.z*1.5f);
	}
	else
	{
		if (Engine::Get_DIKeyState(DIK_UP) || Engine::Get_DIKeyState(DIK_DOWN))
		{
			m_pTransformCom->Set_Scale(m_vScale.x, m_vScale.y, m_vScale.z);
		}
	}

	Engine::CGameObject::Update_GameObject(fTimeDelta);
	BillBoard();
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return 0;
}

void CShop::Render_GameObject(void)
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

HRESULT CShop::Add_Component(void)
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



HRESULT CShop::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
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

void CShop::CheckInventory()
{
	int iWeaponIdx[3] = { -1,-1,-1 };
	int iIDx = 0;
	for (auto Item : (*m_InvenVec))
	{
		if (Item.first.find(L"���Ǵ��") != wstring::npos)
			iWeaponIdx[0] = iIDx;
		if (Item.first.find(L"����Ǵ��") != wstring::npos)
			iWeaponIdx[1] = iIDx;
		if (Item.first.find(L"�������ҹ���") != wstring::npos)
			iWeaponIdx[2] = iIDx;
		iIDx++;
	}

	for (int i = 0; i < 3; i++)
	{
		switch (iWeaponIdx[i])
		{
		case 0:
			m_pIcon[i]->Set_ButtonPos(_vec3{ -0.3f, -0.18f ,-0.02f });
			m_pIcon[i]->Set_Number((*m_InvenVec)[iWeaponIdx[i]].second);
			break;
		case 1:
			m_pIcon[i]->Set_ButtonPos(_vec3{ -0.02f, -0.18f ,-0.02f });
			m_pIcon[i]->Set_Number((*m_InvenVec)[iWeaponIdx[i]].second);
			break;
		case 2:
			m_pIcon[i]->Set_ButtonPos(_vec3{ 0.27f, -0.18f ,-0.02f });
			m_pIcon[i]->Set_Number((*m_InvenVec)[iWeaponIdx[i]].second);
			break;


		default:
			m_pIcon[i]->Set_ButtonPos(_vec3{ 0.f, -1000.f ,0.f });

			break;
		}
	}


	
}



wstring CShop::Get_ItemName()
{
	wstring wstrItem[ITEM_NUM] = { L"���Ǵ��",L"����Ǵ��",L"�������ҹ���" };
	int iSelectIdx=m_pButton->Get_ButtonIdx();

	int iIDx = 0;
	int iWeaponIdx[ITEM_NUM] = { -1,-1,-1 };

	for (auto Item : (*m_InvenVec))
	{
		if (Item.first.find(L"���Ǵ��") != wstring::npos)
		{
			iWeaponIdx[0] = iIDx;
		}
		if (Item.first.find(L"����Ǵ��") != wstring::npos)
		{
			iWeaponIdx[1] = iIDx;
		}if (Item.first.find(L"�������ҹ���") != wstring::npos)
		{
			iWeaponIdx[2] = iIDx;
		}
		iIDx++;
	}
	if (ITEM_NUM > iSelectIdx)
	{
		return wstrItem[iSelectIdx];

	}
	else //��� �����۰������°� �̻���
	{
			if (iWeaponIdx[iSelectIdx- ITEM_NUM] >= 0)
			{
				switch (iSelectIdx - ITEM_NUM)
				{
				case 0:
					return (*m_InvenVec)[0].first;
					break;
				case 1:
					return (*m_InvenVec)[1].first;
					break;
				case 2:
					return (*m_InvenVec)[2].first;
					break;
				}
			}
		return L"";
	}

}

void CShop::ChangeEnable(_bool bIsEnable)
{
	m_bIsOn = bIsEnable;
	if (m_pButton != nullptr)
	{
		m_pButton->ChangeEnable(m_bIsOn);
		for(int i=0; i<3; i++)
			m_pIcon[i]->ChangeEnable(m_bIsOn);
	}
}

CShop* CShop::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, _float fLength, _float fRotY, _bool bIsLeft, UISTATE eUIState)
{
	CShop*	pInstance = new CShop(pGraphicDev, wstrTexName, fLength,fRotY,bIsLeft, eUIState);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CShop::Free(void)
{


	C3DUI::Free();
}

