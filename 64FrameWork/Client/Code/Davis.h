#pragma once
#include "Defines.h"
#include "GameObject.h"
#include "DynamicObject.h"
BEGIN(Engine)
class CNaviMesh;
END
class CDavis : public CDynamicObject
{
public:
	explicit				CDavis(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx, _vec3 vPos, _uint uiCellIdx = 0);
	virtual					~CDavis(void);

public:
	HRESULT					Ready_GameObject();
	virtual	HRESULT			LateReady_GameObject()override;
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			Render_GameObject(void) override;
	

private:
	void					Collision_Check(_float fTimeDelta);

private:
	virtual HRESULT			Add_Component(void);
	void					Set_TransformData();
	void					StateMachine()override;

private://Pattern

public:
	static CDavis*			Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx, _vec3 vPos, _uint uiCellIdx = 0);



private:

	DAVISSTATE				m_eCurState;
	DAVISSTATE				m_ePreState;

private:
	virtual void			Free(void) override;
};

/* Davis Ani*/
//46. Unique_Idle
//47. Emotion_Nagative
//48. Emotion_Pogitive
//49. Idle
