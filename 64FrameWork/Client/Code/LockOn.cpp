#include "stdafx.h"
#include "LockOn.h"

#include "Export_Function.h"

CLockOn::CLockOn(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName)
	: Engine::CGameObject(pGraphicDev),m_wstrTexName(wstrTexName)
{

}

CLockOn::~CLockOn(void)
{

}

HRESULT CLockOn::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	//m_vScale.x = m_pTextureCom->Get_ImageInfo().Width*0.001f;
	//m_vScale.y = m_pTextureCom->Get_ImageInfo().Height*0.001f;
	//m_vScale.z = 0.0000001f;


	//m_pTransformCom->Set_Scale(m_vScale.x, m_vScale.y, m_vScale.z);
	//m_pTransformCom->Set_Pos(0.f, 0.f, 0.f);
	


	return S_OK;
}

HRESULT CLockOn::LateReady_GameObject(void)
{
	m_pPlayerTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Transform", Engine::ID_DYNAMIC));

	return S_OK;
}

_int CLockOn::Update_GameObject(const _float& fTimeDelta)
{
	_vec3 vTargetPos = { 0.f,100.f,0.f };
	_vec3 vPos= { 0.f,100.f,0.f };

	if (m_pTargetTransformCom != nullptr)
	{
		vTargetPos = *m_pTargetTransformCom->Get_Info(Engine::INFO_POS);
		_vec3 vPlayerPos = *m_pPlayerTransformCom->Get_Info(Engine::INFO_POS);
		vPos = (vTargetPos - vPlayerPos)* 0.6f;
		vPos = vPlayerPos + vPos;
	
	}
	Blink_Image(fTimeDelta);
	m_pTransformCom->Set_Pos(vPos.x, vPos.y+1.5f, vPos.z);
	Engine::CGameObject::Update_GameObject(fTimeDelta);


	_matrix	matWorld, matView, matBill;

	Set_Scale();
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

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return 0;
}

void CLockOn::Render_GameObject(void)
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

HRESULT CLockOn::Add_Component(void)
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
	
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_LockOn"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);
	
	return S_OK;
}



HRESULT CLockOn::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
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
	pEffect->SetFloat("g_TextureA", m_fAlpha);

	return S_OK;
}

void CLockOn::Blink_Image(_float fTimeDelta)
{
	m_fSin += fTimeDelta*200;
	if (m_fSin > 180.f)
		m_fSin = 0.f;
	m_fAlpha=0.5f+(sinf(D3DXToRadian(m_fSin))*0.5f);

}

void CLockOn::Set_Scale()
{
	m_pTransformCom->m_matWorld._11 *= m_pTextureCom->Get_ImageInfo().Width*0.015;
	m_pTransformCom->m_matWorld._22 *= m_pTextureCom->Get_ImageInfo().Height*0.015f;
	m_pTransformCom->m_matWorld._33 *= m_pTextureCom->Get_ImageInfo().Width*0.015f;


}

void CLockOn::Set_TargetPos(Engine::CTransform * pTargetTrasnform)
{
	m_pTargetTransformCom = pTargetTrasnform;
}

CLockOn* CLockOn::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName)
{
	CLockOn*	pInstance = new CLockOn(pGraphicDev, wstrTexName);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CLockOn::Free(void)
{


	Engine::CGameObject::Free();
}

