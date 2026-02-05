#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcCol : public CVIBuffer
{
private:
	explicit CRcCol();
	explicit CRcCol(const CRcCol& rhs);
	virtual ~CRcCol();

public:
	virtual		HRESULT		Ready_Buffer();
	virtual		void		Render_Buffer();

public:
	static CRcCol* Create();
	virtual CComponent* Clone();
private:
	virtual void	Free();

};

END