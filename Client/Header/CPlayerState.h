#pragma once
#include "CBase.h"
#include "Engine_Define.h"

class CPlayerState : public CBase
{
public:
	virtual void Enter() = 0; // 상태 진입 시 한번만 실행
	virtual _int Update_State(const _float& fTimeDelta) = 0;
	virtual void LateUpdate_State(const _float& fTimeDelta) = 0;
	virtual void Exit() = 0; // 상태 탈출 시 한번만 실행
	virtual void Render_State() = 0;

private:
	void Free() override;
};

