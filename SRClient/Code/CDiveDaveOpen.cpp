#include "pch.h"
#include "CDiveDaveOpen.h"
#include "CDInputMgr.h"
#include "CDiveDave.h"
#include "CCameraMgr.h"
#include "CDiveDaveCam.h"
#include "CDiveItemBox.h"
CDiveDaveOpen::CDiveDaveOpen(CGameObject* pOwner)
	:CPlayerState(pOwner)
{
}

CDiveDaveOpen::~CDiveDaveOpen()
{
}

void CDiveDaveOpen::Enter()
{
	static_cast<CDiveDave*>(m_pPlayer)->Init_Frame();
	_float fWidth = 35.f;
	_float fHeight = 55.f;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	static_cast<CDiveDave*>(m_pPlayer)->Multiply_Scale(&vScale);
	static_cast<CDiveDave*>(m_pPlayer)->Set_TextureCom(L"Com_OpenTexture");
}

void CDiveDaveOpen::Input(const _float& fTimeDelta)
{
	if (CDInputMgr::GetInstance()->Key_Pressing(DIK_SPACE))
	{
		m_fOpenTime += fTimeDelta * 5.f;
	}
	if (CDInputMgr::GetInstance()->Key_Up(DIK_SPACE))
	{
		static_cast<CDiveDave*>(m_pPlayer)->Set_State(DiveState::IDLE);
	}
}

_int CDiveDaveOpen::Update_State(const _float& fTimeDelta)
{
	Input(fTimeDelta);
	if (m_fOpenTime > 5.f)
	{
		static_cast<CDiveDave*>(m_pPlayer)->Set_State(DiveState::IDLE);
	}

	static_cast<CDiveDave*>(m_pPlayer)->AddFrame(fTimeDelta, 10.f, 3);
	return 0;
}

void CDiveDaveOpen::LateUpdate_State(const _float& fTimeDelta)
{
}

void CDiveDaveOpen::Render_State()
{
	CTexture* pPlayerTextureCom = static_cast<CDiveDave*>(m_pPlayer)->Get_TextureCom();

	_float fFrame = static_cast<CDiveDave*>(m_pPlayer)->Get_Frame();

	pPlayerTextureCom->Set_Texture((_uint)fFrame);
}

void CDiveDaveOpen::Exit()
{
	_float fWidth = 35.f;
	_float fHeight = 55.f;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
	static_cast<CDiveDave*>(m_pPlayer)->Multiply_Scale(&vScale);

	Clear();
}

void CDiveDaveOpen::Clear()
{
	static_cast<CDiveItemBox*>(static_cast<CDiveDave*>(m_pPlayer)->m_pCurOnItemBox)->Set_Open();
	m_fOpenTime = 0.f;
}

CDiveDaveOpen* CDiveDaveOpen::Create(CGameObject* pOwner)
{
	CDiveDaveOpen* pState = new CDiveDaveOpen(pOwner);

	return pState;
}

void CDiveDaveOpen::Free()
{
}
