#pragma once
#include "CBase.h"
#include "CGameObject.h"

enum class DIVEDAVESTATE
{
	IDLE = 0,
	MOVE,
	ATTACK,			// 작살, 총 공격
	MELEEATTACK,	// 근접 공격
	TANNING,
	OPEN,
	PICKUP,
	HIT,
	DIE,
	DAVE_STATE_END
};

enum class PROJECTILESTATE 
{ 
	READY, 
	FIRE, 
	HIT, 
	RETURN, 
	STATE_END 
};

enum class ATTACKSUBSTATE
{
	ATTACK_READY,
	ATTACK_FIRE,
	ATTACK_FIGHT,
	ATTACK_FAIL,
	SUB_END
};

enum class JOHNSTATE
{
	IDLE = 0,
	MOVE,
	ATTACK,
	MELEEATTACK,
	HIT,
	DIE,
	JOHN_STATE_END
};

// T는 상태를 소유하는 객체 클래스
template<typename T>
class CBaseState : public CBase
{
public:
	explicit CBaseState(T* pOwner) : m_pOwner(pOwner) {}
	virtual ~CBaseState() {}

public:
	virtual void Enter() = 0; // 상태 진입 시 한번만 실행
	virtual void Input(const _float& fTimeDelta) = 0; // 현재 상태에서만 받을 키,마우스 입력
	virtual _int Update_State(const _float& fTimeDelta) = 0;
	virtual void LateUpdate_State(const _float& fTimeDelta) = 0;
	virtual void Render_State() = 0;
	virtual void Exit() = 0; // 상태 탈출 시 한번만 실행

protected:
	virtual void Clear() = 0; // 상태마다의 값, 플래그 초기화 (Enter에서 호출)

protected:
	T* m_pOwner = nullptr;

protected:
	virtual void Free() {};
};

