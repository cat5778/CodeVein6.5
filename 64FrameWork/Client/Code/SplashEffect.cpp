#include "stdafx.h"
#include "SplashEffect.h"

#include "Export_Function.h"


CSplashEffect::CSplashEffect(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName,  _vec2 vScale,_vec3 vPos, _bool bIsLoop)
	: CGameEffect(pGraphicDev,wstrTexName,vScale,vPos,bIsLoop)
{


}

CSplashEffect::CSplashEffect(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, wstring wstrParentInstName, string strBoneName, _vec2 vScale, _vec3 vPos)
	: CGameEffect(pGraphicDev, wstrTexName, wstrParentInstName, strBoneName, vScale, vPos)
{

	m_fVerticalTime = 60.f;
}

CSplashEffect::CSplashEffect(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, wstring wstrParentInstName, string strBoneName, _vec2 vScale,_vec3 vPos, _bool bIsTracking, _bool bIsLoop)
	: CGameEffect(pGraphicDev, wstrTexName, wstrParentInstName, strBoneName, vScale, vPos, bIsTracking, bIsLoop)
{

}

CSplashEffect::~CSplashEffect(void)
{

}

HRESULT CSplashEffect::Ready_GameObject(void)
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	m_vScale.x = m_pTextureCom->Get_ImageInfo().Width*0.01f*m_vMultiScale.x;
	m_vScale.y = m_pTextureCom->Get_ImageInfo().Height*0.01f*m_vMultiScale.y;
	m_vScale.z = m_pTextureCom->Get_ImageInfo().Width*0.01f*m_vMultiScale.x;
	m_pTransformCom->Set_Scale(m_vScale.x, m_vScale.y, m_vScale.z);
	m_fFrameMax=m_pTextureCom->Get_ImageCnt();





	return S_OK;
}

HRESULT CSplashEffect::LateReady_GameObject(void)

{
	//m_pTargetTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", L"RussianHat_0", L"Com_Transform", Engine::ID_DYNAMIC));
	//_vec3 vTargetPos = *m_pTargetTransformCom->Get_Info(Engine::INFO_POS);

	//m_pTransformCom->Get_WorldMatrix(&mat);




	return S_OK;
}

_int CSplashEffect::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bIsParent&&nullptr == m_pParentBoneMatrix)
	{
		Engine::CDynamicMesh*	pMonsterMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Get_Component(L"GameLogic", m_wstrParantName.c_str(), L"Com_Mesh", Engine::ID_STATIC));
		NULL_CHECK_RETURN(pMonsterMeshCom, 0);

		const Engine::D3DXFRAME_DERIVED* pBone = pMonsterMeshCom->Get_FrameByName(m_strBoneName.c_str());
		NULL_CHECK_RETURN(pBone, 0);

		m_pParentBoneMatrix = &pBone->CombinedTransformationMatrix;
		
		Engine::CTransform*	pObjectTransform = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", m_wstrParantName.c_str(), L"Com_Transform", Engine::ID_DYNAMIC));
		NULL_CHECK_RETURN(pObjectTransform, 0);

		m_pParentWorldMatrix = pObjectTransform->Get_WorldMatrixPointer();

		m_OldMatrix = (*m_pParentBoneMatrix * *m_pParentWorldMatrix);
		memcpy(m_vPos, &m_OldMatrix._41, sizeof(_vec3));
	}
	if (m_bIsParent)
	{
		m_pTransformCom->Set_ParentMatrix(&(*m_pParentBoneMatrix * *m_pParentWorldMatrix));
		m_pTransformCom->Get_WorldMatrix(&m_OldMatrix);
		memcpy(m_vPos, &m_OldMatrix._41, sizeof(_vec3));
	}

	Splash_Scale(fTimeDelta, 1.f);
	

	m_vOldScale = { m_vScale.x * m_vMultiScale.x,
					m_vScale.y*m_vMultiScale.y,
					m_vScale.z*m_vMultiScale.x };



	m_pTransformCom->Set_Scale(m_vOldScale.x, m_vOldScale.y, m_vOldScale.z);
	if(m_bVertical)
		VerticalMove(fTimeDelta);


	Test(fTimeDelta);


	if (!m_bVertical)
		m_pTransformCom->Set_Pos(m_vPos.x + m_vAddPos.x, m_vPos.y + m_vAddPos.y + m_vOldScale.y*0.5f , m_vPos.z + m_vAddPos.z);
	else
		m_pTransformCom->Set_Pos(m_vPos.x + m_vAddPos.x, m_vPos.y + m_vAddPos.y + m_fVerticalPos, m_vPos.z + m_vAddPos.z);




	m_fScollTime[0]+= fTimeDelta;
	m_fFrameCnt += m_fFrameMax * fTimeDelta;

	if (m_fFrameCnt >= m_fFrameMax)
	{
		if(m_bIsLoop)
			m_fFrameCnt = 0.f;
		else
		{
			m_bEnable = false;
			m_fFrameCnt = m_fFrameMax;
		}
	}

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
	if(!m_bIsDistortion)
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_ALPHA, this);
	else
		m_pRendererCom->Add_RenderGroup(Engine::RENDER_DISTORTION, this);

	return 0;
}

