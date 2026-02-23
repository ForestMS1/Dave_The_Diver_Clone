#include "CProjectileHit.h"
#include "CDiveDave.h"
#include "CHarpoonProjectile.h"
CProjectileHit::CProjectileHit(CHarpoonProjectile* pOwner)
    : CBaseState<CHarpoonProjectile>(pOwner)
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
    CHarpoonProjectile* pProjectile = static_cast<CHarpoonProjectile*>(m_pOwner);
    if (static_cast<CDiveDave*>(pProjectile->m_pParentGameObject)->Get_State() != DIVEDAVESTATE::ATTACK)
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

CProjectileHit* CProjectileHit::Create(CHarpoonProjectile* pOwner)
{
    CProjectileHit* pState = new CProjectileHit(pOwner);

    return pState;
}

void CProjectileHit::Free()
{
}
