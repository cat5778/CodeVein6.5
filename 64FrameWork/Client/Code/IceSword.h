#ifndef IceSword_h__
#define IceSword_h__

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

class CIceSword : public Engine::CGameObject
{
private:
	explicit				CIceSword(LPDIRECT3DDEVICE9 pGraphicDev, _uint uiInstIdx);
	virtual					~CIceSword(void);

public:
	HRESULT					Ready_GameObject();
	virtual HRESULT			LateReady_GameObject(void);


	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			Render_GameObject(void) override;
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

private:
	HRESULT					Add_Component(void);

public:

	void					Set_EquipObject(wstring wstrObjName);
	_bool					Get_Equip() { return m_bIsEquip; }
	void					Set_Equip(_bool bIsEquip);
	void					Set_Throw();
	void					Set_Coll(_bool bIsColl) { m_bColl = bIsColl; }
private:
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CTransform*		m_pTargetTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CStaticMesh*	m_pMeshCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;
	
	_float					m_fScale = 0.f;
	_vec3					m_vThrow = { INIT_VEC3 };
	_float					m_fAccThrow = 4.f;
	_uint					m_iFlag = 0;
	_bool					m_bColl = false;
	_bool					m_bIsEquip = false;
	_vec3					m_vOldPos;
	_vec3					m_vOldRotation;
	_matrix					m_MatOldWorld;
	const	_matrix*		m_pParentBoneMatrix = nullptr;
	const	_matrix*		m_pParentWorldMatrix = nullptr;
	wstring					m_wstrEquipName;

	//Test
	_float					m_fAnlgeX = 0.f;
	_float					m_fAnlgeY = 0.f;
public:
	static CIceSword*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint uiInstIdx);

private:
	virtual void Free(void) override;
};

#endif // IceSword_h__
