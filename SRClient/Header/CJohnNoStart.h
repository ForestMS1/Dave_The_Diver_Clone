#pragma once
#include "CBaseState.h"
class CJohn;

class CJohnNoStart :
    public CBaseState<CJohn>
{
private:
    explicit CJohnNoStart(CJohn* pOwner);
    virtual ~CJohnNoStart();

public:
    void Enter() override;
    void Input(const _float& fTimeDelta) override;
    _int Update_State(const _float& fTimeDelta) override;
    void LateUpdate_State(const _float& fTimeDelta) override;
    void Render_State() override;
    void Exit() override;
    void Clear() override;

private:
    _bool m_bSoundBgm = false;
    _bool m_bJohnDialogCreated = false;
    _bool m_bJohnDialog2Created = false;
    _bool m_bDaveDialogCreated = false;
    
public:
    static CJohnNoStart* Create(CJohn* pOwner);
private:
    virtual void Free() override;
};

