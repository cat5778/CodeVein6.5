#include "stdafx.h"
#include "InvenSub.h"
#include "3DButton.h"
#include "Export_Function.h"
#include "ThirdPersonCamera.h"
#include "Player.h"
#include "Image.h"

CInvenSub::CInvenSub(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, _float fLength, _float fRotY, _bool bIsRight ,UISTATE eUIState)
	:C3DUI(pGraphicDev,wstrTexName,fLength,fRotY,bIsRight,eUIState)
{
	m_wstrInstName = L"InvenSub_UI";
}

CInvenSub::~CInvenSub(void)
{

}

HRESULT CInvenSub::Ready_GameObject(void)
{
	C3DUI::Ready_GameObject();
	m_fGap = 0.45f;

	return S_OK;
}

HRESULT CInvenSub::LateReady_GameObject(void)
{

	C3DUI::LateReady_GameObject();


	return S_OK;
}

_int CInvenSub::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsOn)
		return 0;
	m_fGap = 0.45f;
	if (Engine::Get_DIKeyState(DIK_LEFT) || Engine::Get_DIKeyState(DIK_RIGHT))
	{
		m_pTransformCom->Set_Scale(m_vScale.x, m_vScale.y, m_vScale.z);
	}
	else
	{
		if (Engine::Get_DIKeyState(DIK_UP) || Engine::Get_DIKeyState(DIK_DOWN))
			m_pTransformCom->Set_Scale(m_vScale.x*1.5f, m_vScale.y*1.5f, m_vScale.z*1.5f);
	}

	Engine::CGameObject::Update_GameObject(fTimeDelta);
	BillBoard();
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return 0;
}

void CInvenSub::Render_GameObject(void)
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

HRESULT CInvenSub::Add_Component(void)
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



HRESULT CInvenSub::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
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

wstring CInvenSub::Get_ItemName()
{
	wstring wstrItem;
	switch (m_pButton->Get_ButtonIdx())
	{
	case 0:
		wstrItem = L"Âø¿ë";
		break;
	} 

	return wstrItem;
}

CInvenSub* CInvenSub::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, _float fLength, _float fRotY, _bool bIsLeft, UISTATE eUIState)
{
	CInvenSub*	pInstance = new CInvenSub(pGraphicDev, wstrTexName, fLength,fRotY,bIsLeft, eUIState);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CInvenSub::Free(void)
{
	C3DUI::Free();
}

