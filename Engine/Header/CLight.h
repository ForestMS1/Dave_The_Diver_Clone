#pragma once

#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CLight : public CBase
{
protected:
	explicit CLight();
	virtual ~CLight();

public:
	virtual		 HRESULT			Ready_Light(const _uint& iIndex) =0;
	virtual		 void				Off_Light() {};

	_uint Get_Index() { return m_iIndex; }
protected:
	_uint				m_iIndex;
	D3DLIGHT9			m_tLight;


private:
	virtual void	Free();

};

END
