#ifndef DisplayWapon_h__
#define DisplayWapon_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
class CShader;
class CTexture;

END

class CDisplayWeapon : public Engine::CGameObject
{
private:
	explicit				CDisplayWeapon(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrMeshName);
	virtual					~CDisplayWeapon(void);

public:

public:
	HRESULT					Ready_GameObject(const _uint& iFlag);
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			Render_GameObject(void) override;
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
	
public:
	virtual void			Set_Enable(bool bEnable)override;
private:
	HRESULT					Add_Component(void);
	_bool					Collision_ToObject(const _tchar* pLayerTag, const _tchar* pObjTag);



private:
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CTexture*		m_pNoiseTextureCom = nullptr;

	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CStaticMesh*	m_pMeshCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;
	wstring					m_wstrMeshName;
	_float					m_fDissolveTime = 0.f;
	_uint					m_iFlag = 0;
	_bool					m_bColl = false;
	_uint					m_uiPass = 0;
	const	_matrix*		m_pParentBoneMatrix = nullptr;
	const	_matrix*		m_pParentWorldMatrix = nullptr;

public:
	static CDisplayWeapon*		Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrMeshName, const _uint& iFlag);

private:
	virtual void Free(void) override;
};

#endif // DisplayWapon_h__
