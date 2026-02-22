#pragma once
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CLineBuffer : public CComponent
{
private:
	explicit CLineBuffer();
	explicit CLineBuffer(const CLineBuffer& rhs);
	virtual ~CLineBuffer();

public:
	virtual		HRESULT		Ready_Buffer();
	void		Render_Buffer(vector<_vec3>& points, _matrix& matVP);

public:
	static CLineBuffer* Create();
	virtual CComponent* Clone();

private:
	LPD3DXLINE		m_pLine;
private:
	virtual void	Free();

};

END