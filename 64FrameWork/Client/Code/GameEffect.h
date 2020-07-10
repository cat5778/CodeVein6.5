#ifndef GameEffect_h__
#define GameEffect_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;
class CShader;

END

class CGameEffect : public Engine::CGameObject
{
protected:
	explicit CGameEffect(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, _vec2 vScale, _vec3 vPos, _bool bIsLoop);
	explicit CGameEffect(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, wstring wstrParentInstName, string strBoneName, _vec2 vScale, _vec3 vPos);
	explicit CGameEffect(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, wstring wstrParentInstName, string strBoneName, _vec2 vScale, _vec3 vPos, _bool bIsTracking, _bool bIsLoop);
	virtual ~CGameEffect(void);

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual HRESULT LateReady_GameObject(void);

	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

protected:
	virtual HRESULT		Add_Component(void);
	virtual HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
public:

	void		Set_Loop(_bool bIsLoop) { m_bIsLoop= bIsLoop;}
	void		Set_Scale(_vec2 vScale) { m_vMultiScale = vScale; }
	void		Set_Pos(_vec3 vPos) { m_vPos = vPos; }

	void		Test(_float fTimeDelta);


protected:
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
	_bool					m_bIsParent = false;
	_bool					m_bIsTracking = false;


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
	_vec2					m_vMultiScale = { 1.f,1.f };
	_bool					m_bIsLoop = false;
	wstring					m_wstrParantName;
	string					m_strBoneName;
	_vec3					m_vAddPos = { 0.f,0.f ,0.f };
	_bool					m_bVertical = false;
	_float					m_fVerticalPos = 0.f;
	_float					m_fVerticalTime = 0.f;

public:

	static CGameEffect*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName,  _vec2 vScale,_vec3 vPos, _bool bIsLoop);
	static CGameEffect*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, wstring wstrParentInstName, string strBoneName, _vec2 vScale = { 1.f,1.f }, _vec3 vPos = { 0.f,0.f,0.f });
	static CGameEffect*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, wstring wstrParentInstName, string strBoneName, _vec2 vScale = { 1.f,1.f }, _vec3 vPos = { 0.f,0.f,0.f }, _bool bIsTracking = false, _bool bIsLoop = false);



protected:
	virtual void Free(void) override;
};

#endif // BoostEffect_h__s
