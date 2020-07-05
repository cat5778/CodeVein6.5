#ifndef Collider_h__
#define Collider_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollider : public CComponent
{
private:
	explicit CCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCollider(void);

public:
	const _vec3*		Get_Min(void)  { return &m_vMin; }
	const _vec3*		Get_Max(void) { return &m_vMax; }
	const _matrix*		Get_ColliderMatrix(void) { return &m_matWorld; }


public:
	HRESULT		Ready_Collider(const _vec3* pPos, const _ulong& dwNumVtx, const _ulong& dwStride);
	void		Render_Collider(COLLTYPE eType, const _matrix* pColliderMatrix);

private:
	_vec3		m_vMin, m_vMax;
	_matrix		m_matWorld;

	LPDIRECT3DDEVICE9			m_pGraphicDev;

#ifdef _DEBUG
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;
	LPDIRECT3DTEXTURE9			m_pTexture[COL_END];

#endif

public:
	static CCollider*		Create(LPDIRECT3DDEVICE9 pGraphicDev, 
									const _vec3* pPos, 
									const _ulong& dwNumVtx,
									const _ulong& dwStride);

private:
	virtual void		Free(void);
};

END
#endif // Collider_h__
