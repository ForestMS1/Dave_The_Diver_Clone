#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcDynamicTex : public CVIBuffer
{
private:
	explicit CRcDynamicTex();
	explicit CRcDynamicTex(const CRcDynamicTex& rhs);
	virtual ~CRcDynamicTex();

public:
	virtual		HRESULT		Ready_Buffer();
	virtual		void		Render_Buffer();
	void Update_Gauge(const _float& fRatio);

public:
	static CRcDynamicTex* Create();
	virtual CComponent* Clone();
private:
	virtual void	Free();

};

END