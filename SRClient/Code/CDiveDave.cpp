#include "pch.h"
#include "CDiveDave.h"
#include "CDInputMgr.h"
#include "CGraphicDev.h"
#include "CRenderer.h"
#include "CPlayerState.h"
#include "CDiveDaveIdle.h"
#include "CDiveDaveMove.h"
#include "CDiveDaveAttack.h"
#include "CDiveDaveMeeleAttack.h"
#include "CAttackReadyArm.h"
#include "CDiveDaveTanning.h"
#include "CDiveDaveOpen.h"
#include "CCollisionMgr.h"
#include "CColliderMgr.h"
#include "CDiveItemBox.h"
#include "CDiveDavePickUp.h"
#include "CDiveDaveHit.h"
#include "CDiveDaveDie.h"
#include "CParticleMgr.h"
string debugState[(_uint)DIVEDAVESTATE::DAVE_STATE_END] = { "IDLE", "MOVE", "ATTACK", "MELEEATTACK", "TANNING", "OPEN", "PICKUP", "HIT", "DIE" };
string debugEquipped[(_uint)EQUIPPED::EQUIPPED_END] = {  "HARPOON", "GUN" };

CDiveDave::CDiveDave()
	: m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pTransformCom(nullptr)
{
}

CDiveDave::CDiveDave(const CDiveDave& rhs)
	: CGameObject(rhs)
{
}

CDiveDave::~CDiveDave()
{
}

HRESULT CDiveDave::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	if(FAILED(Add_State()))
		return E_FAIL;
	
	_vec3 vScale = { 0.5f, 0.5f, 1.f };
	m_pTransformCom->Multiply_Scale(&vScale);

	Set_State(DIVEDAVESTATE::IDLE);


	//-------------AABB Collider With ItemBox----------------
	_vec3 vExtents = { 1.0f, 1.0f, 1.0f };
	_vec3 vPos = { 00.0f, 0.0f, 0.0f };
	m_pAABB = CAABB::Create(&vPos, &vExtents, L"AABB_DiveDaveWithItemBox", this);
	m_pAABBItem = CAABB::Create(&vPos, &vExtents, L"AABB_DiveDaveWithItem", this);

	//CColliderMgr::GetInstance()->Set_Render(true);

	return S_OK;
}

_int CDiveDave::Update_GameObject(const _float& fTimeDelta)
{
	// 충돌체 그룹에 넣어줘야한다.
	CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_DiveDaveWithItemBox", m_pAABB);
	CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_DiveDaveWithItem", m_pAABBItem);
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
	m_pAABB->Transform(m_pTransformCom->Get_World());
	m_pAABBItem->Transform(m_pTransformCom->Get_World());

	_bool isGlobalState = Check_GlobalState();
	if (!isGlobalState)
	{
		Key_Input();
		Mouse_Input();
	}
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);
	m_pFSM->Update_State(fTimeDelta);

#ifdef _DEBUG
	ImGui::Begin("DiveDave Info");
	string state = "State : " + debugState[(_uint)CDiveDave::Get_State()];
	ImGui::Text(state.c_str());
	string Equipped = "Equipped : " + debugEquipped[(_uint)m_eCurEquipped];
	ImGui::Text(Equipped.c_str());
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	ImGui::SliderFloat3("Transform", vPos, 0.f, 0.f);
	if (ImGui::Button("OnHit"))
	{
		m_bIsHit = true;
		//테스트
		_vec3 Pos{};
		m_pTransformCom->Get_Info(INFO_POS, &Pos);
		CParticleMgr::GetInstance()->spwan_Particle(PARTICLE_BLOOD, Pos, 4);
	}

	if (ImGui::Button("OnDead"))
		m_bIsDie = true;
	ImGui::End();
#endif
	return iExit;
}

void CDiveDave::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
	m_pFSM->LateUpdate_State(fTimeDelta);

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	Compute_ViewZ(&vPos);
}

