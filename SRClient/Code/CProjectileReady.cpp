#include "CProjectileReady.h"
#include "CHelper.h"
#include "CHarpoonProjectile.h"
CProjectileReady::CProjectileReady(CGameObject* pOwner)
    : CPlayerState(pOwner)
{
}

CProjectileReady::~CProjectileReady()
{
}

void CProjectileReady::Enter()
{
}

void CProjectileReady::Input(const _float& fTimeDelta)
{
}

_int CProjectileReady::Update_State(const _float& fTimeDelta)
{
	Set_ParentTransform();
	Rotate_ToMouse();
	Shot_ToMouse();

    return 0;
}

void CProjectileReady::LateUpdate_State(const _float& fTimeDelta)
{
}

void CProjectileReady::Render_State()
{
}

void CProjectileReady::Exit()
{
}

void CProjectileReady::Clear()
{
}

CProjectileReady* CProjectileReady::Create(CGameObject* pOwner)
{
    CProjectileReady* pState = new CProjectileReady(pOwner);
    
    return pState;
}

void CProjectileReady::Free()
{
}


void CProjectileReady::Set_ParentTransform()
{
	_vec3 vParentPos;
	m_pPlayer->Get_Parent()->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &vParentPos);
	_vec3 vOffSet = { 0.f, 0.5f, 0.f };
	vOffSet.y *= m_pPlayer->Get_Parent()->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->m_vScale.y;
	vParentPos += vOffSet;
	m_pPlayer->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Set_Pos(vParentPos.x, vParentPos.y, vParentPos.z);
}

void CProjectileReady::Rotate_ToMouse()
{
	_vec3 vMousePos, vPlayerPos;

	CHelper::GetMousePointInWorld(&vMousePos);
	m_pPlayer->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &vPlayerPos);

	_vec3 vDir = vMousePos - vPlayerPos;

	float fLengthSq = vDir.x * vDir.x + vDir.y * vDir.y;

	// 너무 가까우면 회전 유지
	if (fLengthSq < 0.001f)
		return;

	float fDegree = D3DXToDegree(atan2f(vDir.y, vDir.x));

	if (vDir.x < 0.f)
		m_pPlayer->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->m_vAngle.x = 180.f;
	else
		m_pPlayer->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->m_vAngle.x = 0.f;

	m_pPlayer->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->m_vAngle.z = fDegree;
}

void CProjectileReady::Shot_ToMouse()
{
	_vec3 vMousePos, vProjectilePos;

	CHelper::GetMousePointInWorld(&vMousePos);
	m_pPlayer->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &vProjectilePos);

	*static_cast<CHarpoonProjectile*>(m_pPlayer)->Get_Dir() = vMousePos - vProjectilePos;
	D3DXVec3Normalize(static_cast<CHarpoonProjectile*>(m_pPlayer)->Get_Dir(), static_cast<CHarpoonProjectile*>(m_pPlayer)->Get_Dir());
}