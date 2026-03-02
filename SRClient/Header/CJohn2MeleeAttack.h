#pragma once
#include "CBaseState.h"
class CJohn2;

class CJohn2MeleeAttack :
    public CBaseState<CJohn2>
{
private:
    explicit CJohn2MeleeAttack(CJohn2* pOwner);
    virtual ~CJohn2MeleeAttack();

public:
    void Enter() override;
    void Input(const _float& fTimeDelta) override;
    _int Update_State(const _float& fTimeDelta) override;
    void LateUpdate_State(const _float& fTimeDelta) override;
    void Render_State() override;
    void Exit() override;
    void Clear() override;

    void Reset_Size();
    void Set_Size();

public:
    static CJohn2MeleeAttack* Create(CJohn2* pOwner);

private:
    wstring_view m_wsTexName;
    _bool   m_bGoComplete = false;

private:
    virtual void Free() override;
};

