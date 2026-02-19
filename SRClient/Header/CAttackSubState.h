#pragma once
#include "CBase.h"
#include "CGameObject.h"

class CDiveDaveAttack;

class CAttackSubState :
    public CBase
{
public:
	explicit CAttackSubState(CGameObject* pPlayer, CDiveDaveAttack* pParentState);
	virtual ~CAttackSubState();

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
	CGameObject* m_pPlayer;
	CDiveDaveAttack* m_pParentState;

protected:
	virtual void Free() override;
};

