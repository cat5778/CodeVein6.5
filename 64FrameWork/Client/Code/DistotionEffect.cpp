#include "stdafx.h"
#include "DistotionEffect.h"

#include "Export_Function.h"

CDistortionEffect::CDistortionEffect(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, wstring wstrAlphaTexName, _vec3 vPos)
	: Engine::CGameObject(pGraphicDev), m_vPos(vPos),m_wstrTexName(wstrTexName),m_wstrAlphaTexName(wstrAlphaTexName)
{
	m_wstrMaskTexName = L"FireMask2";
}

CDistortionEffect::~CDistortionEffect(void)
{

}

HRESULT CDistortionEffect::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_vScale.x = m_pTextureCom->Get_ImageInfo().Width*0.001f;
	m_vScale.y = m_pTextureCom->Get_ImageInfo().Height*0.001f;
	m_vScale.z = m_pTextureCom->Get_ImageInfo().Width*0.001f;
	m_pTransformCom->Set_Pos(&m_vPos);
	m_fScale[0] = m_vScale.x;
	m_fScale[1] = m_vScale.y;
	m_fScale[2] = m_vScale.z;
	m_fFrameMax = 31;
	for (int i = 0; i < 3; i++)
		m_fScollTime[i] = 0.f;

	//m_pTransformCom->Set_Scale(m_pTextureCom->Get_ImageInfo().Width*0.001f,
	//	m_pTextureCom->Get_ImageInfo().Height*0.001f,
	//	0.0000001f);

	return S_OK;
}

HRESULT CDistortionEffect::LateReady_GameObject(void)
{
	m_pTargetTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Transform", Engine::ID_DYNAMIC));
	_vec3 vTargetPos = *m_pTargetTransformCom->Get_Info(Engine::INFO_POS);
	m_pTransformCom->Set_Pos(vTargetPos.x, vTargetPos.y + 1.5f, vTargetPos.z);

	return S_OK;
}

_int CDistortionEffect::Update_GameObject(const _float& fTimeDelta)
{
	m_fFameTime += fTimeDelta;
	m_fScollTime[0]+= fTimeDelta;
	m_fFrameCnt += m_fFrameMax * fTimeDelta;

	if (m_fFrameCnt >= m_fFrameMax)
		m_fFrameCnt = 0.f;


	Engine::CGameObject::Update_GameObject(fTimeDelta);

	_matrix	matWorld, matView, matBill;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixIdentity(&matBill);

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);

	m_pTransformCom->Set_WorldMatrix(&(matBill * matWorld));

	Engine::CGameObject::Compute_ViewZ(&m_pTransformCom->m_vInfo[Engine::INFO_POS]);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_DISTORTION, this);

	return 0;
}

void CDistortionEffect::Render_GameObject(void)
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

HRESULT CDistortionEffect::Add_Component(void)
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

	pComponent = m_pAlphaTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, m_wstrAlphaTexName.c_str()));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_AlphaTexture", pComponent);

	pComponent = m_pMaskTextureCom = dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, m_wstrMaskTexName.c_str()));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_MaskTexture", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);
	
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Distortion"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);
	
	return S_OK;
}



HRESULT CDistortionEffect::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	//pEffect->SetFloatArray("scrollSpeeds", m_fScollTime, 3);
	//pEffect->SetFloatArray("scales", m_fScale, 3);e
	pEffect->SetFloat("g_frameTime", m_fFameTime);

	//pEffect->SetVector("scales", &m_vScale);
	//m_pTextureCom->Set_Texture(pEffect, "g_DepthAlphaTexture", _uint(m_fFrameCnt));
	//m_pAlphaTextureCom->Set_Texture(pEffect, "g_DepthAlphaTexture", 0);
	m_pTextureCom->Set_Texture(pEffect, "_pTex_Orig", _uint(m_fFrameCnt));
	m_pMaskTextureCom->Set_Texture(pEffect, "g_AlphaTexture", _uint(m_fFrameCnt));
	m_pAlphaTextureCom->Set_Texture(pEffect, "_pTex_Distortion", 0);


	
	Engine::SetUp_OnShader(pEffect, L"Target_Distortion", "_pTex_Orig");
	//Engine::SetUp_OnShader(pEffect, L"Target_Depth", "_pTex_Orig");


	return S_OK;
}

CDistortionEffect* CDistortionEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, wstring wstrAlphaTexName, _vec3 vPos)
{
	CDistortionEffect*	pInstance = new CDistortionEffect(pGraphicDev, wstrTexName, wstrAlphaTexName, vPos);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CDistortionEffect::Free(void)
{


	Engine::CGameObject::Free();
}

