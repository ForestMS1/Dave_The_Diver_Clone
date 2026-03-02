#pragma once
#include "CBaseState.h"
class CJohn2;

class CJohn2Chase :
    public CBaseState<CJohn2>
{
private:
    explicit CJohn2Chase(CJohn2* pOwner);
    virtual ~CJohn2Chase();

public:
    void Enter() override;
    void Input(const _float& fTimeDelta) override;
    _int Update_State(const _float& fTimeDelta) override;
    void LateUpdate_State(const _float& fTimeDelta) override;
    void Render_State() override;
    void Exit() override;
    void Clear() override;

public:
    static CJohn2Chase* Create(CJohn2* pOwner);

private:
    void MoveTo_Target(const _float& fTimeDelta);
    void Switch_TexName();
    void Reset_Size();
    void Set_Size();

    wstring_view m_wsTexName;
    CTransform* m_pOwnerTransform = nullptr;

private:
    enum JOHNDIR
    {
        UP = 0,
        UP_RIGHT,
        UP_LEFT,
        RIGHT,
        LEFT,
        DOWN,
        DOWN_RIGHT,
        DOWN_LEFT,
        DIR_END = 8
    };
    JOHNDIR m_eDir = DIR_END;

private:
    virtual void Free() override;
};

