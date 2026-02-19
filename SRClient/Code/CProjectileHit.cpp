#include "CProjectileHit.h"
#include "CHarpoonProjectile.h"
#include "CDiveDave.h"
CProjectileHit::CProjectileHit(CGameObject* pOwner)
    : CPlayerState(pOwner)
{
}

CProjectileHit::~CProjectileHit()
{
}

void CProjectileHit::Enter()
{
}

void CProjectileHit::Input(const _float& fTimeDelta)
{
}

_int CProjectileHit::Update_State(const _float& fTimeDelta)
{
    CHarpoonProjectile* pProjectile = static_cast<CHarpoonProjectile*>(m_pPlayer);
    if (static_cast<CDiveDave*>(pProjectile->m_pParentGameObject)->Get_State() != DiveState::ATTACK)
    {
        pProjectile->Set_State(PROJECTILESTATE::RETURN);
    }
    return 0;
}

void CProjectileHit::LateUpdate_State(const _float& fTimeDelta)
{
}

void CProjectileHit::Render_State()
{
}

void CProjectileHit::Exit()
{
}

void CProjectileHit::Clear()
{
}

CProjectileHit* CProjectileHit::Create(CGameObject* pOwner)
{
    CProjectileHit* pState = new CProjectileHit(pOwner);

    return pState;
}

void CProjectileHit::Free()
{
}
