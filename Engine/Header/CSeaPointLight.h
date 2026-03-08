
#pragma once
#include "Engine_Define.h"
#include "CLight.h"
BEGIN(Engine)

class ENGINE_DLL CSeaPointLight : public CLight
{
public:
	explicit CSeaPointLight();
	virtual ~CSeaPointLight();

public:
	virtual		HRESULT			Ready_Light(const _uint& iIndex);
	virtual		 void				Off_Light();

	static CSeaPointLight* Create(const _uint& iIndex, _vec3 _pos);


	void Set_Pos(_vec3 _pos) { p_Pos = _pos; }
private:
	_vec3 p_Pos{};
private:
	virtual void Free();
};

END


