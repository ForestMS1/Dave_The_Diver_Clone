#pragma once

#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CLight : public CBase
{
private:
	explicit CLight();
	virtual ~CLight();

public:
	HRESULT			Ready_Light(const D3DLIGHT9* pLightInfo, const _uint& iIndex);

private:
	_uint				m_iIndex;
	D3DLIGHT9			m_tLight;

public:
	static CLight* Create(
		const D3DLIGHT9* pLightInfo,
		const _uint& iIndex);

private:
	virtual void	Free();

};

END
