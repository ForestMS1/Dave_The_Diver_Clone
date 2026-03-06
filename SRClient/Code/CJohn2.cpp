#include "CJohn2.h"
#include "CRenderer.h"
#include "CGraphicDev.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CSelector.h"
#include "CSequence.h"
#include "CActionNode.h"
#include "CManagement.h"
#include "CTimerMgr.h"
#include "CDiveDaveBullet.h"
#include "CColliderMgr.h"
#include "CJohnMine.h"
#include "CDiveDave.h"
#include "CJohn2Idle.h"
#include "CJohn2NoStart.h"
#include "CJohn2Intro.h"
#include "CParticleMgr.h"
#include "CJohn2AttackReady.h"
#include "CJohn2Chase.h"
#include "CJohn2AttackShoot.h"
#include "CJohnGuidedBullet.h"
#include "CGunSkill02Aim.h"
#include "CJohn2MeleeAttackReady.h"
#include "CJohn2MeleeAttack.h"
#include "CJohn2MeleeAttackReturn.h"
#include "CGameMemMgr.h"
#include "CJohn2Hit.h"

CJohn2::CJohn2(_float x, _float y, _float z)
	: m_vCreatePos({ x,y,z })
{
}

CJohn2::CJohn2(const CJohn2& rhs)
	:CGameObject(rhs)
{
}

CJohn2::~CJohn2()
{
}

HRESULT CJohn2::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;
	if (FAILED(Add_State()))
		return E_FAIL;

	_vec3 vScale = { 1.25f, 1.25f, 1.f };
	m_pTransformCom->Multiply_Scale(&vScale);

	Set_State(JOHN2STATE::BEFORE_START);

	m_pTransformCom->Set_Pos(m_vCreatePos.x, m_vCreatePos.y, m_vCreatePos.z);


	//-------------AABB Collider----------------
	_vec3 vExtents = { 1.0f, 1.0f, 1.0f };

	_vec3 vPos = { 0.0f, 0.0f, 0.0f };

	m_pAABB = CAABB::Create(&vPos, &vExtents, L"AABB_JohnWithGuided", this);

	return S_OK;
}

_int CJohn2::Update_GameObject(const _float& fTimeDelta)
{
	CJohn2::Start();

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

void CJohn2::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);

	m_pFSM->LateUpdate_State(fTimeDelta);

}

