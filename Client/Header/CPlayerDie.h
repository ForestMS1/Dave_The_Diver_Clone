#pragma once
#include "CPlayerState.h"
class CPlayerDie : public CPlayerState
{
private:
	explicit CPlayerDie(CDSPlayer* pPlayer);
	virtual ~CPlayerDie();

public:
	void Enter() override;
	_int Update_State(const _float& fTimeDelta) override;
	void LateUpdate_State(const _float& fTimeDelta) override;
	void Render_State() override;
	void Exit() override;

private:
	virtual void Clear() override;

public:
	static CPlayerDie* Create(CDSPlayer* pPlayer);

private:
	void Free() override;
};

