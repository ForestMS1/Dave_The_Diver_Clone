#include "pch.h"
#include "CDiveDaveHit.h"
#include "CDiveDave.h"
#include "CSoundMgr.h"
CDiveDaveHit::CDiveDaveHit(CDiveDave* pOwner)
	:CBaseState<CDiveDave>(pOwner)
{
}

CDiveDaveHit::~CDiveDaveHit()
{
}

void CDiveDaveHit::Enter()
{
	m_pOwner->Init_Frame();
	_float fWidth = 43.f;
	_float fHeight = 55.f;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	m_pOwner->Multiply_Scale(&vScale);
	m_pOwner->Set_TextureCom(L"Com_HitTexture");

	CSoundMgr::GetInstance()->PlaySoundOne(L"Sound_dave_hit", CSoundMgr::SFX, 1.f);
}

void CDiveDaveHit::Input(const _float& fTimeDelta)
{

}

_int CDiveDaveHit::Update_State(const _float& fTimeDelta)
{
	Input(fTimeDelta);
	m_pOwner->Acc_HitTime(fTimeDelta);
	if (m_pOwner->Get_HitTime() > 0.5f)
		m_pOwner->Set_State(DIVEDAVESTATE::IDLE);
	m_pOwner->AddFrame(fTimeDelta, 10.f, 2);
	return 0;
}

void CDiveDaveHit::LateUpdate_State(const _float& fTimeDelta)
{
}

void CDiveDaveHit::Render_State()
{
	CTexture* pPlayerTextureCom = m_pOwner->Get_TextureCom();

	_float fFrame = m_pOwner->Get_Frame();

	pPlayerTextureCom->Set_Texture((_uint)fFrame);
}

void CDiveDaveHit::Exit()
{
	_float fWidth = 43.f;
	_float fHeight = 55.f;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
	m_pOwner->Multiply_Scale(&vScale);

	Clear();
}

void CDiveDaveHit::Clear()
{
	m_pOwner->Hit_Free();
	m_pOwner->Reset_HitTime();
}


CDiveDaveHit* CDiveDaveHit::Create(CDiveDave* pOwner)
{
	CDiveDaveHit* pState = new CDiveDaveHit(pOwner);

	return pState;
}

void CDiveDaveHit::Free()
{
}
