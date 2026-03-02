#pragma once
#include "CBaseState.h"
class CJohn;

class CJohnBattleAngry :
    public CBaseState<CJohn>
{
private:
    explicit CJohnBattleAngry(CJohn* pOwner);
    virtual ~CJohnBattleAngry();

public:
    void Enter() override;
    void Input(const _float& fTimeDelta) override;
    _int Update_State(const _float& fTimeDelta) override;
    void LateUpdate_State(const _float& fTimeDelta) override;
    void Render_State() override;
    void Exit() override;
    void Clear() override;

public:
    static CJohnBattleAngry* Create(CJohn* pOwner);

private:
    wstring_view m_wsTexName;
    _float m_fSplashDelay = 0.f;
    _uint m_iSplashCnt = 0;
private:
    virtual void Free() override;
};

