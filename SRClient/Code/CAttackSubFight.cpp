#include "CAttackSubFight.h"
#include "CDiveDave.h"
#include "CDInputMgr.h"
#include "CCameraMgr.h"
#include "CDiveDaveCam.h"
#include "CDiveDaveAttack.h"
CAttackSubFight::CAttackSubFight(CDiveDaveAttack* pParentState)
    : CAttackSubState(pParentState)
{
    m_pDiveDave = m_pOwner->Get_OwnerDave();
}

CAttackSubFight::~CAttackSubFight()
{
}

void CAttackSubFight::Enter()
{
    m_pDiveDave->Init_Frame();
    _float fWidth = 47.f;
    _float fHeight = 54.f;
    _float fAspect = fWidth + fHeight;
    fAspect /= 2.f;

    _vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
    m_pDiveDave->Multiply_Scale(&vScale);
    m_pDiveDave->Set_TextureCom(L"Com_AttackFightTexture");


    m_pDiveDave->Set_FishCaught(false);

    Event e;
    e.type = EVENTTYPE::ATTACK_START;
    e.value = m_pOwner->Get_OwnerDave()->Is_Flip();
    m_pOwner->Get_OwnerDave()->State_Notify(e);

    // 카메라 쉐이킹 시작
    static_cast<CDiveDaveCam*>(CCameraMgr::GetInstance()->Get_Camera(L"ChaseToPlayerCam"))->FightShakingStart(m_fAttackTime);
}

void CAttackSubFight::Input(const _float& fTimeDelta)
{
    if (CDInputMgr::GetInstance()->Mouse_Down(DIM_LB) || CDInputMgr::GetInstance()->Key_Down(DIK_SPACE))
    {
        m_fAttackGauge += 1.f;
        if (m_fAttackGauge > 10.f)
        {
            m_pDiveDave->Set_State(DIVEDAVESTATE::IDLE);
            m_pDiveDave->Set_FishCaught(true);
        }
    }

    m_fDecreaseDelay += fTimeDelta;
    m_fAttackTime -= fTimeDelta;

    if (m_fDecreaseDelay > 0.2f)
    {
        m_fDecreaseDelay = 0.f;
        m_fAttackGauge -= 0.6f;

        if (m_fAttackGauge < 0.f || m_fAttackTime < 0.f)
        {
            m_pDiveDave->Set_FishCaught(false);
            m_pOwner->Set_State(ATTACKSUBSTATE::ATTACK_FAIL);
        }
    }

    ImGui::Begin("AttackGauge");
    ImGui::Text("m_fAttackGauge : %f", m_fAttackGauge);
    ImGui::End();
}

_int CAttackSubFight::Update_State(const _float& fTimeDelta)
{
    if (m_pDiveDave->Get_State() != DIVEDAVESTATE::ATTACK)
        return 0;

    Event e;
    e.type = EVENTTYPE::ATTACK_GAUGE_CHANGE;
    e.fValue = m_fAttackGauge / 10.f;
    m_pOwner->Get_OwnerDave()->State_Notify(e);

    Input(fTimeDelta);
    m_pDiveDave->AddFrame(fTimeDelta, 10.f, 8);
    //Mouse_Check();

    return 0;
}

void CAttackSubFight::LateUpdate_State(const _float& fTimeDelta)
{
    if (m_pDiveDave->Get_State() != DIVEDAVESTATE::ATTACK)
        return;
}

void CAttackSubFight::Render_State()
{
    if (m_pDiveDave->Get_State() != DIVEDAVESTATE::ATTACK)
        return;

    CTexture* pPlayerTextureCom = m_pDiveDave->Get_TextureCom();

    _float fFrame = m_pDiveDave->Get_Frame();

    pPlayerTextureCom->Set_Texture((_uint)fFrame);
}

void CAttackSubFight::Exit()
{
    _float fWidth = 47.f;
    _float fHeight = 54.f;
    _float fAspect = fWidth + fHeight;
    fAspect /= 2.f;

    _vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
    m_pDiveDave->Multiply_Scale(&vScale);

    Clear();
}

void CAttackSubFight::Clear()
{
    m_fAttackGauge = 2.f;
    m_fDecreaseDelay = 0.f;
    m_fAttackTime = 5.f;

    Event e;
    e.type = EVENTTYPE::ATTACK_END;
    m_pOwner->Get_OwnerDave()->State_Notify(e);

    // 카메라 쉐이킹 강종
    static_cast<CDiveDaveCam*>(CCameraMgr::GetInstance()->Get_Camera(L"ChaseToPlayerCam"))->FightShakingEnd();
}

CAttackSubFight* CAttackSubFight::Create(CDiveDaveAttack* pParentState)
{
    CAttackSubFight* pSubState = new CAttackSubFight(pParentState);

    return pSubState;
}

void CAttackSubFight::Free()
{
}
