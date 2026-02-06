#pragma once
#include "CBase.h"
#include "Engine_Define.h"

class CDSPlayer;

class CPlayerState : public CBase
{
protected:
	explicit CPlayerState(CDSPlayer* pPlayer);
	virtual ~CPlayerState();

public:
	virtual void Enter() = 0; // 상태 진입 시 한번만 실행
	virtual _int Update_State(const _float& fTimeDelta) = 0;
	virtual void LateUpdate_State(const _float& fTimeDelta) = 0;
	virtual void Render_State() = 0;
	virtual void Exit() = 0; // 상태 탈출 시 한번만 실행

protected:
	virtual void Clear() = 0; // 상태마다의 값, 플래그 초기화 (Enter에서 호출)

protected:
	CDSPlayer* m_pPlayer;

protected:
	void Free() override;
};

