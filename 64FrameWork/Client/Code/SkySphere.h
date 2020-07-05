#ifndef SkySphere_h__
#define SkySphere_h__

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

class CSkySphere : public Engine::CGameObject
{
private:
	explicit				CSkySphere(LPDIRECT3DDEVICE9 pGraphicDev, _uint uiStageIdx);
	virtual					~CSkySphere(void);

public:
	HRESULT					Ready_GameObject();
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			Render_GameObject(void) override;
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
	void					Ready_Stage();
private:
	HRESULT					Add_Component(void);

private:
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CTransform*		m_pTargetTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CStaticMesh*	m_pMeshCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;
	_float					m_fScale = 0.5f;//0.05
	_uint					m_uiStageIdx=0;
	_float					m_fRot=0.f;
	_float					m_fLength = 1.0f;
	wstring					m_wstrMeshName;


public:
	static CSkySphere*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint uiStageIdx);

private:
	virtual void Free(void) override;
};

#endif // SkySphere_h__
