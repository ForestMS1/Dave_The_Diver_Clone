#include "pch.h"
#include "CDiveDaveIdle.h"
#include "CDInputMgr.h"
#include "CDiveDave.h"
#include "CCameraMgr.h"
#include "CDiveDaveCam.h"
CDiveDaveIdle::CDiveDaveIdle(CDiveDave* pOwner)
	:CBaseState<CDiveDave>(pOwner)
{
}

CDiveDaveIdle::~CDiveDaveIdle()
{
}

void CDiveDaveIdle::Enter()
{
	m_pOwner->Init_Frame();
	_float fWidth = 40.f;
	_float fHeight = 57.f;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	m_pOwner->Multiply_Scale(&vScale);
	m_pOwner->Set_TextureCom(L"Com_IdleTexture");
}

void CDiveDaveIdle::Input(const _float& fTimeDelta)
{
	// ItemBox 충돌상태에서 SPACE누르면 OPEN 진입
	if (m_pOwner->m_bIsOnItemBox && CDInputMgr::GetInstance()->Key_Down(DIK_SPACE))
		m_pOwner->Set_State(DIVEDAVESTATE::OPEN);

	// Item 충돌상태에서 SPACE 누르면 아이템 줍기
	if(m_pOwner->m_bIsOnItem && CDInputMgr::GetInstance()->Key_Down(DIK_SPACE))
		m_pOwner->Set_State(DIVEDAVESTATE::PICKUP);


	if (!m_pOwner->Get_CanMouseInput())
		return;


	if (CDInputMgr::GetInstance()->Key_Down(DIK_W) || CDInputMgr::GetInstance()->Key_Down(DIK_A)
		|| CDInputMgr::GetInstance()->Key_Down(DIK_S) || CDInputMgr::GetInstance()->Key_Down(DIK_D))
		m_pOwner->Set_State(DIVEDAVESTATE::MOVE);
}

_int CDiveDaveIdle::Update_State(const _float& fTimeDelta)
{
	Input(fTimeDelta);
	Restore_Fov(fTimeDelta);
	m_pOwner->AddFrame(fTimeDelta, 10.f, 8);
	return 0;
}

void CDiveDaveIdle::LateUpdate_State(const _float& fTimeDelta)
{
}

void CDiveDaveIdle::Render_State()
{
	CTexture* pPlayerTextureCom = m_pOwner->Get_TextureCom();

	_float fFrame = m_pOwner->Get_Frame();

	pPlayerTextureCom->Set_Texture((_uint)fFrame);
}

void CDiveDaveIdle::Exit()
{
	_float fWidth = 40.f;
	_float fHeight = 57.f;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
	m_pOwner->Multiply_Scale(&vScale);
}

void CDiveDaveIdle::Clear()
{
}

void CDiveDaveIdle::Restore_Fov(const _float& fTimeDelta)
{
	CDiveDaveCam* pCam = static_cast<CDiveDaveCam*>(CCameraMgr::GetInstance()->Get_CurCamera());
	if (pCam == nullptr)
		return;
	if (D3DXToDegree(pCam->GetFov()) < 60.f)
		pCam->ZoomOut(fTimeDelta * 10.f);
}

CDiveDaveIdle* CDiveDaveIdle::Create(CDiveDave* pOwner)
{
	CDiveDaveIdle* pState = new CDiveDaveIdle(pOwner);

	return pState;
}

void CDiveDaveIdle::Free()
{
}
