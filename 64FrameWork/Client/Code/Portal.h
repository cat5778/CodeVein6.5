#ifndef Portal_h__
#define Portal_h__

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
//class CPortalInfo;
class CPortal : public C3DUI
{
private:
	explicit CPortal(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, _float fLength, _float fRotY, _bool bIsRight, UISTATE eUIState);
	virtual ~CPortal(void);

public:
	virtual HRESULT		Ready_GameObject(void) override;
	virtual HRESULT		LateReady_GameObject(void);
	virtual _int		Update_GameObject(const _float& fTimeDelta) override;
	
	virtual void		Render_GameObject(void) override;

public:
	virtual void		ChangeEnable(_bool bIsEnable)override;
	virtual void		ChangeEnable()override;

public:
	_uint				Get_PortalIdx();
private:
	virtual HRESULT		Add_Component(void);
	virtual HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

private:
	C3DIcon*						m_pIcon = nullptr;
	//CPortalInfo*					m_pInfoUI = nullptr;
	_uint							m_uiWaypointIdx = 0;
	//TEst
	_vec3							m_vTempTest = { INIT_VEC3 };

public:
	static CPortal*		Create(LPDIRECT3DDEVICE9 pGraphicDev,  wstring wstrTexName, _float fLength,_float fRotY,_bool bIsRight =true, UISTATE eUIState=UI_END);

private:
	virtual void Free(void) override;
};

#endif // Portal_h__
