#pragma once
#include "CBaseState.h"
class CJohn2;

class CGunSkill02Aim :
    public CBaseState<CJohn2>
{
private:
    explicit CGunSkill02Aim(CJohn2* pOwner);
    virtual ~CGunSkill02Aim();

public:
    void Enter() override;
    void Input(const _float& fTimeDelta) override;
    _int Update_State(const _float& fTimeDelta) override;
    void LateUpdate_State(const _float& fTimeDelta) override;
    void Render_State() override;
    void Exit() override;
    void Clear() override;

public:
    static CGunSkill02Aim* Create(CJohn2* pOwner);

private:
    wstring_view m_wsTexName;
    _float m_fSplashDelay = 0.f;
    _uint m_iSplashCnt = 0;

    _float m_fChangeAnimTime = 0.f;
private:
    virtual void Free() override;
};

