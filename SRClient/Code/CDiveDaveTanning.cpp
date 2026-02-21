#include "pch.h"
#include "CDiveDaveTanning.h"
#include "CDInputMgr.h"
#include "CDiveDave.h"
#include "CCameraMgr.h"
#include "CDiveDaveCam.h"
CDiveDaveTanning::CDiveDaveTanning(CGameObject* pOwner)
	:CPlayerState(pOwner)
{
}

CDiveDaveTanning::~CDiveDaveTanning()
{
}

void CDiveDaveTanning::Enter()
{
	static_cast<CDiveDave*>(m_pPlayer)->Init_Frame();
	_float fWidth = 42.f;
	_float fHeight = 55.f;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	static_cast<CDiveDave*>(m_pPlayer)->Multiply_Scale(&vScale);
	static_cast<CDiveDave*>(m_pPlayer)->Set_TextureCom(L"Com_TanningTexture");
}

void CDiveDaveTanning::Input(const _float& fTimeDelta)
{
	if (CDInputMgr::GetInstance()->Key_Pressing(DIK_SPACE))
	{
		m_fTanningTime += fTimeDelta * 5.f;
	}
	if (CDInputMgr::GetInstance()->Key_Up(DIK_SPACE))
	{
		static_cast<CDiveDave*>(m_pPlayer)->Set_State(DiveState::IDLE);
	}
}

_int CDiveDaveTanning::Update_State(const _float& fTimeDelta)
{
	Input(fTimeDelta);

	if (m_fTanningTime > 5.f)
	{
		static_cast<CDiveDave*>(m_pPlayer)->Set_State(DiveState::IDLE);
	}

	static_cast<CDiveDave*>(m_pPlayer)->AddFrame(fTimeDelta, 10.f, 4);
	return 0;
}

void CDiveDaveTanning::LateUpdate_State(const _float& fTimeDelta)
{
}

void CDiveDaveTanning::Render_State()
{
	CTexture* pPlayerTextureCom = static_cast<CDiveDave*>(m_pPlayer)->Get_TextureCom();

	_float fFrame = static_cast<CDiveDave*>(m_pPlayer)->Get_Frame();

	pPlayerTextureCom->Set_Texture((_uint)fFrame);
}

void CDiveDaveTanning::Exit()
{
	_float fWidth = 42.f;
	_float fHeight = 55.f;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
	static_cast<CDiveDave*>(m_pPlayer)->Multiply_Scale(&vScale);

	Clear();
}

void CDiveDaveTanning::Clear()
{
	m_fTanningTime = 0.f;
}

CDiveDaveTanning* CDiveDaveTanning::Create(CGameObject* pOwner)
{
	CDiveDaveTanning* pState = new CDiveDaveTanning(pOwner);

	return pState;
}

void CDiveDaveTanning::Free()
{
}
