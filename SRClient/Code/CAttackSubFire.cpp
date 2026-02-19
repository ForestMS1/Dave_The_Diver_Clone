#include "CAttackSubFire.h"
#include "CDiveDave.h"
#include "CDInputMgr.h"
#include "CCameraMgr.h"
#include "CDiveDaveCam.h"
#include "CDiveDaveAttack.h"
#include "CManagement.h"
#include "CHarpoonProjectile.h"
CAttackSubFire::CAttackSubFire(CGameObject* pPlayer, CDiveDaveAttack* pParentState)
    :CAttackSubState(pPlayer, pParentState)
{
}

CAttackSubFire::~CAttackSubFire()
{
}

void CAttackSubFire::Enter()
{
    static_cast<CDiveDave*>(m_pPlayer)->Init_Frame();
    _float fWidth = 37.f;
    _float fHeight = 55.f;
    _float fAspect = fWidth + fHeight;
    fAspect /= 2.f;

    _vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
    static_cast<CDiveDave*>(m_pPlayer)->Multiply_Scale(&vScale);
    static_cast<CDiveDave*>(m_pPlayer)->Set_TextureCom(L"Com_AttackFireTexture");


    // น฿ป็
    CHarpoonProjectile* pProjectile = static_cast<CHarpoonProjectile*>
        (CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Get_GameObjectFirst(L"HarpoonProjectile"));
    pProjectile->TriggerOn();
}

void CAttackSubFire::Input(const _float& fTimeDelta)
{
}

_int CAttackSubFire::Update_State(const _float& fTimeDelta)
{
    if (static_cast<CDiveDave*>(m_pPlayer)->Get_State() != DiveState::ATTACK)
        return 0;

    Input(fTimeDelta);
    static_cast<CDiveDave*>(m_pPlayer)->AddFrame(fTimeDelta, 10.f, 1);
    //Mouse_Check();

    return 0;
}

void CAttackSubFire::LateUpdate_State(const _float& fTimeDelta)
{
    if (static_cast<CDiveDave*>(m_pPlayer)->Get_State() != DiveState::ATTACK)
        return;


    CHarpoonProjectile* pHarpoonProjectile = dynamic_cast<CHarpoonProjectile*>
        (CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Get_GameObjectFirst(L"HarpoonProjectile"));

    if (pHarpoonProjectile->GetProjectilState() == PROJECTILESTATE::HIT)
        m_pParentState->Set_State(ATTACKSUBSTATE::ATTACK_FIGHT);
}

void CAttackSubFire::Render_State()
{
    if (static_cast<CDiveDave*>(m_pPlayer)->Get_State() != DiveState::ATTACK)
        return;

    CTexture* pPlayerTextureCom = static_cast<CDiveDave*>(m_pPlayer)->Get_TextureCom();

    _float fFrame = static_cast<CDiveDave*>(m_pPlayer)->Get_Frame();

    pPlayerTextureCom->Set_Texture((_uint)fFrame);
}

void CAttackSubFire::Exit()
{
    _float fWidth = 37.f;
    _float fHeight = 55.f;
    _float fAspect = fWidth + fHeight;
    fAspect /= 2.f;

    _vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
    static_cast<CDiveDave*>(m_pPlayer)->Multiply_Scale(&vScale);
}

void CAttackSubFire::Clear()
{
}
CAttackSubFire* CAttackSubFire::Create(CGameObject* pPlayer, CDiveDaveAttack* pParentState)
{
    CAttackSubFire* pSubState = new CAttackSubFire(pPlayer, pParentState);

    return pSubState;
}


void CAttackSubFire::Free()
{
}
