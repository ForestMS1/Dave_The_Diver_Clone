#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CAllLight : public CLight
{
public:
	explicit CAllLight();
	virtual ~CAllLight();

public:
	virtual HRESULT			Ready_Light(const _uint& iIndex);


	static CAllLight* Create(const _uint& iIndex);

private:
	void Free() override;
};

END


