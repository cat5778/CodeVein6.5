#ifndef RcTex_h__
#define RcTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcTex : public CVIBuffer
{
private:
	explicit CRcTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcTex(const CRcTex& rhs);
	virtual ~CRcTex(void);

public:
	virtual HRESULT Ready_Buffer(void) override;
	HRESULT	Ready_Buffer(_uint iNumInstance);
	void	Render_InstanceBuffer(void);

private:
	LPDIRECT3DVERTEXBUFFER9			m_pVBTransform;
	LPDIRECT3DVERTEXDECLARATION9	m_pDeclaration;
	_uint							m_iNumInstance;

public:
	static CRcTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CRcTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iNumInstance);
	virtual CResources* Clone(void) override;

private:
	virtual void Free(void);
};

END
#endif // RcTex_h__
