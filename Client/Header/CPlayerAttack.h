#pragma once
#include "CPlayerState.h"
class CPlayerAttack : public CPlayerState
{
public:
	_int Update_State(const _float& fTimeDelta) override;
	void LateUpdate_State(const _float& fTimeDelta) override;

private:
	void Free() override;
};

