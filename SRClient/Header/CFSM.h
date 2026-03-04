#pragma once
#include "CBase.h"
#include "CBaseState.h"


//ex CFSM<CDiveDave, DIVEDAVESTATE>

// T는 FSM을 소유하는 객체 클래스, E는 상태 정의한 enum class

template<typename T, typename E> // , typename std::enable_if<std::is_class<T>::value&& std::is_enum<E>::value, int>::type = 0>
class CFSM : public CBase
{
private:
	explicit					 CFSM(T* pOwner) : m_pOwner(pOwner) {}
	virtual						~CFSM() {}

public:
	template<typename S>
	void						Add_State(E e_State) {
		auto iter = m_mapState.find(e_State);
		if (iter != m_mapState.end())
			return;

		m_mapState.insert({ e_State, S::Create(m_pOwner) });
	}

public:
	_int						Update_State(const _float& fTimeDelta)		{ m_pState->Update_State(fTimeDelta); return 0; }
	void						LateUpdate_State(const _float& fTimeDelta)	{ m_pState->LateUpdate_State(fTimeDelta); }
	void						Render_State()								{ m_pState->Render_State(); }

public:
	CBaseState<T>*				Get_pState()				{ return m_pState; }
	E							Get_State()					{ return m_eCurState; }
	void						Set_State(E state){
		auto iter = m_mapState.find(state);
		if (iter == m_mapState.end())
			return;

		if (m_mapState[state] == m_pState)
			return;

		if (m_pState != nullptr)
			m_pState->Exit();

		m_pState = m_mapState[state];
		m_ePrevState = m_eCurState; // 현재 상태를 이전 상태로 설정
		m_eCurState = state;

		m_pState->Enter();
	}
	E							Get_PrevState()				{ return m_ePrevState; }

public:
	static CFSM*				Create(T* pOwner){
		CFSM<T, E>* pFSM = new CFSM<T, E>(pOwner);

		return pFSM;
	}

private:
	T*									m_pOwner = nullptr;
	CBaseState<T>*						m_pState = nullptr;
	E									m_eCurState;
	E									m_ePrevState;
	unordered_map<E, CBaseState<T>*>	m_mapState;

private:
	virtual void Free() { for_each(m_mapState.begin(), m_mapState.end(), CDeleteMap()); m_mapState.clear(); };
};

