#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCubeCol : public CVIBuffer
{
private:
	explicit CCubeCol();
	explicit CCubeCol(const CCubeCol& rhs);
	virtual ~CCubeCol();

public:
	virtual		HRESULT		Ready_Buffer(_ulong dwColor);
	virtual		void		Render_Buffer();

public:
	static CCubeCol* Create(_ulong dwColor);
	virtual CComponent* Clone();
private:
	virtual void	Free();
};

END
