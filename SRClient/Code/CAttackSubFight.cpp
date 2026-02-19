#include "CAttackSubFight.h"
#include "CDiveDave.h"
#include "CDInputMgr.h"
#include "CCameraMgr.h"
#include "CDiveDaveCam.h"
#include "CDiveDaveAttack.h"
CAttackSubFight::CAttackSubFight(CGameObject* pPlayer, CDiveDaveAttack* pParentState)
    : CAttackSubState(pPlayer, pParentState)
{
}

CAttackSubFight::~CAttackSubFight()
{
}

void CAttackSubFight::Enter()
{
    static_cast<CDiveDave*>(m_pPlayer)->Init_Frame();
    _float fWidth = 47.f;
    _float fHeight = 54.f;
    _float fAspect = fWidth + fHeight;
    fAspect /= 2.f;

    _vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
    static_cast<CDiveDave*>(m_pPlayer)->Multiply_Scale(&vScale);
    static_cast<CDiveDave*>(m_pPlayer)->Set_TextureCom(L"Com_AttackFightTexture");


    static_cast<CDiveDave*>(m_pPlayer)->Set_FishCaught(false);
}

void CAttackSubFight::Input(const _float& fTimeDelta)
{
    if (CDInputMgr::GetInstance()->Mouse_Down(DIM_LB))
    {
        m_fAttackGauge += 1.f;
        if (m_fAttackGauge > 10.f)
        {
            static_cast<CDiveDave*>(m_pPlayer)->Set_State(DiveState::IDLE);
            static_cast<CDiveDave*>(m_pPlayer)->Set_FishCaught(true);
        }
    }

    m_fDecreaseDelay += fTimeDelta;

    if (m_fDecreaseDelay > 0.2f)
    {
        m_fDecreaseDelay = 0.f;
        m_fAttackGauge -= 0.6f;

        if (m_fAttackGauge < 0.f)
            m_pParentState->Set_State(ATTACKSUBSTATE::ATTACK_FAIL);
    }

    ImGui::Begin("AttackGauge");
    ImGui::Text("m_fAttackGauge : %f", m_fAttackGauge);
    ImGui::End();
}

_int CAttackSubFight::Update_State(const _float& fTimeDelta)
{
    if (static_cast<CDiveDave*>(m_pPlayer)->Get_State() != DiveState::ATTACK)
        return 0;

    Input(fTimeDelta);
    static_cast<CDiveDave*>(m_pPlayer)->AddFrame(fTimeDelta, 10.f, 8);
    //Mouse_Check();

    return 0;
}

void CAttackSubFight::LateUpdate_State(const _float& fTimeDelta)
{
    if (static_cast<CDiveDave*>(m_pPlayer)->Get_State() != DiveState::ATTACK)
        return;
}

void CAttackSubFight::Render_State()
{
    if (static_cast<CDiveDave*>(m_pPlayer)->Get_State() != DiveState::ATTACK)
        return;

    CTexture* pPlayerTextureCom = static_cast<CDiveDave*>(m_pPlayer)->Get_TextureCom();

    _float fFrame = static_cast<CDiveDave*>(m_pPlayer)->Get_Frame();

    pPlayerTextureCom->Set_Texture((_uint)fFrame);
}

void CAttackSubFight::Exit()
{
    _float fWidth = 47.f;
    _float fHeight = 54.f;
    _float fAspect = fWidth + fHeight;
    fAspect /= 2.f;

    _vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
    static_cast<CDiveDave*>(m_pPlayer)->Multiply_Scale(&vScale);

    Clear();
}

void CAttackSubFight::Clear()
{
    m_fAttackGauge = 2.f;
    m_fDecreaseDelay = 0.f;
}

CAttackSubFight* CAttackSubFight::Create(CGameObject* pPlayer, CDiveDaveAttack* pParentState)
{
    CAttackSubFight* pSubState = new CAttackSubFight(pPlayer, pParentState);

    return pSubState;
}

void CAttackSubFight::Free()
{
}
