#pragma once
#include "Defines.h"
#include "GameObject.h"
#include "DynamicObject.h"
BEGIN(Engine)
class CNaviMesh;
END
class CCocoonDevil : public CDynamicObject
{
public:
	explicit				CCocoonDevil(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx,_uint uiStageIdx=1);
	explicit				CCocoonDevil(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx, TRANSFORM_INFO tInfo, _uint uiStageIdx=1);
	explicit				CCocoonDevil(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx, _vec3 vPos, _uint uiCellIdx = 0);
	virtual					~CCocoonDevil(void);

public:
	HRESULT					Ready_GameObject();
	virtual	HRESULT			LateReady_GameObject()override;
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			Render_GameObject(void) override;
	


private:
	virtual HRESULT			Add_Component(void);
	void					Set_TransformData();
	void					StateMachine()override;

private://Pattern
	void					Pattern(_float fTimeDelta);
	void					KnockBack(_float fTimeDelta);
	void					Chaing_Target(_float fTimeDelta);

	void					Shoot(_float fTimeDelta);

	void					Mist(_float fTimeDelta);

	void					Idle(_float fTimeDelta);
	void					Collision_Check(_float fTimeDelta);
	void					Hurt(_float fTimeDelta);
	void					Death(_float fTimeDelta);

public:
	static CCocoonDevil*	Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx = 0, _uint uiInstIdx = 0);
	static CCocoonDevil*	Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx ,  TRANSFORM_INFO tInfo, _uint uiStageIdx = 0);
	static CCocoonDevil*	Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx, _vec3 vPos, _uint uiCellIdx = 0);



private:

	_bool						m_bIsStart = false;
	_float						m_fJump=0.f;
	COCCONDEVILSTATE			m_eCurState;
	COCCONDEVILSTATE			m_ePreState;
	DWORD						m_dwHurtDirectionFlag = 0;
	const	_matrix*			m_pParentBoneMatrix = nullptr;
	_bool						m_bIsShoot = false;
	_vec3						m_vShootPos = { INIT_VEC3 };
	_uint						m_uiShootCount = 0;
private:
	virtual void			Free(void) override;
};


/*CoCoonDevil
44. Damage_B
45. Damage_F
46. Death
47. Idle
48. Attack_Shoot_N
49. Attack_Mist_N


*/
