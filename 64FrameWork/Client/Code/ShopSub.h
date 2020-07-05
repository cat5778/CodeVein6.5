#ifndef ShopSub_h__
#define ShopSub_h__

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
class CShopSub : public C3DUI
{
private:
	explicit CShopSub(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, _float fLength, _float fRotY, _bool bIsRight, UISTATE eUIState);
	virtual ~CShopSub(void);

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual HRESULT	LateReady_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	
	virtual void Render_GameObject(void) override;

public:
	virtual wstring		Get_ItemName()override;
private:
	virtual HRESULT		Add_Component(void);
	virtual HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

private:


public:
	static CShopSub*		Create(LPDIRECT3DDEVICE9 pGraphicDev,  wstring wstrTexName, _float fLength,_float fRotY,_bool bIsRight =true, UISTATE eUIState=UI_END);

private:
	virtual void Free(void) override;
};

#endif // ShopSub_h__