void CDiveDave::Render_GameObject()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pFSM->Render_State();

	m_pBufferCom->Render_Buffer();

	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

ATTACKSUBSTATE CDiveDave::Get_AttackSubState()
{
	if (CDiveDave::Get_State() == DIVEDAVESTATE::ATTACK)
		return dynamic_cast<CDiveDaveAttack*>(m_pFSM->Get_pState())->Get_State();
}

void CDiveDave::Move(_vec3* vDir, const _float& fTimeDelta)
{
	m_pTransformCom->Move_Pos(vDir, m_fSpeed, fTimeDelta);
	// 테스트
	_vec3 Pos{};
	m_pTransformCom->Get_Info(INFO_POS, &Pos);
	CParticleMgr::GetInstance()->spwan_Particle(PARTICLE_BUBBLE, Pos, 4);
	//CParticleMgr::GetInstance()->spwan_Particle(PARTICLE_BLOOD, Pos, 4);
}

void CDiveDave::AddFrame(const _float& fTimeDelta, const _float& fSpeed, _uint size)
{
	m_fFrame += fSpeed * fTimeDelta;
	if (m_fFrame > size)
		m_fFrame = 0.f;
}

_bool CDiveDave::Check_GlobalState()
{
	if (m_bIsDie)
	{
		m_pFSM->Set_State(DIVEDAVESTATE::DIE);
		return true;
	}

	if (m_bIsHit)
	{
		m_pFSM->Set_State(DIVEDAVESTATE::HIT);
	
		return true;
	}

	return false;
}

