#pragma once
#include "CBaseState.h"

class CHarpoonProjectile;

class CProjectileHit :
    public CBaseState<CHarpoonProjectile>
{
private:
    explicit CProjectileHit(CHarpoonProjectile* pOwner);
    ~CProjectileHit();

public:
    void Enter() override;
    void Input(const _float& fTimeDelta) override;
    _int Update_State(const _float& fTimeDelta) override;
    void LateUpdate_State(const _float& fTimeDelta) override;
    void Render_State() override;
    void Exit() override;
    void Clear() override;

public:
    static CProjectileHit* Create(CHarpoonProjectile* pOwner);
private:
    virtual void Free() override;
};

