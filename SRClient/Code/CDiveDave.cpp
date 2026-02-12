#include "pch.h"
#include "CDiveDave.h"
#include "CDInputMgr.h"
#include "CGraphicDev.h"
#include "CRenderer.h"
#include "CPlayerState.h"
#include "CDiveDaveIdle.h"
#include "CDiveDaveMove.h"
#include "CDiveDaveAttack.h"
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
	
	Set_State(DiveState::IDLE);

	return S_OK;
}

_int CDiveDave::Update_GameObject(const _float& fTimeDelta)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);
	m_pState->Update_State(fTimeDelta);
	return iExit;
}

void CDiveDave::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
	m_pState->LateUpdate_State(fTimeDelta);
}

void CDiveDave::Render_GameObject()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	//m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	m_pState->Render_State();

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

HRESULT CDiveDave::Ready_Component()
{
	// 버퍼
	if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
		return E_FAIL;

	// 텍스쳐
	//if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_PlayerTexture", L"Com_Texture", &m_pTextureCom))))
	//    return E_FAIL;

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
	if(CDInputMgr::GetInstance()->Key_Down(DIK_W) || CDInputMgr::GetInstance()->Key_Down(DIK_A)
		|| CDInputMgr::GetInstance()->Key_Down(DIK_S) || CDInputMgr::GetInstance()->Key_Down(DIK_D))
		Set_State(DiveState::MOVE);
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
