#ifndef LockOn_h__
#define LockOn_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;
class CShader;

END

class CLockOn : public Engine::CGameObject
{
private:
	explicit CLockOn(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName);
	virtual ~CLockOn(void);

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual HRESULT	LateReady_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;
public:
	void		Set_TargetPos(Engine::CTransform* pTargetTrasnform);
private:

	HRESULT		Add_Component(void);
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
	void		Blink_Image(_float fTimeDelta);
	void		Set_Scale();

private:
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CTransform*		m_pTargetTransformCom = nullptr;
	Engine::CTransform*		m_pPlayerTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;

	wstring					m_wstrTexName = L"";
	_vec3					m_vScale;
	_float					m_fFrameCnt = 0;
	_float					m_fFrameMax = 90.f;
	_float					m_fAlpha = 1.f;
	_float					m_fSin = 0.f;

public:
	static CLockOn*		Create(LPDIRECT3DDEVICE9 pGraphicDev,  wstring wstrTexName);

private:
	virtual void Free(void) override;
};

#endif // LockOn_h__
