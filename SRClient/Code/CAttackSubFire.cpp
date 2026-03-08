#include "CAttackSubFire.h"
#include "CDiveDave.h"
#include "CDInputMgr.h"
#include "CCameraMgr.h"
#include "CDiveDaveCam.h"
#include "CDiveDaveAttack.h"
#include "CManagement.h"
#include "CHarpoonProjectile.h"
#include "CDiveDaveGun.h"
#include "CSoundMgr.h"
CAttackSubFire::CAttackSubFire(CDiveDaveAttack* pParentState)
    : CAttackSubState(pParentState)
{
    m_pDiveDave = m_pOwner->Get_OwnerDave();
}

CAttackSubFire::~CAttackSubFire()
{
}

void CAttackSubFire::Enter()
{
    m_pDiveDave->Init_Frame();
    _float fWidth = 37.f;
    _float fHeight = 55.f;
    _float fAspect = fWidth + fHeight;
    fAspect /= 2.f;

    _vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
    m_pDiveDave->Multiply_Scale(&vScale);
    m_pDiveDave->Set_TextureCom(L"Com_AttackFireTexture");


    // น฿ป็
    if (m_pDiveDave->Get_CurEquipped() == EQUIPPED::HARPOON)
    {
        CHarpoonProjectile* pProjectile = static_cast<CHarpoonProjectile*>
            (CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Get_GameObjectFirst(L"HarpoonProjectile"));
        pProjectile->TriggerOn();
        CSoundMgr::GetInstance()->PlaySoundOne(L"Sound_Harpoon_Shot", CSoundMgr::SFX, 1.f);
    }
    else if (m_pDiveDave->Get_CurEquipped() == EQUIPPED::GUN)
    {
        CDiveDaveGun* pGun = static_cast<CDiveDaveGun*>
            (CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Get_GameObjectFirst(L"DiveDaveGun"));
        pGun->Fire();
    }

    m_fReboundTime = 0.f;
}

void CAttackSubFire::Input(const _float& fTimeDelta)
{
}

_int CAttackSubFire::Update_State(const _float& fTimeDelta)
{
    if (m_pDiveDave->Get_State() != DIVEDAVESTATE::ATTACK)
        return 0;


    if (m_fReboundTime <= 0.5f)
    {
        m_fReboundTime += fTimeDelta;
		_vec3 vReboundDir = m_pDiveDave->Is_Flip() ? _vec3{ 1.f, 0.f, 0.f } : _vec3{ -1.f, 0.f, 0.f };
        m_pDiveDave->Move(&vReboundDir, fTimeDelta, 1.f);
    }

    Input(fTimeDelta);
    m_pDiveDave->AddFrame(fTimeDelta, 10.f, 1);
    //Mouse_Check();

    return 0;
}

void CAttackSubFire::LateUpdate_State(const _float& fTimeDelta)
{
    if (m_pDiveDave->Get_State() != DIVEDAVESTATE::ATTACK)
        return;


    if (m_pDiveDave->Get_CurEquipped() == EQUIPPED::HARPOON)
    {
        if(m_pDiveDave->Is_FishCaught())
            m_pOwner->Set_State(ATTACKSUBSTATE::ATTACK_FIGHT);
    }
}

void CAttackSubFire::Render_State()
{
    if (m_pDiveDave->Get_State() != DIVEDAVESTATE::ATTACK)
        return;

    CTexture* pPlayerTextureCom = m_pDiveDave->Get_TextureCom();

    _float fFrame = m_pDiveDave->Get_Frame();

    pPlayerTextureCom->Set_Texture((_uint)fFrame);
}

void CAttackSubFire::Exit()
{
    _float fWidth = 37.f;
    _float fHeight = 55.f;
    _float fAspect = fWidth + fHeight;
    fAspect /= 2.f;

    _vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
    m_pDiveDave->Multiply_Scale(&vScale);
}

void CAttackSubFire::Clear()
{
    m_fReboundTime = 0.f;
}
CAttackSubFire* CAttackSubFire::Create(CDiveDaveAttack* pParentState)
{
    CAttackSubFire* pSubState = new CAttackSubFire(pParentState);

    return pSubState;
}


void CAttackSubFire::Free()
{
}
