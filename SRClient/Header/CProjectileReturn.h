#pragma once
#include "CBaseState.h"

class CHarpoonProjectile;

class CProjectileReturn :
    public CBaseState<CHarpoonProjectile>
{
private:
    explicit CProjectileReturn(CHarpoonProjectile* pOwner);
    ~CProjectileReturn();

public:
    void Enter() override;
    void Input(const _float& fTimeDelta) override;
    _int Update_State(const _float& fTimeDelta) override;
    void LateUpdate_State(const _float& fTimeDelta) override;
    void Render_State() override;
    void Exit() override;
    void Clear() override;

private:
    void Set_ParentTransform();
    void Return_Act(const _float& fTimeDelta);
public:
    static CProjectileReturn* Create(CHarpoonProjectile* pOwner);
private:
    virtual void Free() override;
};

