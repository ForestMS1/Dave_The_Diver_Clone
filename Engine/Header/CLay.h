#pragma once
#include "CGameObject.h"

class CTerrainTex;
class CTransform;

class CLay : public CGameObject
{
protected:
	explicit CLay();
	explicit CLay(const CLay& rhs);
	virtual ~CLay();

public:
	virtual		HRESULT		Ready_GameObject();
	virtual		_int		Update_GameObject(const _float& fTimeDelta);
	virtual		void		LateUpdate_GameObject(const _float& fTimeDelta);

public:
	_vec3					Map_Picking(HWND hWnd);

protected:
	virtual void	Free();
};

