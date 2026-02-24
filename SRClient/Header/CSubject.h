#pragma once
#include "CGameObject.h"
#include "IObserver.h"

class CSubject : public CGameObject
{
public:
	explicit CSubject();
	explicit CSubject(const CSubject& rhs);
	virtual ~CSubject();

public:
	void	Add_Observer(IObserver* observer)
	{
		if (!observer)
			return;

		auto iter = std::find(m_vecObserver.begin(), m_vecObserver.end(), observer);
		if (iter == m_vecObserver.end())
			m_vecObserver.push_back(observer);
	}

	void	Remove_Observer(IObserver* observer)
	{
		m_vecObserver.erase(
			std::remove(m_vecObserver.begin(), m_vecObserver.end(), observer),
			m_vecObserver.end()
		);
	}

protected:
	void Notify(const Event& e)
	{
		auto observers = m_vecObserver;

		for (auto& observer : observers)
		{
			if(observer)
				observer->OnNotify(e);
		}
	}
	

protected:
	std::vector<IObserver*> m_vecObserver;
};

