#ifndef Inven_h__
#define Inven_h__

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
class CInvenInfo;
class CInven : public C3DUI
{
private:
	explicit CInven(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrTexName, _float fLength, _float fRotY, _bool bIsRight, UISTATE eUIState);
	virtual ~CInven(void);

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual HRESULT	LateReady_GameObject(void);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	
	virtual void Render_GameObject(void) override;

public:
	virtual wstring		Get_ItemName()override;
	virtual void		ChangeEnable(_bool bIsEnable)override;
	virtual void		ChangeEnable()override;


private:
	virtual HRESULT		Add_Component(void);
	virtual HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

private:
	void				CheckInventory();
private:
	vector<pair<wstring, _uint>>*	m_InvenVec;
	C3DIcon*						m_pIcon[3] = { nullptr,nullptr,nullptr };
	CInvenInfo*						m_pInfoUI = nullptr;

	//TEst
	_vec3							m_vTempTest = { INIT_VEC3 };

public:
	static CInven*		Create(LPDIRECT3DDEVICE9 pGraphicDev,  wstring wstrTexName, _float fLength,_float fRotY,_bool bIsRight =true, UISTATE eUIState=UI_END);

private:
	virtual void Free(void) override;
};

#endif // Inven_h__
