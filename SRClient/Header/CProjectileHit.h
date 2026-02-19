#pragma once
#include "CPlayerState.h"
class CProjectileHit :
    public CPlayerState
{
private:
    explicit CProjectileHit(CGameObject* pOwner);
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
    static CProjectileHit* Create(CGameObject* pOwner);
private:
    virtual void Free() override;
};

