#pragma once
#include "CBase.h"
#include "CLight.h"
#include "Engine_Enum.h"

BEGIN(Engine)

class ENGINE_DLL CLightMgr : public CBase
{
	DECLARE_SINGLETON(CLightMgr)

private:
	explicit CLightMgr();
	virtual ~CLightMgr();

	void UpdateLight();

public:



	HRESULT Add_Light(CLight* Light, LIGHTID _ID);

private:
	list<CLight*>			m_LightList[LIGHT_END];

private:
	virtual void		Free();
};

END