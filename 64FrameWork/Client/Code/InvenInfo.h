#ifndef InvenInfo_h__
#define InvenInfo_h__

#include "Defines.h"
#include "GameObject.h"
#include "3DUI.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRenderer;
class CShader;

END
class CThirdPersonCamera;
class C3DButton;
class C3DIcon; 
class CDisplayWeapon;
class CInven;
class CInvenInfo : public C3DUI
{
private:
	explicit CInvenInfo(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, _float fLength);
	virtual ~CInvenInfo(void);

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual HRESULT	LateReady_GameObject(void);
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	
	virtual void	Render_GameObject(void) override;

public:
	virtual void		ChangeEnable(_bool bIsEnable)override;
	virtual void		ChangeEnable()override;


private:
	virtual HRESULT		Add_Component(void);
	virtual HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

	//TEst
private:
	CInven*							m_pInven = nullptr;
	_vec3							m_vTempTest = { INIT_VEC3 };
	CDisplayWeapon*					m_pWeapon[3] = { nullptr,nullptr ,nullptr };
public:
	static CInvenInfo*		Create(LPDIRECT3DDEVICE9 pGraphicDev,  wstring wstrTexName, _float fLength);

private:
	virtual void Free(void) override;
};

#endif // InvenInfo_h__
