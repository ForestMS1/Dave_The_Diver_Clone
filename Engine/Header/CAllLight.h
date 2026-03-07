#pragma once
#include "Engine_Define.h"
#include "CLight.h"
BEGIN(Engine)

class ENGINE_DLL CAllLight : public CLight
{
public:
	explicit CAllLight();
	virtual ~CAllLight();

public:
	virtual		HRESULT			Ready_Light(const _uint& iIndex);
	virtual		 void				Off_Light();

	static CAllLight* Create(const _uint& iIndex);

private:
	virtual void Free();
};

END


