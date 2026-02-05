#pragma once
#include "CPlayerState.h"
class CPlayerDie : public CPlayerState
{
public:
	void Enter() override;
	_int Update_State(const _float& fTimeDelta) override;
	void LateUpdate_State(const _float& fTimeDelta) override;
	void Render_State() override;
	void Exit() override;
private:
	void Free() override;
};

