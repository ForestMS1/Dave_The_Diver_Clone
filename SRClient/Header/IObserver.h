#pragma once
#include "CGameObject.h"
enum class EVENTTYPE
{
	NO_EVENT = 0,
	GET_ITEM,
	USE_ITEM,
	RESTORE_HP,
	WEAPON_CHANGE,
	ITEMSLOT_CHANGE,
	//...
	EVENTTYPE_END
};

struct Event
{
	EVENTTYPE type = EVENTTYPE::NO_EVENT;
	int value = 0;
	std::wstring_view ItemTextureName;
};

class IObserver : public CGameObject
{
public:
	explicit IObserver();
	explicit IObserver(const IObserver& rhs);
	virtual ~IObserver();

public:
	virtual void OnNotify(const Event& e) = 0;
};