void CSplashEffect::Render_GameObject(void)
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

HRESULT CSplashEffect::Add_Component(void)
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
	
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Effect"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);
	
	return S_OK;
}



HRESULT CSplashEffect::SetUp_ConstantTable(LPD3DXEFFECT& pEffect)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);
	if(!m_bVertical)
		pEffect->SetFloat("g_fAlphaRatio", 1.f - (m_fSplashScale*0.5f));
	else
		pEffect->SetFloat("g_fAlphaRatio", 1.f - (m_fSplashScale));

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture", _uint(m_fFrameCnt));
	
	Engine::SetUp_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");


	return S_OK;
}

void CSplashEffect::Splash_Scale(_float fTimeDelta, _float fMaxScale)
{
	if (fMaxScale >= m_fSplashScale)
	{
		if(m_bVertical)
			m_fSplashScale += fTimeDelta*fMaxScale*1.f;
		else
			m_fSplashScale += fTimeDelta*fMaxScale*2.f;
		m_vMultiScale.x = m_fSplashScale;
		m_vMultiScale.y = m_fSplashScale;
	}
	else
	{
		Set_Enable(false);
	}


/*
	if (180.f >= m_fSplashScale)
	{
		m_fSplashScale += fTimeDelta*400.f;
		m_vMultiScale.x = sinf(D3DXToRadian(m_fSplashScale))*fMaxScale;
		m_vMultiScale.y = sinf(D3DXToRadian(m_fSplashScale))*fMaxScale;
	}
	else
	{
		m_fSplashScale = 1.f;
	}
*/


}

void CSplashEffect::Set_Distortion()
{
	m_bIsDistortion = true;
}

void CSplashEffect::VerticalMove(_float fTimeDelta)
{

	//if (2.f >= m_fSplashScale)
	//{
	//	m_fSplashScale += fTimeDelta*2.f;
	//	m_vMultiScale.x = m_fSplashScale;
	//	m_vMultiScale.y = m_fSplashScale;
	//	cout << m_fSplashScale << endl;
	//}
	//else
	//{
	//	m_fSplashScale = 0.1f;
	//}

	//if (m_fSplashScale >= 0.5f)
	//{
	//}
		if (180.f >= m_fVerticalTime)
		{
			m_fVerticalTime += fTimeDelta*180.f;

			m_fVerticalPos = sinf(D3DXToRadian(m_fVerticalTime))*1.f;
		}
		else
		{
			m_fVerticalTime = 0.f;

		}
}

void CSplashEffect::Set_Enable(bool bEnable)
{
	m_bEnable = bEnable;
	if (bEnable)
	{
		m_fVerticalTime = 0.f;
		m_fSplashScale = 0.f;
	}
	else
	{
	
	}

}


CSplashEffect * CSplashEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, _vec2 vScale, _vec3 vPos, _bool bIsLoop)
{
	CSplashEffect*	pInstance = new CSplashEffect(pGraphicDev, wstrTexName, vScale, vPos, bIsLoop);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CSplashEffect * CSplashEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, wstring wstrParentInstName, string strBoneName, _vec2 vScale, _vec3 vPos)
{
	CSplashEffect*	pInstance = new CSplashEffect(pGraphicDev, wstrTexName, wstrParentInstName, strBoneName, vScale, vPos);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CSplashEffect * CSplashEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, wstring wstrParentInstName, string strBoneName, _vec2 vScale, _vec3 vPos, _bool bIsTracking, _bool bIsLoop)
{
	CSplashEffect*	pInstance = new CSplashEffect(pGraphicDev, wstrTexName, wstrParentInstName, strBoneName,  vScale, vPos, bIsTracking, bIsLoop);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CSplashEffect::Free(void)
{


	Engine::CGameObject::Free();
}

