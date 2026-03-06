#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CGoldBallBuffer : public CVIBuffer
{
private:
	explicit CGoldBallBuffer();
	explicit CGoldBallBuffer(const CGoldBallBuffer& rhs);
	virtual ~CGoldBallBuffer();

public:
	virtual		HRESULT		Ready_Buffer();
	virtual		void		Render_Buffer();

public:
	static CGoldBallBuffer* Create();
	virtual CComponent* Clone();
private:
	virtual void	Free();

};

END