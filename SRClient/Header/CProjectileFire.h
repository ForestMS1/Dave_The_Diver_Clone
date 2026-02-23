#pragma once
#include "CBaseState.h"

class CHarpoonProjectile;


class CProjectileFire : public CBaseState<CHarpoonProjectile>
{
private:
    explicit CProjectileFire(CHarpoonProjectile* pOwner);
    ~CProjectileFire();

public:
    void Enter() override;
    void Input(const _float& fTimeDelta) override;
    _int Update_State(const _float& fTimeDelta) override;
    void LateUpdate_State(const _float& fTimeDelta) override;
    void Render_State() override;
    void Exit() override;
    void Clear() override;

private:
    void Go_ToDir(const _float& fTimeDelta);

private:
    _bool m_bIsHitFish = false;

public:
    static CProjectileFire* Create(CHarpoonProjectile* pOwner);
private:
    virtual void Free() override;
};

