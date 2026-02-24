#include "CAttackSubFail.h"
#include "CDiveDave.h"
#include "CDInputMgr.h"
#include "CCameraMgr.h"
#include "CDiveDaveCam.h"
#include "CDiveDaveAttack.h"
CAttackSubFail::CAttackSubFail(CDiveDaveAttack* pParentState)
    : CAttackSubState(pParentState)
{
    m_pDiveDave = m_pOwner->Get_OwnerDave();
}

CAttackSubFail::~CAttackSubFail()
{
}

void CAttackSubFail::Enter()
{
    m_pDiveDave->Init_Frame();
    _float fWidth = 47.f;
    _float fHeight = 54.f;
    _float fAspect = fWidth + fHeight;
    fAspect /= 2.f;

    _vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
    m_pDiveDave->Multiply_Scale(&vScale);
    m_pDiveDave->Set_TextureCom(L"Com_AttackFailTexture");
}

void CAttackSubFail::Input(const _float& fTimeDelta)
{
}

_int CAttackSubFail::Update_State(const _float& fTimeDelta)
{
    if (m_pDiveDave->Get_State() != DIVEDAVESTATE::ATTACK)
        return 0;

    Input(fTimeDelta);
    m_pDiveDave->AddFrame(fTimeDelta, 10.f, 1);
    //Mouse_Check();
    m_fFailDelay += fTimeDelta;
    if (m_fFailDelay < 1.f)
        m_pDiveDave->Set_State(DIVEDAVESTATE::IDLE);

    return 0;
}

void CAttackSubFail::LateUpdate_State(const _float& fTimeDelta)
{
    if (m_pDiveDave->Get_State() != DIVEDAVESTATE::ATTACK)
        return;
}

void CAttackSubFail::Render_State()
{
    if (m_pDiveDave->Get_State() != DIVEDAVESTATE::ATTACK)
        return;

    CTexture* pPlayerTextureCom = m_pDiveDave->Get_TextureCom();

    _float fFrame = m_pDiveDave->Get_Frame();

    pPlayerTextureCom->Set_Texture((_uint)fFrame);
}

void CAttackSubFail::Exit()
{
    _float fWidth = 47.f;
    _float fHeight = 54.f;
    _float fAspect = fWidth + fHeight;
    fAspect /= 2.f;

    _vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
    m_pDiveDave->Multiply_Scale(&vScale);
}

void CAttackSubFail::Clear()
{
}
CAttackSubFail* CAttackSubFail::Create(CDiveDaveAttack* pParentState)
{
    CAttackSubFail* pSubState = new CAttackSubFail(pParentState);

    return pSubState;
}


void CAttackSubFail::Free()
{
}
