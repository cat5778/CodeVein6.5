#ifndef BoostEffect_h__
#define BoostEffect_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;
class CShader;

END

class CBoostEffect : public Engine::CGameObject
{
private:
	explicit CBoostEffect(LPDIRECT3DDEVICE9 pGraphicDev,wstring wstrTexName,  wstring wstrAlphaTexName, _bool bRight);
	virtual ~CBoostEffect(void);

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual HRESULT LateReady_GameObject(void);

	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

private:
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CTexture*		m_pAlphaTextureCom = nullptr;
	Engine::CTexture*		m_pMaskTextureCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CTransform*		m_pTargetTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;

	const	_matrix*		m_pParentBoneMatrix = nullptr;
	const	_matrix*		m_pParentWorldMatrix = nullptr;

	_bool					m_bIsRight = false;
	_float					m_fFrameCnt = 0;
	_float					m_fFrameMax = 0.f;
	wstring					m_wstrTexName;
	wstring					m_wstrAlphaTexName;
	wstring					m_wstrMaskTexName;
	_vec3					m_vPos;
	_vec3					m_vScale;
	_float					m_fScollTime[3];
	_float					m_fScale[3];
	_float					m_fFameTime=1.f;
	_matrix					m_OldMatrix;
public:

	static CBoostEffect*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, wstring wstrAlphaTexName, _bool bRight = true);

private:
	virtual void Free(void) override;
};

#endif // BoostEffect_h__s
