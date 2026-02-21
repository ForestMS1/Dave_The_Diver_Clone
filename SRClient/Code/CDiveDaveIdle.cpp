#include "pch.h"
#include "CDiveDaveIdle.h"
#include "CDInputMgr.h"
#include "CDiveDave.h"
#include "CCameraMgr.h"
#include "CDiveDaveCam.h"
CDiveDaveIdle::CDiveDaveIdle(CGameObject* pOwner)
	:CPlayerState(pOwner)
{
}

CDiveDaveIdle::~CDiveDaveIdle()
{
}

void CDiveDaveIdle::Enter()
{
	static_cast<CDiveDave*>(m_pPlayer)->Init_Frame();
	_float fWidth = 40.f;
	_float fHeight = 57.f;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	static_cast<CDiveDave*>(m_pPlayer)->Multiply_Scale(&vScale);
	static_cast<CDiveDave*>(m_pPlayer)->Set_TextureCom(L"Com_IdleTexture");
}

void CDiveDaveIdle::Input(const _float& fTimeDelta)
{
	// ItemBox 충돌상태에서 SPACE누르면 OPEN 진입
	if (static_cast<CDiveDave*>(m_pPlayer)->m_bIsOnItemBox && CDInputMgr::GetInstance()->Key_Down(DIK_SPACE))
		static_cast<CDiveDave*>(m_pPlayer)->Set_State(DiveState::OPEN);


	if (!static_cast<CDiveDave*>(m_pPlayer)->Get_CanMouseInput())
		return;


	if (CDInputMgr::GetInstance()->Key_Down(DIK_W) || CDInputMgr::GetInstance()->Key_Down(DIK_A)
		|| CDInputMgr::GetInstance()->Key_Down(DIK_S) || CDInputMgr::GetInstance()->Key_Down(DIK_D))
		static_cast<CDiveDave*>(m_pPlayer)->Set_State(DiveState::MOVE);
}

_int CDiveDaveIdle::Update_State(const _float& fTimeDelta)
{
	Input(fTimeDelta);
	Restore_Fov(fTimeDelta);
	static_cast<CDiveDave*>(m_pPlayer)->AddFrame(fTimeDelta, 10.f, 8);
	return 0;
}

void CDiveDaveIdle::LateUpdate_State(const _float& fTimeDelta)
{
}

void CDiveDaveIdle::Render_State()
{
	CTexture* pPlayerTextureCom = static_cast<CDiveDave*>(m_pPlayer)->Get_TextureCom();

	_float fFrame = static_cast<CDiveDave*>(m_pPlayer)->Get_Frame();

	pPlayerTextureCom->Set_Texture((_uint)fFrame);
}

void CDiveDaveIdle::Exit()
{
	_float fWidth = 40.f;
	_float fHeight = 57.f;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
	static_cast<CDiveDave*>(m_pPlayer)->Multiply_Scale(&vScale);
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

CDiveDaveIdle* CDiveDaveIdle::Create(CGameObject* pOwner)
{
	CDiveDaveIdle* pState = new CDiveDaveIdle(pOwner);

	return pState;
}

void CDiveDaveIdle::Free()
{
}
