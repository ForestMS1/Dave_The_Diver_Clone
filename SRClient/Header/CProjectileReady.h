#pragma once
#include "CBaseState.h"

class CHarpoonProjectile;

class CProjectileReady :
    public CBaseState<CHarpoonProjectile>
{
private:
    explicit CProjectileReady(CHarpoonProjectile* pOwner);
    ~CProjectileReady();

public:
    void Enter() override;
    void Input(const _float& fTimeDelta) override;
    _int Update_State(const _float& fTimeDelta) override;
    void LateUpdate_State(const _float& fTimeDelta) override;
    void Render_State() override;
    void Exit() override;
    void Clear() override;

private:
    void	Set_ParentTransform();
    void	Rotate_ToMouse();
    void	Shot_ToMouse();
public:
    static CProjectileReady* Create(CHarpoonProjectile* pOwner);
private:
    virtual void Free() override;
};

