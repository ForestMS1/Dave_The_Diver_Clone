#include "IObserver.h"

IObserver::IObserver()
{
}

IObserver::IObserver(const IObserver& rhs)
	: CGameObject(rhs)
{
}

IObserver::~IObserver()
{
}
