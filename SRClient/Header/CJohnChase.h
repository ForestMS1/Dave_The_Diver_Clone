#pragma once
#include "CBaseState.h"

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


class CJohn;

class CJohnChase :
    public CBaseState<CJohn>
{
private:
    explicit CJohnChase(CJohn* pOwner);
    virtual ~CJohnChase();

public:
    void Enter() override;
    void Input(const _float& fTimeDelta) override;
    _int Update_State(const _float& fTimeDelta) override;
    void LateUpdate_State(const _float& fTimeDelta) override;
    void Render_State() override;
    void Exit() override;
    void Clear() override;

public:
    static CJohnChase* Create(CJohn* pOwner);

private:
    void MoveTo_Target(const _float& fTimeDelta);
    void Switch_TexName();
    void Reset_Size();
    void Set_Size();

    wstring_view m_wsTexName;
    CTransform* m_pOwnerTransform = nullptr;

private:
    JOHNDIR m_eDir = DIR_END;

private:
    virtual void Free() override;
};

