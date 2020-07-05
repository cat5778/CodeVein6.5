#ifndef Projectile_h__
#define Projectile_h__

#include "Defines.h"
#include "GameObject.h"


BEGIN(Engine)
class CStaticMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
class CShader;
END

class CProjectile : public Engine::CGameObject
{
public:
	explicit				CProjectile(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	virtual					~CProjectile(void);

public:
	HRESULT					Ready_GameObject();
	void					Ready_SphereMesh();
	virtual	HRESULT			LateReady_GameObject()override;
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			Render_GameObject(void) override;
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
	_bool					Is_Coll();
private:
	HRESULT					Add_Component(void);

private:
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CTransform*		m_pTargetTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CStaticMesh*	m_pMeshCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;
	LPD3DXMESH				m_pSphereMesh = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;


	_vec3					m_vShootPos = {INIT_VEC3};
	_vec3					m_vDir = { INIT_VEC3 };
	_bool					m_bColl = false;
	_vec3					m_vOldPos;
	_vec3					m_vOldRotation;
	_matrix					m_MatOldWorld;
	const	_matrix*		m_pParentBoneMatrix = nullptr;
	const	_matrix*		m_pParentWorldMatrix = nullptr;
	_bool					m_bIsColl = false;
	_float					m_fSpeed = 3.5f;

#ifdef _DEBUG
	_matrix					m_matSphereWorld;		//
#endif // _DEBUG


public:
	static CProjectile*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);

private:
	virtual void Free(void) override;




};


#endif // Halberd_h__
