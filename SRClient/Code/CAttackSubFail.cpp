#include "CAttackSubFail.h"
#include "CDiveDave.h"
#include "CDInputMgr.h"
#include "CCameraMgr.h"
#include "CDiveDaveCam.h"
CAttackSubFail::CAttackSubFail(CGameObject* pPlayer, CDiveDaveAttack* pParentState)
    :CAttackSubState(pPlayer, pParentState)
{
}

CAttackSubFail::~CAttackSubFail()
{
}

void CAttackSubFail::Enter()
{
    static_cast<CDiveDave*>(m_pPlayer)->Init_Frame();
    _float fWidth = 47.f;
    _float fHeight = 54.f;
    _float fAspect = fWidth + fHeight;
    fAspect /= 2.f;

    _vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
    static_cast<CDiveDave*>(m_pPlayer)->Multiply_Scale(&vScale);
    static_cast<CDiveDave*>(m_pPlayer)->Set_TextureCom(L"Com_AttackFailTexture");
}

void CAttackSubFail::Input(const _float& fTimeDelta)
{
}

_int CAttackSubFail::Update_State(const _float& fTimeDelta)
{
    if (static_cast<CDiveDave*>(m_pPlayer)->Get_State() != DiveState::ATTACK)
        return 0;

    Input(fTimeDelta);
    static_cast<CDiveDave*>(m_pPlayer)->AddFrame(fTimeDelta, 10.f, 1);
    //Mouse_Check();

    return 0;
}

void CAttackSubFail::LateUpdate_State(const _float& fTimeDelta)
{
    if (static_cast<CDiveDave*>(m_pPlayer)->Get_State() != DiveState::ATTACK)
        return;
}

void CAttackSubFail::Render_State()
{
    if (static_cast<CDiveDave*>(m_pPlayer)->Get_State() != DiveState::ATTACK)
        return;

    CTexture* pPlayerTextureCom = static_cast<CDiveDave*>(m_pPlayer)->Get_TextureCom();

    _float fFrame = static_cast<CDiveDave*>(m_pPlayer)->Get_Frame();

    pPlayerTextureCom->Set_Texture((_uint)fFrame);
}

void CAttackSubFail::Exit()
{
    _float fWidth = 47.f;
    _float fHeight = 54.f;
    _float fAspect = fWidth + fHeight;
    fAspect /= 2.f;

    _vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
    static_cast<CDiveDave*>(m_pPlayer)->Multiply_Scale(&vScale);
}

void CAttackSubFail::Clear()
{
}
CAttackSubFail* CAttackSubFail::Create(CGameObject* pPlayer, CDiveDaveAttack* pParentState)
{
    CAttackSubFail* pSubState = new CAttackSubFail(pPlayer, pParentState);

    return pSubState;
}


void CAttackSubFail::Free()
{
}
