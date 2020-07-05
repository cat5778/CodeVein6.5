#ifndef Gauge_h__
#define Gauge_h__

#include "Defines.h"
#include "GameObject.h"
#include "UI.h"

class CGauge : public CUI
{
private:
	explicit CGauge(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, _vec3 vPos, PIVOT ePivot, _vec3 vUVASpeed);
	virtual ~CGauge(void);

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual HRESULT	LateReady_GameObject(void);
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	Render_GameObject(void) override;

public:
	void			Set_GaugeRatio(_float fRatio);
	void			Blink_Gauge(_float fTimeDelta,_float fSpeed, _float fStartTime);
	void			LerpGague(_float fTimeDelata);

private:
	_float			m_fSin = 0.f;




public:
	static CGauge*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, _vec3 vPos, PIVOT ePivot, _vec3 vUVASpeed = { 0.f,0.f,1.f });

private:
	virtual void Free(void) override;

};

#endif // Gauge_h__
