#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)
class ENGINE_DLL CTargetArrowTex :
    public CVIBuffer
{
private:
	explicit CTargetArrowTex();
	explicit CTargetArrowTex(const CTargetArrowTex& rhs);
	virtual ~CTargetArrowTex();

public:
	virtual		HRESULT		Ready_Buffer();
	virtual		void		Render_Buffer();

public:
	static CTargetArrowTex* Create();
	virtual CComponent* Clone();
private:
	virtual void	Free();

};
END
