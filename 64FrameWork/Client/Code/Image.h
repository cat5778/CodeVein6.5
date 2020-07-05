#ifndef Image_h__
#define Image_h__

#include "Defines.h"
#include "GameObject.h"
#include "UI.h"

class CImage : public CUI
{
private:
	explicit CImage(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, _vec3 vPos, PIVOT ePivot, _float fFilter,_vec3 vUVASpeed, _float fAlphaSpeed);
	virtual ~CImage(void);

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual HRESULT	LateReady_GameObject(void)override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	Render_GameObject(void) override;

private:
	virtual HRESULT			Add_Component(void)override;
	virtual HRESULT			SetUp_ConstantTable(LPD3DXEFFECT& pEffect)override;


public:
	void			Set_MaxAlpha(_float fMaxAlpha);
	void			Set_GaugeRatio(_float fRatio);
	void			Blink_Image(_float fTimeDelta, _float fSpeed, _float fStartTime);
	void			LerpGague(_float fTimeDelata);

private:
	_float			m_fTime = 0.f;
	_float			m_fSin = 0.f;
	_float			m_fFilter = 0.0f;
	_float			m_fAlphaSpd = 0.f;
	_float			m_fAlphaMax = 1.f;
	_vec3			m_vTargetPos;
public:
	static CImage*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, _vec3 vPos, PIVOT ePivot, _float fFilter,_vec3 vUVASpeed = { 0.f,0.f,1.f },_float fAlphaSpeed=0.f);

private:
	virtual void Free(void) override;

};

#endif // Image_h__
