#include "CJohn.h"
#include "CRenderer.h"
#include "CGraphicDev.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CJohnIdle.h"
#include "CSelector.h"
#include "CSequence.h"
#include "CActionNode.h"
#include "CManagement.h"
#include "CTimerMgr.h"
#include "CJohnChase.h"
#include "CJohnAttackReady.h"
#include "CJohnAttackShoot.h"
#include "CDiveDaveBullet.h"
#include "CJohnMeleeAttackReady.h"
#include "CJohnMeleeAttack.h"
#include "CJohnIntro.h"
#include "CJohnGuidedBullet.h"
#include "CColliderMgr.h"
#include "CJohnNoStart.h"
#include "CJohnHit.h"
#include "CDiveDave.h"
CJohn::CJohn(_float x, _float y, _float z)
	: m_vCreatePos({x,y,z})
{
}

CJohn::CJohn(const CJohn& rhs)
	:CGameObject(rhs)
{
}

CJohn::~CJohn()
{
}

HRESULT CJohn::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;
	if (FAILED(Add_State()))
		return E_FAIL;

	_vec3 vScale = { 0.5f, 0.5f, 1.f };
	m_pTransformCom->Multiply_Scale(&vScale);

	Set_State(JOHNSTATE::BEFORE_START);

	m_pTransformCom->Set_Pos(m_vCreatePos.x, m_vCreatePos.y, m_vCreatePos.z);

	
	//-------------AABB Collider----------------
	_vec3 vExtents = { 1.0f, 1.0f, 1.0f };

	_vec3 vPos = { 0.0f, 0.0f, 0.0f };

	m_pAABB = CAABB::Create(&vPos, &vExtents, L"AABB_JohnWithGuided", this);

	return S_OK;
}

_int CJohn::Update_GameObject(const _float& fTimeDelta)
{
	CJohn::Start();

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
	// 충돌체 그룹에 넣어줘야한다.
	CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_JohnWithGuided", m_pAABB);
	m_pAABB->Transform(m_pTransformCom->Get_World());

	CGameObject::Update_GameObject(fTimeDelta);

	_bool isGlobalState = Check_GlobalState();

	m_pFSM->Update_State(fTimeDelta);

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	Compute_ViewZ(&vPos);

	return 0;
}

void CJohn::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);

	m_pFSM->LateUpdate_State(fTimeDelta);

}

