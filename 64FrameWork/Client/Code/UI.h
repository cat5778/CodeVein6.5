#ifndef UI_h__
#define UI_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;

END

class CUI : public Engine::CGameObject
{
protected:
	explicit CUI(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, _vec3 vPos, PIVOT ePivot, _vec3 vUVASpeed);
	virtual ~CUI(void);

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual HRESULT	LateReady_GameObject(void);
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual void	Render_GameObject(void) override;

protected:
	virtual HRESULT		Add_Component(void);
	virtual HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
	virtual void		Set_TargetTransform();
	virtual void		ConvertPivotPos();

protected:
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CTransform*		m_pTargetTransformCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;

	_matrix					m_matProj;
	_matrix					m_matOld;

	wstring					m_wstrTexName = L"";
	_vec3					m_vConvertPos = { INIT_VEC3 };
	_vec3					m_vPos = { INIT_VEC3 };
	_float					m_fSizeX, m_fSizeY;
	_float					m_fConvertSizeX, m_fConvertSizeY;

	_float					m_fUScale = 1.f;
	_float					m_fOldUScale = 1.f;
	_float					m_fLerpTime = 0.f;


	_vec3					m_vTexUVA = { 0.f,0.f,1.0f };
	_vec3					m_vUBA = {0.f,0.f,1.0f};
	_float					m_fFrameCnt = 0;
	_float					m_fFrameMax = 90.f;
	PIVOT					m_ePivot = PIVOT_M;
public:
	static CUI*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, _vec3 vPos, PIVOT ePivot, _vec3 vUVASpeed = { 0.f,0.f,1.f });

protected:
	virtual void Free(void) override;
};

#endif // UI_h__
