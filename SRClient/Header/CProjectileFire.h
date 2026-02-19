#pragma once
#include "CPlayerState.h"
class CProjectileFire :
    public CPlayerState
{
private:
    explicit CProjectileFire(CGameObject* pOwner);
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
    static CProjectileFire* Create(CGameObject* pOwner);
private:
    virtual void Free() override;
};

