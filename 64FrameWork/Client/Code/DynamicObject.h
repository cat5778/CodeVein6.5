#pragma once


#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CStaticMesh;
class CDynamicMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
class CColliderGroup;
END


class CDynamicObject : public Engine::CGameObject
{
public:
	explicit				CDynamicObject(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx, _uint uiStageIdx=0);
	explicit				CDynamicObject(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx, TRANSFORM_INFO tInfo, _uint uiStageIdx=0);
	virtual					~CDynamicObject(void);

public:
	HRESULT					Ready_GameObject();
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			Render_GameObject(void) override;

protected:
	virtual HRESULT			Add_Component(void);
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
	virtual void			StateMachine();

public:
	_uint					Get_AnimationIdx() { return m_uiAni; }
	void					Set_AnimationIdx(_uint uiAnim) { m_uiAni= uiAnim; }
	_vec3					Get_TargetPos();
	_vec3					Get_Pos();
	_vec3					Get_Look();
	_vec3					Get_Right();
	float					Get_Angle(const D3DXVECTOR3& a, const D3DXVECTOR3& b);
	_float					Get_AniRatio();
	_float					Get_AngleOnTheTarget();
	_float					Get_TargetDist();

	void					HurtMon(_float fDamage,_bool bIsStrongAtk);
	

	void					MoveAni(_float fTimeDelta, _float fMinRatio, _float fMaxRatio, _float fSpeed, _vec3 vDir, _bool bIsJump=false);
	void					RotateAni(_float fTimeDelta, _float fMinRatio, _float fMaxRatio, _float fRotSpeed);
	void					RotateToTarget(_float fTimeDelta, _float fMinRatio, _float fMaxRatio=0.f);
	void					SetColliderEnable(_float fMin, _float fMax);

	_bool					CheckDeath() { return m_bIsMonDead; }


protected:
	HRESULT					Load_Text(const TCHAR * pFilePath);
	wchar_t*				CharToWChar(const char* pstrSrc);

protected:
	virtual void			Set_TransformData();

protected:
	_uint					m_uiStageIdx = 0;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CTransform*		m_pTargetTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CDynamicMesh*	m_pMeshCom = nullptr;
	Engine::CColliderGroup* m_pColliderGroupCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;
	Engine::CNaviMesh*		m_pNaviCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;
	TRANSFORM_INFO			m_tTransformInfo;
	_bool					m_bIsRespawn = false;
	_uint					m_uiAni = 0;
	_bool					m_bIsHurt = false;
	_bool					m_bisStrongHurt = false;
	_vec3					m_vHurtPos = { INIT_VEC3 };
	_bool					m_bIsMonDead = false;

protected:
	_uint					m_uiPattern = 0;
	_float					m_fDistance = 0.f;
	_float					m_fDetectRange = 0.f;
	_float					m_fAttackRange = 0.f;
	_float					m_fSpeed = 1.0f;
	_float					m_fAnimSpeed = 1.0f;
	_float					m_fRotSpeed = 1.0f;
	_float					m_fDamage = 10.f;
	_float					m_fCurHp = 100.f;
	_float					m_fMaxHp = 100.f;
	_vec3					m_vRespawn = { INIT_VEC3 };
	_uint					m_uiCellIdx = 0;

public:
	static CDynamicObject*	Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx = 0, _uint uiStageIdx=0);
	static CDynamicObject*	Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx ,  TRANSFORM_INFO tInfo, _uint uiStageIdx=0);

protected:
	virtual void			Free(void) override;
};

