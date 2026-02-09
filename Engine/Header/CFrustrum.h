#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CFrustrum : public CVIBuffer
{
private:
	explicit CFrustrum();
	explicit CFrustrum(const CFrustrum& rhs);
	virtual ~CFrustrum();

public:
	virtual		HRESULT		Ready_Buffer();
			void		Render_Buffer(const _matrix& view, const _matrix& proj);

public:
	static CFrustrum* Create();
	virtual CComponent* Clone();
private:
	virtual void	Free();
};

END