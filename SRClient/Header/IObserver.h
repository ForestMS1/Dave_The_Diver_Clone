#pragma once
#include "CGameObject.h"
enum class EVENTTYPE
{
	NO_EVENT = 0,
	GET_ITEM,
	USE_ITEM,
	CHANGE_HP,
	CHANGE_DEPTH,
	CHANGE_WEIGTH,
	ATTACK_START,
	ATTACK_GAUGE_CHANGE,
	ATTACK_END,
	GET_WEAPON,
	BOSS_INTRO_START,
	BOSS_INTRO_END,
	WEAPONSLOT_CHANGE,
	ITEMSLOT_CHANGE,
	//...
	EVENTTYPE_END
};

struct Event
{
	EVENTTYPE type = EVENTTYPE::NO_EVENT;
	int value = 0;
	float fValue = 0.f;
	float fValue2 = 0.f;
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