void CJohn2::Render_GameObject()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pFSM->Render_State();

	m_pBufferCom->Render_Buffer();

	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CJohn2::Ready_Component()
{
	// 버퍼
	if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
		return E_FAIL;

	// 트랜스폼
	if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT	CJohn2::Add_State()
{
	m_pFSM = CFSM<CJohn2, JOHN2STATE>::Create(this);
	if (m_pFSM == nullptr)
		return E_FAIL;

	m_pFSM->Add_State<CJohn2NoStart>(JOHN2STATE::BEFORE_START);
	m_pFSM->Add_State<CJohn2Idle>(JOHN2STATE::IDLE);
	m_pFSM->Add_State<CJohn2Chase>(JOHN2STATE::CHASE);
	m_pFSM->Add_State<CJohn2AttackReady>(JOHN2STATE::ATTACK_READY);
	m_pFSM->Add_State<CJohn2AttackShoot>(JOHN2STATE::SHOT);
	m_pFSM->Add_State<CJohn2MeleeAttackReady>(JOHN2STATE::MELEEATTACK_READY);
	m_pFSM->Add_State<CJohn2MeleeAttack>(JOHN2STATE::MELEEATTACK);
	m_pFSM->Add_State<CJohn2MeleeAttackReturn>(JOHN2STATE::MELEEATTACK_RETURN);
	m_pFSM->Add_State<CJohn2Hit>(JOHN2STATE::HIT);
	m_pFSM->Add_State<CGunSkill02Aim>(JOHN2STATE::SPLASH_READY);

	return S_OK;
}

void CJohn2::AddFrame(const _float& fTimeDelta, const _float& fSpeed, _uint size, _bool loop)
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

CJohn2* CJohn2::Create(_float x, _float y, _float z)
{
	CJohn2* pBoss = new CJohn2(x, y, z);
	if (FAILED(pBoss->Ready_GameObject()))
	{
		Safe_Release(pBoss);
		MSG_BOX("CJohn2 Create Failed");
		return nullptr;
	}
	return pBoss;

}

void CJohn2::Free()
{
	Safe_Release(m_pAABB);
	Safe_Release(m_pFSM);
	CGameObject::Free();
}

void CJohn2::Start()
{
	if (m_bInitComplete)
		return;

	m_bInitComplete = true;
	m_pTargetTransform = static_cast<CTransform*>
		(CManagement::GetInstance()->Get_FirstObjectComponent(ID_DYNAMIC, L"0_GameLogic_Layer", L"DiveDave", L"Com_Transform"));
}

_bool CJohn2::Check_TargetInRange(_float fRange)
{
	// 범위 내에 들어왔음
	if (D3DXVec3Length(&m_vDirToTarget) < fRange)
		return true;

	return false;
}

void CJohn2::Update_ToTargetDir()
{
	if (m_pTargetTransform == nullptr)
		return;
	_vec3 vCurPos, vTargetPos;
	m_pTransformCom->Get_Info(INFO_POS, &vCurPos);
	m_pTargetTransform->Get_Info(INFO_POS, &vTargetPos);

	m_vDirToTarget = vTargetPos - vCurPos;
}

void CJohn2::Shot_Bullet()
{
	_vec3 vCurPos, vNorToTarget;
	m_pTransformCom->Get_Info(INFO_POS, &vCurPos);
	D3DXVec3Normalize(&vNorToTarget, &m_vDirToTarget);

	_vec3 vAxisX = { 1.f, 0.f, 0.f };

	_float dot = vAxisX.x * vNorToTarget.x + vAxisX.y * vNorToTarget.y;
	_float cross = vAxisX.x * vNorToTarget.y - vAxisX.y * vNorToTarget.x;
	_float fAngle = D3DXToDegree(atan2(cross, dot));

	CJohnGuidedBullet* pBullet = CJohnGuidedBullet::Create(vCurPos, vNorToTarget, fAngle, this);
	CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Add_GameObject(L"JohnGuidedBullet", pBullet);
}

void CJohn2::Splash_Mine()
{
	//_vec3 vCurPos, vNorToTarget;
	//m_pTransformCom->Get_Info(INFO_POS, &vCurPos);
	//D3DXVec3Normalize(&vNorToTarget, &m_vDirToTarget);


	//CJohn2Mine* pBullet = CJohn2Mine::Create(vCurPos, vNorToTarget);
	//CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Add_GameObject(L"JohnMine", pBullet);

	_vec3 vCurPos;
	m_pTransformCom->Get_Info(INFO_POS, &vCurPos);

	const int iBulletCount = 12;

	for (int i = 0; i < iBulletCount; ++i)
	{
		float fAngle = ((float)rand() / RAND_MAX) * D3DX_PI * 2.f;
	
		_vec3 vDir;
		vDir.x = cosf(fAngle);
		vDir.y = sinf(fAngle);
		vDir.z = 0.f;
	
		D3DXVec3Normalize(&vDir, &vDir);
	
		CJohnMine* pBullet = CJohnMine::Create(vCurPos, vDir);
		CManagement::GetInstance()->Get_Scene()
			->Get_Layer(L"0_GameLogic_Layer")
			->Add_GameObject(L"JohnMine", pBullet);
	}
}
_bool CJohn2::Rush_ToTarget(const _float& fTimeDelta)
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


	m_pTransformCom->Move_Pos(&vDir, 20.f, fTimeDelta);
	m_fAccRushDist += fTimeDelta * 20.f;

	if (m_fAccRushDist > 7.f)
	{
		m_fAccRushDist = 0.f;
		m_bRushStart = false;
		return true;
	}
	return false;
}

void CJohn2::EncounterTarget()
{
	if (m_bStartCombat)
		return;
	m_bStartCombat = true;

	CJohn2Intro* pIntro = CJohn2Intro::Create();
	CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_UI_Layer")->Add_GameObject(L"JW2_Intro", pIntro);
}

void CJohn2::CollisionWithTarget()
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

_bool CJohn2::Check_GlobalState()
{
	if (m_bIsDie)
	{
		m_pFSM->Set_State(JOHN2STATE::DIE);
		return true;
	}

	if (m_bIsHit)
	{
		m_pFSM->Set_State(JOHN2STATE::HIT);

		return true;
	}

	return false;
}

void CJohn2::Move(_vec3* vDir, const _float& fTimeDelta)
{
	m_pTransformCom->Move_Pos(vDir, m_fSpeed, fTimeDelta);
	// 테스트
	_vec3 Pos{};
	m_pTransformCom->Get_Info(INFO_POS, &Pos);
	//CParticleMgr::GetInstance()->spwan_Particle(PARTICLE_BUBBLE, Pos, 4);
	//CParticleMgr::GetInstance()->spwan_Particle(PARTICLE_BLOOD, Pos, 4);
}

void CJohn2::On_Dead() {
	 m_bIsDie = true; 
	 if (!CGameMemMgr::GetInstance()->Get_BossKilled()) {
		 CGameMemMgr::GetInstance()->Set_BossKilled(true);
	}
}