void CJohn::Render_GameObject()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pFSM->Render_State();

	m_pBufferCom->Render_Buffer();

	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CJohn::Ready_Component()
{
	// 버퍼
	if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
		return E_FAIL;

	// 트랜스폼
	if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT	CJohn::Add_State()
{
	m_pFSM = CFSM<CJohn, JOHNSTATE>::Create(this);
	if (m_pFSM == nullptr)
		return E_FAIL;

	m_pFSM->Add_State<CJohnIdle>(JOHNSTATE::IDLE);
	m_pFSM->Add_State<CJohnChase>(JOHNSTATE::CHASE);
	m_pFSM->Add_State<CJohnAttackReady>(JOHNSTATE::ATTACK_READY);
	m_pFSM->Add_State<CJohnAttackShoot>(JOHNSTATE::SHOT);
	m_pFSM->Add_State<CJohnMeleeAttackReady>(JOHNSTATE::MELEEATTACK_READY);
	m_pFSM->Add_State<CJohnMeleeAttack>(JOHNSTATE::MELEEATTACK);
	m_pFSM->Add_State<CJohnNoStart>(JOHNSTATE::BEFORE_START);
	m_pFSM->Add_State<CJohnHit>(JOHNSTATE::HIT);

	return S_OK;
}

void CJohn::AddFrame(const _float& fTimeDelta, const _float& fSpeed, _uint size, _bool loop)
{
	m_fFrame += fSpeed * fTimeDelta;
	if (loop)
	{
		if (m_fFrame > size)
			m_fFrame = 0.f;
	}
	else
	{
		if (m_fFrame > size)
			m_fFrame = size - 1;
	}
}

CJohn* CJohn::Create(_float x, _float y, _float z)
{
	CJohn* pBoss = new CJohn(x,y,z);
	if (FAILED(pBoss->Ready_GameObject()))
	{
		Safe_Release(pBoss);
		MSG_BOX("CJohn Create Failed");
		return nullptr;
	}
	return pBoss;

}

void CJohn::Free()
{
	Safe_Release(m_pAABB);
	Safe_Release(m_pFSM);
	CGameObject::Free();
}

void CJohn::Start()
{
	if (m_bInitComplete)
		return;

	m_bInitComplete = true;
	m_pTargetTransform = static_cast<CTransform*>
		(CManagement::GetInstance()->Get_FirstObjectComponent(ID_DYNAMIC, L"0_GameLogic_Layer", L"DiveDave", L"Com_Transform"));
}

_bool CJohn::Check_TargetInRange(_float fRange)
{
	// 범위 내에 들어왔음
	if (D3DXVec3Length(&m_vDirToTarget) < fRange)
		return true;

	return false;
}

void CJohn::Update_ToTargetDir()
{
	if (m_pTargetTransform == nullptr)
		return;
	_vec3 vCurPos, vTargetPos;
	m_pTransformCom->Get_Info(INFO_POS, &vCurPos);
	m_pTargetTransform->Get_Info(INFO_POS, &vTargetPos);

	m_vDirToTarget = vTargetPos - vCurPos;
}

void CJohn::Shot_Bullet()
{
	_vec3 vCurPos, vNorToTarget;
	m_pTransformCom->Get_Info(INFO_POS, &vCurPos);
	D3DXVec3Normalize(&vNorToTarget, &m_vDirToTarget);

	_vec3 vAxisX = { 1.f, 0.f, 0.f };
	
	_float dot = vAxisX.x * vNorToTarget.x + vAxisX.y * vNorToTarget.y;
	_float cross = vAxisX.x * vNorToTarget.y - vAxisX.y * vNorToTarget.x;
	_float fAngle = D3DXToDegree(atan2(cross, dot));

	CJohnGuidedBullet* pBullet = CJohnGuidedBullet::Create(vCurPos, vNorToTarget, fAngle);
	CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Add_GameObject(L"JohnBullet", pBullet);
}

_bool CJohn::Rush_ToTarget(const _float& fTimeDelta)
{
	_vec3 vDir;
	D3DXVec3Normalize(&vDir, &m_vDirToTarget);

	if (!m_bRushStart)
	{
		_vec3 vRotDir;
		if (Get_ToTargetDir().x > 0.f)
			vRotDir = { 0.f, 0.f, 0.f };
		else
			vRotDir = { 0.f,-180.f, 0.f };
		Set_RotateDir(&vRotDir);
		m_bRushStart = true;
	}


	m_pTransformCom->Move_Pos(&vDir, 5.f, fTimeDelta);
	m_fAccRushDist += fTimeDelta * 5.f;

	if (m_fAccRushDist > 7.f)
	{
		m_fAccRushDist = 0.f;
		m_bRushStart = false;
		return true;
	}
	return false;
}

void CJohn::EncounterTarget()
{
	if (m_bStartCombat)
		return;
	m_bStartCombat = true;

	CJohnIntro* pIntro = CJohnIntro::Create();
	CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_UI_Layer")->Add_GameObject(L"JW_Intro", pIntro);
}

void CJohn::CollisionWithTarget()
{
	if (auto pColliders = CColliderMgr::GetInstance()->Get_Colliders(L"Coll_DiveDaveWithItemBox"))
	{
		for (auto& pCollider : *pColliders)
		{
			if (m_pAABB->Intersect(pCollider))
			{
				if (pCollider->Get_Tag() == L"AABB_DiveDaveWithItemBox")
				{
					reinterpret_cast<CDiveDave*>(pCollider->Get_VoidPtr())->On_Hit(10.f);
				}
			}
		}
	}
}

_bool CJohn::Check_GlobalState()
{
	if (m_bIsDie)
	{
		m_pFSM->Set_State(JOHNSTATE::DIE);
		return true;
	}

	if (m_bIsHit)
	{
		m_pFSM->Set_State(JOHNSTATE::HIT);

		return true;
	}

	return false;
}
