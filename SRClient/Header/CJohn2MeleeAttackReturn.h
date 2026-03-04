#pragma once
#include "CBaseState.h"
class CJohn2;

class CJohn2MeleeAttackReturn :
    public CBaseState<CJohn2>
{
private:
    explicit CJohn2MeleeAttackReturn(CJohn2* pOwner);
    virtual ~CJohn2MeleeAttackReturn();

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
    static CJohn2MeleeAttackReturn* Create(CJohn2* pOwner);

private:
    wstring_view m_wsTexName;

private:
    virtual void Free() override;
};

