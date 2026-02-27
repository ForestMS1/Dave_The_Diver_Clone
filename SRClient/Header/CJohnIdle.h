#pragma once
#include "CBaseState.h"

class CJohn;

class CJohnIdle :
    public CBaseState<CJohn>
{
private:
    explicit CJohnIdle(CJohn* pOwner);
    virtual ~CJohnIdle();

public:
    void Enter() override;
    void Input(const _float& fTimeDelta) override;
    _int Update_State(const _float& fTimeDelta) override;
    void LateUpdate_State(const _float& fTimeDelta) override;
    void Render_State() override;
    void Exit() override;
    void Clear() override;

public:
    static CJohnIdle* Create(CJohn* pOwner);
private:
    virtual void Free() override;

};

