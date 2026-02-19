#include "CProjectileReturn.h"
#include "CHarpoonProjectile.h"
#include "CDiveDave.h"
#include "CTestFish.h"

CProjectileReturn::CProjectileReturn(CGameObject* pOwner)
    : CPlayerState(pOwner)
{
}

CProjectileReturn::~CProjectileReturn()
{
}

void CProjectileReturn::Enter()
{
}

void CProjectileReturn::Input(const _float& fTimeDelta)
{
}

_int CProjectileReturn::Update_State(const _float& fTimeDelta)
{
	Return_Act(fTimeDelta);
    return 0;
}

void CProjectileReturn::LateUpdate_State(const _float& fTimeDelta)
{
}

void CProjectileReturn::Render_State()
{
}

void CProjectileReturn::Exit()
{
	Clear();
}

void CProjectileReturn::Clear()
{
	CHarpoonProjectile* pProjectile = static_cast<CHarpoonProjectile*>(m_pPlayer);
	pProjectile->m_pCaughtFish = nullptr;
}

CProjectileReturn* CProjectileReturn::Create(CGameObject* pOwner)
{
    CProjectileReturn* pState = new CProjectileReturn(pOwner);

    return pState;
}
void CProjectileReturn::Free()
{
}

void CProjectileReturn::Return_Act(const _float& fTimeDelta)
{
	CHarpoonProjectile* pProjectile = static_cast<CHarpoonProjectile*>(m_pPlayer);

	_vec3 vCurPos, vPlayerPos;
	pProjectile->m_pTransformCom->Get_Info(INFO_POS, &vCurPos);
	static_cast<CDiveDave*>(pProjectile->m_pParentGameObject)->Get_Pos(&vPlayerPos);
	_vec3 vDiff = vPlayerPos - vCurPos;
	D3DXVec3Length(&vDiff);

	if (D3DXVec3Length(&vDiff) > 1.f)
	{
		pProjectile->m_pTransformCom->Move_Pos(&pProjectile->m_vDir, -pProjectile->m_fSpeed, fTimeDelta);

		// 플레이어가 물고기 잡기에 성공했다면 물고기 끌어당김
		if (pProjectile->m_pCaughtFish != nullptr && static_cast<CDiveDave*>(pProjectile->m_pParentGameObject)->Is_FishCaught())
			static_cast<CTestFish*>(pProjectile->m_pCaughtFish)->Pull_Fish(&pProjectile->m_vDir, -pProjectile->m_fSpeed, fTimeDelta);
	}
	else
	{
		if (pProjectile->m_pCaughtFish != nullptr && static_cast<CDiveDave*>(pProjectile->m_pParentGameObject)->Is_FishCaught())
		{
			pProjectile->m_pCaughtFish->Set_Dead();
			pProjectile->m_pCaughtFish = nullptr;
			static_cast<CDiveDave*>(pProjectile->m_pParentGameObject)->Set_FishCaught(false);
		}
		Set_ParentTransform();
		pProjectile->Set_State(PROJECTILESTATE::READY);
	}
}

void CProjectileReturn::Set_ParentTransform()
{
	_vec3 vParentPos;
	m_pPlayer->Get_Parent()->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &vParentPos);
	_vec3 vOffSet = { 0.f, 0.5f, 0.f };
	vOffSet.y *= m_pPlayer->Get_Parent()->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->m_vScale.y;
	vParentPos += vOffSet;
	m_pPlayer->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Set_Pos(vParentPos.x, vParentPos.y, vParentPos.z);
}
