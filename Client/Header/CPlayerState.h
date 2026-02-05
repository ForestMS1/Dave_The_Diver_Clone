#pragma once
#include "CBase.h"
#include "Engine_Define.h"

class CPlayerState : public CBase
{
public:
	virtual _int Update_State(const _float& fTimeDelta) = 0;
	virtual void LateUpdate_State(const _float& fTimeDelta) = 0;

private:
	void Free() override;
};

