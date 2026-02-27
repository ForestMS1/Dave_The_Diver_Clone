#pragma once
#include "CBaseState.h"

class CDiveDave;

class CDiveDaveIdle : public CBaseState<CDiveDave>
{
private:
	explicit CDiveDaveIdle(CDiveDave* pOwner);
	~CDiveDaveIdle();

public:
	void Enter() override;
	void Input(const _float& fTimeDelta) override;
	_int Update_State(const _float& fTimeDelta) override;
	void LateUpdate_State(const _float& fTimeDelta) override;
	void Render_State() override;
	void Exit() override;
	void Clear() override;

private:
	void Restore_Fov(const _float& fTimeDelta);

private:
	// 아이템 슬롯 체인지 딜레이
	_bool m_bItemSlotDefense = false;
	_float m_fItemSlotChangeDelay = 1.f;

	// 무기 슬롯 체인지 딜레이
	_bool m_bWeaponSlotDefense = false;
	_float m_fWeaponSlotChangeDelay = 1.f;
public:
	static CDiveDaveIdle* Create(CDiveDave* pOwner);
private:
	virtual void Free() override;
};