HRESULT CDiveDave::Ready_Component()
{
	// 버퍼
	if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
		return E_FAIL;

	// 텍스쳐
	if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_DivePlayerIdleTexture", L"Com_IdleTexture", &m_pTextureCom))))
		return E_FAIL;
	if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_DivePlayerMoveUpTexture", L"Com_MoveUpTexture", &m_pTextureCom))))
		return E_FAIL;
	if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_DivePlayerMoveSideUpTexture", L"Com_MoveSideUpTexture", &m_pTextureCom))))
		return E_FAIL;
	if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_DivePlayerMoveSideTexture", L"Com_MoveSideTexture", &m_pTextureCom))))
		return E_FAIL;
	if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_DivePlayerMoveSideDownTexture", L"Com_MoveSideDownTexture", &m_pTextureCom))))
		return E_FAIL;
	if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_DivePlayerMoveDownTexture", L"Com_MoveDownTexture", &m_pTextureCom))))
		return E_FAIL;
	if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_DivePlayerTanningTexture", L"Com_TanningTexture", &m_pTextureCom))))
		return E_FAIL;
	if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_DivePlayerAttackReadyTexture", L"Com_AttackReadyTexture", &m_pTextureCom))))
		return E_FAIL;
	if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_DivePlayerAttackFireTexture", L"Com_AttackFireTexture", &m_pTextureCom))))
		return E_FAIL;
	if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_DivePlayerAttackFailTexture", L"Com_AttackFailTexture", &m_pTextureCom))))
		return E_FAIL;
	if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_DivePlayerAttackFightTexture", L"Com_AttackFightTexture", &m_pTextureCom))))
		return E_FAIL;
	if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_DivePlayerMeleeDaggerAttack", L"Com_MeleeDaggerAttackTexture", &m_pTextureCom))))
		return E_FAIL;
	if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_DivePlayerOpenTexture", L"Com_OpenTexture", &m_pTextureCom))))
		return E_FAIL;
	if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_DivePlayerPickUpTexture", L"Com_PickUpTexture", &m_pTextureCom))))
		return E_FAIL;
	if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_DivePlayerHitTexture", L"Com_HitTexture", &m_pTextureCom))))
		return E_FAIL;
	if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_DivePlayerDieTexture", L"Com_DieTexture", &m_pTextureCom))))
		return E_FAIL;

	// 트랜스폼
	if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
		return E_FAIL;
	return S_OK;
}
HRESULT	CDiveDave::Add_State()
{
	m_pFSM = CFSM<CDiveDave, DIVEDAVESTATE>::Create(this);
	if (m_pFSM == nullptr)
		return E_FAIL;


	m_pFSM->Add_State<CDiveDaveIdle>(DIVEDAVESTATE::IDLE);
	m_pFSM->Add_State<CDiveDaveMove>(DIVEDAVESTATE::MOVE);
	m_pFSM->Add_State<CDiveDaveAttack>(DIVEDAVESTATE::ATTACK);
	m_pFSM->Add_State<CDiveDaveMeeleAttack>(DIVEDAVESTATE::MELEEATTACK);
	m_pFSM->Add_State<CDiveDaveTanning>(DIVEDAVESTATE::TANNING);
	m_pFSM->Add_State<CDiveDaveOpen>(DIVEDAVESTATE::OPEN);
	m_pFSM->Add_State<CDiveDavePickUp>(DIVEDAVESTATE::PICKUP);
	m_pFSM->Add_State<CDiveDaveHit>(DIVEDAVESTATE::HIT);
	m_pFSM->Add_State<CDiveDaveDie>(DIVEDAVESTATE::DIE);

	//m_mapState.insert({ DIVEDAVESTATE::IDLE, CDiveDaveIdle::Create(this) });
	//m_mapState.insert({ DIVEDAVESTATE::MOVE, CDiveDaveMove::Create(this) });
	//m_mapState.insert({ DIVEDAVESTATE::ATTACK, CDiveDaveAttack::Create(this) });
	//m_mapState.insert({ DIVEDAVESTATE::MELEEATTACK, CDiveDaveMeeleAttack::Create(this) });
	//m_mapState.insert({ DIVEDAVESTATE::TANNING, CDiveDaveTanning::Create(this) });
	//m_mapState.insert({ DIVEDAVESTATE::OPEN, CDiveDaveOpen::Create(this) });
	//m_mapState.insert({ DIVEDAVESTATE::PICKUP, CDiveDavePickUp::Create(this) });
	//m_mapState.insert({ DIVEDAVESTATE::HIT, CDiveDaveHit::Create(this) });
	//m_mapState.insert({ DIVEDAVESTATE::DIE, CDiveDaveDie::Create(this) });

	return S_OK;
}

void CDiveDave::Key_Input()
{
	if (!m_bCanKeyInput)
		return;


	if (CDiveDave::Get_State() == DIVEDAVESTATE::IDLE && CDInputMgr::GetInstance()->Key_Down(DIK_TAB))
		m_eCurEquipped = static_cast<EQUIPPED>((((_uint)m_eCurEquipped) + 1) % (_uint)EQUIPPED::EQUIPPED_END);
}

void CDiveDave::Mouse_Input()
{
	if (!m_bCanMouseInput)
		return;

	if (CDInputMgr::GetInstance()->Mouse_Down(DIM_LB))
		Set_State(DIVEDAVESTATE::MELEEATTACK);
	else if (CDInputMgr::GetInstance()->Mouse_Down(DIM_RB))
		Set_State(DIVEDAVESTATE::ATTACK);
}


CDiveDave* CDiveDave::Create()
{
	CDiveDave* pPlayer = new CDiveDave;
	if (FAILED(pPlayer->Ready_GameObject()))
	{
		Safe_Release(pPlayer);
		MSG_BOX("DiveDave Create Failed");
		return nullptr;
	}
	return pPlayer;
}

void CDiveDave::Free()
{
	Safe_Release(m_pAABB);
	Safe_Release(m_pAABBItem);
	Safe_Release(m_pFSM);
	//for_each(m_mapState.begin(), m_mapState.end(), CDeleteMap());
	//m_mapState.clear();
	CGameObject::Free();
}
