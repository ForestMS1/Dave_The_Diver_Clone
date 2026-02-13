#pragma once
#include "CPlayerState.h"
class ENGINE_DLL CPlayerIdle : public CPlayerState
{
private:
	explicit CPlayerIdle(CDSPlayer* pPlayer);
	virtual ~CPlayerIdle();
public:
	void Enter() override;
	_int Update_State(const _float& fTimeDelta) override;
	void LateUpdate_State(const _float& fTimeDelta) override;
	void Render_State() override;
	void Exit() override;

private:
	virtual void Clear() override;
public:
	static CPlayerIdle* Create(CDSPlayer* pPlayer);

private:
	void Free() override;
};