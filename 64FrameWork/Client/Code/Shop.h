#ifndef Shop_h__
#define Shop_h__

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
class CShop : public C3DUI
{
private:
	explicit CShop(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, _float fLength, _float fRotY, _bool bIsRight, UISTATE eUIState);
	virtual ~CShop(void);

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual HRESULT	LateReady_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	
	virtual void Render_GameObject(void) override;

public:
	virtual wstring		Get_ItemName()override;
	virtual void		ChangeEnable(_bool bIsEnable)override;


private:
	virtual HRESULT		Add_Component(void);
	virtual HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

private:
	void				CheckInventory();
private:
	vector<pair<wstring, _uint>>*	m_InvenVec;
	C3DIcon*						m_pIcon[3] = { nullptr,nullptr,nullptr };

public:
	static CShop*		Create(LPDIRECT3DDEVICE9 pGraphicDev,  wstring wstrTexName, _float fLength,_float fRotY,_bool bIsRight =true, UISTATE eUIState=UI_END);

private:
	virtual void Free(void) override;
};

#endif // Shop_h__
