#include "stdafx.h"
#include "Gauge.h"

#include "Export_Function.h"

CGauge::CGauge(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, _vec3 vPos, PIVOT ePivot, _vec3 vUVASpeed)
	:CUI(pGraphicDev,wstrTexName,vPos,ePivot, vUVASpeed)
{

}

CGauge::~CGauge(void)
{

}

HRESULT CGauge::Ready_GameObject(void)
{
	CUI::Ready_GameObject();

	return S_OK;
}

HRESULT CGauge::LateReady_GameObject(void)
{
	m_pTargetTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Transform", Engine::ID_DYNAMIC));

	return S_OK;
}

_int CGauge::Update_GameObject(const _float& fTimeDelta)
{
	LerpGague(fTimeDelta);

	Blink_Gauge(fTimeDelta, 300.f, 0.35f);
	
	CUI::Update_GameObject(fTimeDelta);

	return 0;
}

void CGauge::Render_GameObject(void)
{

	CUI::Render_GameObject();


}

void CGauge::Set_GaugeRatio(_float fRatio)
{
	m_fOldUScale = fRatio;
}

void CGauge::Blink_Gauge(_float fTimeDelta,_float fSpeed, _float fStartTime)
{
	if (m_fUScale <= fStartTime)
	{
		m_fSin += fTimeDelta*fSpeed;
		if (m_fSin > 180.f)
			m_fSin = 0.f;
		m_vUBA.z = 1.f - (sinf(D3DXToRadian(m_fSin))*0.5f);
	}
	else
	{
		m_fSin = 0.f;
		m_vUBA.z = 1.0f;
	}

}

void CGauge::LerpGague(_float fTimeDelata)
{
	if (m_fLerpTime < 1.0f)
	{
		m_fLerpTime += fTimeDelata*0.1f;
		_float fLerp = (1 - m_fLerpTime)*m_fUScale+ m_fLerpTime*m_fOldUScale;
		m_fUScale = fLerp;
		m_fConvertSizeX = m_fSizeX *m_fUScale;
		ConvertPivotPos();

	}
	if (m_fLerpTime > 1.0f)
		m_fLerpTime = 0.f;

}


CGauge* CGauge::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, _vec3 vPos, PIVOT ePivot, _vec3 vUVASpeed)
{
	CGauge*	pInstance = new CGauge(pGraphicDev, wstrTexName, vPos, ePivot, vUVASpeed);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CGauge::Free(void)
{
	CUI::Free();
}

