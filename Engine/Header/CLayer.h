#pragma once
#include "CBase.h"
#include "CGameObject.h"

BEGIN(Engine)

class ENGINE_DLL CLayer : public CBase
{
private:
	explicit CLayer();
	virtual ~CLayer();

public:
	HRESULT			Add_GameObject(std::wstring_view svObjTag, CGameObject* pGameObject);

	list<CGameObject*> * Get_GameObjects(std::wstring_view svObjTag);
	CGameObject* Get_GameObjectFirst(std::wstring_view svObjTag);

public:
	HRESULT			Ready_Layer();
	_int			Update_Layer(const _float& fTimeDelta);
	void			LateUpdate_Layer(const _float& fTimeDelta);

private:
	map<const std::wstring, list<CGameObject*>> m_mapGameObjects;

public:
	static CLayer* Create();

private:
	virtual void	Free();
};

END