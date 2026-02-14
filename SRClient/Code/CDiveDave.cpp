#include "pch.h"
#include "CDiveDave.h"
#include "CDInputMgr.h"
#include "CGraphicDev.h"
#include "CRenderer.h"
#include "CPlayerState.h"
#include "CDiveDaveIdle.h"
#include "CDiveDaveMove.h"
#include "CDiveDaveAttack.h"
#include "CAttackReadyArm.h"

string debugState[(_uint)DiveState::DAVE_STATE_END] = {"IDLE", "MOVE", "ATTACK", "DIE"};

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

	Set_State(DiveState::IDLE);
	return S_OK;
}

_int CDiveDave::Update_GameObject(const _float& fTimeDelta)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
	Key_Input();
	Mouse_Input();
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);
	m_pState->Update_State(fTimeDelta);

#ifdef _DEBUG
	ImGui::Begin("DiveDave Info");
	string state = "State : " + debugState[(_uint)m_eCurState];
	ImGui::Text(state.c_str());
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	ImGui::SliderFloat3("Transform", vPos, 0.f, 0.f);
	ImGui::End();
#endif
	return iExit;
}

void CDiveDave::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
	m_pState->LateUpdate_State(fTimeDelta);

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	Compute_ViewZ(&vPos);
}

void CDiveDave::Render_GameObject()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pState->Render_State();

	m_pBufferCom->Render_Buffer();

	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CDiveDave::Set_State(DiveState state)
{
	if (m_mapState[state] == m_pState)
		return;

	if(m_pState != nullptr)
		m_pState->Exit();

	m_pState = m_mapState[state];
	m_eCurState = state;

	m_pState->Enter();
}

void CDiveDave::Move(_vec3* vDir, const _float& fTimeDelta)
{
	m_pTransformCom->Move_Pos(vDir, m_fSpeed, fTimeDelta);
}

void CDiveDave::AddFrame(const _float& fTimeDelta, const _float& fSpeed, _uint size)
{
	m_fFrame += fSpeed * fTimeDelta;
	if (m_fFrame > size)
		m_fFrame = 0.f;
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
	if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_DivePlayerAttackReadyTexture", L"Com_AttackReadyTexture", &m_pTextureCom))))
		return E_FAIL;
	if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_DivePlayerAttackFireTexture", L"Com_AttackFireTexture", &m_pTextureCom))))
		return E_FAIL;
	if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_DivePlayerAttackFailTexture", L"Com_AttackFailTexture", &m_pTextureCom))))
		return E_FAIL;


	// 트랜스폼
	if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
		return E_FAIL;
	return S_OK;
}
HRESULT	CDiveDave::Add_State()
{
	m_mapState.insert({ DiveState::IDLE, CDiveDaveIdle::Create(this) });
	m_mapState.insert({ DiveState::MOVE, CDiveDaveMove::Create(this) });
	m_mapState.insert({ DiveState::ATTACK, CDiveDaveAttack::Create(this) });
	//m_mapState.insert({ DiveState::DIE, CDiveDaveDie::Create(this) });

	return S_OK;
}

void CDiveDave::Key_Input()
{

}

void CDiveDave::Mouse_Input()
{
	if (CDInputMgr::GetInstance()->Mouse_Down(DIM_LB))
		Set_State(DiveState::ATTACK);

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
	for_each(m_mapState.begin(), m_mapState.end(), CDeleteMap());
	m_mapState.clear();
	CGameObject::Free();
}
