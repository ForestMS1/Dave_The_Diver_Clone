#pragma once
#include "CFishState.h"
namespace Fish
{
	class CFishStateMachine
	{
	public:
		CFishStateMachine(FISH_STATE eInitialState)
			: m_eTmpState(FS_END)
			, m_eCurrState(eInitialState)
		{
		}
		~CFishStateMachine() = default;

	public:
		void AddState(FISH_STATE eFishState, unique_ptr<CFishState> pState)
		{
			m_mapState.insert({ eFishState , move(pState) });
		}

		bool IsRequestedChange()
		{
			return m_eTmpState != FS_END;
		}

		void ChangeState()
		{
			Get_CurrentState()->Exit();
			m_eCurrState = m_eTmpState;
			Get_CurrentState()->Enter();

			m_eTmpState = FS_END;
		}

		void RequestChangeState(FISH_STATE eToState)
		{
			m_eTmpState = eToState;
		}

		CFishState* Get_CurrentState() { return m_mapState[m_eCurrState].get(); };
		FISH_STATE Get_CurrentStateId() { return m_eCurrState; };
		std::wstring Get_CurrentStateStr()
		{
			switch (m_eCurrState)
			{
			case Fish::FS_STOP: return L"FS_STOP";
			case Fish::FS_SWIM:return L"FS_SWIM";
			case Fish::FS_SPRINT:return L"FS_SPRINT";
			case Fish::FS_DIE:return L"FS_DIE";
			case Fish::FS_QTE:return L"FS_QTE";
			case Fish::FS_END:return L"FS_END";
			default: return L"FS_NOTFOUND_DEFAULT";
			}
		}

	private:
		FISH_STATE m_eCurrState;
		unordered_map<FISH_STATE, unique_ptr<CFishState>> m_mapState;

		FISH_STATE m_eTmpState;
	};
}