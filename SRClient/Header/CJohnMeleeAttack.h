#pragma once
#include "CBaseState.h"

class CJohn;

class CJohnMeleeAttack :
    public CBaseState<CJohn>
{
private:
    explicit CJohnMeleeAttack(CJohn* pOwner);
    virtual ~CJohnMeleeAttack();

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
    static CJohnMeleeAttack* Create(CJohn* pOwner);

private:
    wstring_view m_wsTexName;
    _bool   m_bGoComplete = false;

    _bool   m_bCompleteFramSizeCalc = false; //го..
private:
    virtual void Free() override;
};

