#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTriCol :   public CVIBuffer
{
private:
	explicit CTriCol();
	explicit CTriCol(const CTriCol& rhs);
	virtual ~CTriCol();

public:
	virtual		HRESULT		Ready_Buffer();
	virtual		void		Render_Buffer();

public:
	static CTriCol* Create();
	virtual CComponent* Clone();
private:
	virtual void	Free();

};

END