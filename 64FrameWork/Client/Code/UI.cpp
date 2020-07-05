#include "stdafx.h"
#include "UI.h"

#include "Export_Function.h"

CUI::CUI(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, _vec3 vPos, PIVOT ePivot, _vec3 vUVASpeed)
	: Engine::CGameObject(pGraphicDev), m_vPos(vPos), m_ePivot(ePivot), m_wstrTexName(wstrTexName), m_vTexUVA(vUVASpeed)
{
}

CUI::~CUI(void)
{

}

HRESULT CUI::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	m_fConvertSizeX = m_fSizeX = m_pTextureCom->Get_ImageInfo().Width;
	m_fConvertSizeY = m_fSizeY = m_pTextureCom->Get_ImageInfo().Height;

	ConvertPivotPos();


	return S_OK;
}

HRESULT CUI::LateReady_GameObject(void)
{
	Set_TargetTransform();
	return S_OK;
}

_int CUI::Update_GameObject(const _float& fTimeDelta)
{

	m_vUBA.x += m_vTexUVA.x*fTimeDelta;
	m_vUBA.y += m_vTexUVA.y*fTimeDelta;
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	//// 流背捧康 青纺 积己
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return 0;
}

void CUI::Render_GameObject(void)
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

HRESULT CUI::Add_Component(void)
{

	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	//pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_InRcTex"));
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

	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_UI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

HRESULT CUI::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	_matrix			matWorld, matView, matProj;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);
	
	matWorld._11 = m_fConvertSizeX;
	matWorld._22 = m_fSizeY;
	matWorld._33 = 1.f;
	

	matWorld._41 = m_vConvertPos.x	- WINCX * 0.5f;
	matWorld._42 = -m_vConvertPos.y + WINCY * 0.5f;
	matWorld._43 = m_vConvertPos.z;

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &m_matProj);

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture", _uint(m_fFrameCnt));
	Engine::SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");
	pEffect->SetFloat("g_TextureU", m_vUBA.x);
	pEffect->SetFloat("g_TextureV", m_vUBA.y);
	pEffect->SetFloat("g_TextureA", m_vUBA.z);
	pEffect->SetFloat("g_TextureUScale", m_fUScale);

	return S_OK;
}

void CUI::Set_TargetTransform()
{
	m_pTargetTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Transform", Engine::ID_DYNAMIC));

}

void CUI::ConvertPivotPos()
{
	switch (m_ePivot)
	{
	case PIVOT_LT:
		m_vConvertPos = { m_vPos.x + m_fConvertSizeX*0.5f,m_vPos.y + m_fSizeY*0.5f,m_vPos.z };
		break;
	case PIVOT_RT:
		m_vConvertPos = { m_vPos.x - m_fConvertSizeX*0.5f,m_vPos.y + m_fSizeY*0.5f ,m_vPos.z };
		break;
	case PIVOT_M://1024 512-512 
		m_vConvertPos = { m_fSizeX*0.5f+(m_vPos.x - (m_fSizeX-m_fConvertSizeX*0.5f)),m_vPos.y,m_vPos.z };
		break;
	case PIVOT_RB:
		m_vConvertPos = {  m_vPos.x - m_fConvertSizeX*0.5f,m_vPos.y - m_fSizeY*0.5f ,m_vPos.z };
		break;
	case PIVOT_LB:
		m_vConvertPos = { m_vPos.x + m_fConvertSizeX*0.5f,m_vPos.y - m_fSizeY*0.5f ,m_vPos.z };
		break;
	case PIVOT_END:
		break;
	}

}



CUI* CUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, _vec3 vPos, PIVOT ePivot, _vec3 vUVASpeed)
{
	CUI*	pInstance = new CUI(pGraphicDev, wstrTexName, vPos, ePivot, vUVASpeed);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CUI::Free(void)
{
	Engine::CGameObject::Free();
}

