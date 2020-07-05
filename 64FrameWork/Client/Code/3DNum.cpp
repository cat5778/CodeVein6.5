#include "stdafx.h"
#include "3DNum.h"

#include "Export_Function.h"
#include "ThirdPersonCamera.h"

C3DNumBer::C3DNumBer(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, Engine::CTransform* pParentTransfrom)
	: Engine::CGameObject(pGraphicDev),m_wstrTexName(wstrTexName), m_pTargetTransformCom(pParentTransfrom)
{

}

C3DNumBer::~C3DNumBer(void)
{

}

HRESULT C3DNumBer::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_vScale.x = m_pTextureCom->Get_ImageInfo().Width	*0.01f;
	m_vScale.y = m_pTextureCom->Get_ImageInfo().Height	*0.01f;
	m_vScale.z = m_pTextureCom->Get_ImageInfo().Width	*0.01f;

	m_pTransformCom->Set_Scale(m_vScale.x, m_vScale.y, m_vScale.z);
	m_vPos = { 0.4f,-0.4f ,-0.008f };
	return S_OK;
}

HRESULT C3DNumBer::LateReady_GameObject(void)
{
	return S_OK;
}

_int C3DNumBer::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsOn)
		return 0;

	m_pTransformCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);

	Engine::CGameObject::Update_GameObject(fTimeDelta);	
	m_pTargetTransformCom->Get_WorldMatrix(&m_matTargetWorld);
	m_pTransformCom->Set_ParentMatrix(&m_matTargetWorld);

	m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);

	return 0;
}

void C3DNumBer::Render_GameObject(void)
{

	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_uint	iPassMax = 0;

	SetUp_ConstantTable(pEffect);

	pEffect->Begin(&iPassMax, 0);

	pEffect->BeginPass(1);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();

	pEffect->End();

	Safe_Release(pEffect);
}

HRESULT C3DNumBer::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<Engine::CRcTex*>(Engine::Clone(RESOURCE_STATIC, L"Buffer_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);
	
	pComponent = m_pTextureCom= dynamic_cast<Engine::CTexture*>(Engine::Clone(RESOURCE_STAGE, L"Number"));
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



HRESULT C3DNumBer::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);


	//matWorld *= m_matTargetWorld;
	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture", _uint(m_fFrameCnt));
	
	
	Engine::SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");
	pEffect->SetFloat("g_TextureA", m_fAlpha);


	return S_OK;
}

void C3DNumBer::ChangeEnable(_bool bIsOn)
{
	m_bIsOn = bIsOn;
}

void C3DNumBer::Blink_Image(_float fTimeDelta, _float fSpeed)
{
	m_fSin += fTimeDelta*fSpeed;

	if (m_fSin > 180.f)
		m_fSin = 0.f;
	m_fAlpha = 1.f - (sinf(D3DXToRadian(m_fSin))*0.5f);
}

void C3DNumBer::Set_ButtonPos(_vec3 vPos)
{
	m_vPos = vPos;
}

void C3DNumBer::Set_Number(_uint uiNumber)
{
	m_fFrameCnt = (_float)uiNumber;
}



void C3DNumBer::TestPos()
{
	if (CKeyMgr::GetInstance()->KeyPressing(KEY_LEFT))
	{
		m_vPos.x -= 0.1f;
		cout << "X=" << m_vPos.x << endl;
	}
	if (CKeyMgr::GetInstance()->KeyPressing(KEY_RIGHT))
	{
		m_vPos.x += 0.1f;
		cout << "X=" << m_vPos.x << endl;

	}
	if (CKeyMgr::GetInstance()->KeyPressing(KEY_UP))
	{
		m_vPos.y += 0.1f;
		cout << "Y=" << m_vPos.y << endl;

	}
	if (CKeyMgr::GetInstance()->KeyPressing(KEY_DOWN))
	{
		m_vPos.y -= 0.1f;
		cout << "Y=" << m_vPos.y << endl;
	}
	if (CKeyMgr::GetInstance()->KeyPressing(KEY_I))
	{
		m_vPos.z += 0.1f;
		cout <<"Z=" <<m_vPos.z << endl;
	}
	if (CKeyMgr::GetInstance()->KeyPressing(KEY_O))
	{
		m_vPos.z -= 0.1f;
		cout << "Z=" << m_vPos.z << endl;
	}

	m_pTransformCom->Set_Pos(m_vPos.x, m_vPos.y, m_vPos.z);
}

_uint C3DNumBer::Get_ButtonIdx()
{
	return m_uiButtonIdx;
}


C3DNumBer* C3DNumBer::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, Engine::CTransform* pParentTransfrom)
{
	C3DNumBer*	pInstance = new C3DNumBer(pGraphicDev, wstrTexName, pParentTransfrom);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void C3DNumBer::Free(void)
{


	Engine::CGameObject::Free();
}

