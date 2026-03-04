#include "pch.h"
#include "CDiveDaveInitStart.h"
#include "CDInputMgr.h"
#include "CDiveDave.h"
#include "CCameraMgr.h"
#include "CDiveDaveCam.h"
#include "CDiveItem.h"
#include "CParticleMgr.h"
#include "CSoundMgr.h"
CDiveDaveInitStart::CDiveDaveInitStart(CDiveDave* pOwner)
	:CBaseState<CDiveDave>(pOwner)
{
}

CDiveDaveInitStart::~CDiveDaveInitStart()
{
}

void CDiveDaveInitStart::Enter()
{
	m_pOwner->Init_Frame();
	_float fWidth = 63.f;
	_float fHeight = 39.f;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	m_pOwner->Multiply_Scale(&vScale);
	m_pOwner->Set_TextureCom(L"Com_MoveSideTexture");


	CTransform* pTransformCom = m_pOwner->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform");
	pTransformCom->Set_Pos(-20.f, 20.f, 0.f);
	pTransformCom->Get_Info(INFO_POS, &m_vOriginPos);

	CSoundMgr::GetInstance()->PlaySoundLoop(L"Sound_DiveInGameBGM", CSoundMgr::CHANNELID::BGM, 1.f);
	//CSoundMgr::GetInstance()->PlaySoundLoop(L"Sound_DiveInGameBGM", CSoundMgr::CHANNELID::BGM, 1.f);
}

void CDiveDaveInitStart::Input(const _float& fTimeDelta)
{

}

_int CDiveDaveInitStart::Update_State(const _float& fTimeDelta)
{
	m_fInitReady += fTimeDelta;
	if (m_fInitReady < 1.f)
		return 0;

	m_fInitTime += fTimeDelta;

	_float t = m_fInitTime;

	const float Depth = 5.f;     // 최대 잠수 깊이
	const float Speed = 2.5f;    // 가속 강도

	_float y = -Depth * (1.f - exp(-Speed * t));

	_vec3 vFinalPos = _vec3(-10.f + t * 3.f, 5.f + y, 0.f);

	if (vFinalPos.y > 0)
	{
		m_pOwner->Change_Depth(vFinalPos.y * 0.1f);
	}

	CTransform* pTransform = m_pOwner->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform");
	pTransform->Set_Pos(vFinalPos.x, vFinalPos.y, vFinalPos.z);

	_vec3 Pos{};
	pTransform->Get_Info(INFO_POS, &Pos);
	CParticleMgr::GetInstance()->spwan_Particle(PARTICLE_BUBBLE, Pos, 4);

	if (m_fInitTime > 3.5f)
		m_pOwner->Set_State(DIVEDAVESTATE::IDLE);

	m_pOwner->AddFrame(fTimeDelta, 10.f, 8);

	return 0;
}

void CDiveDaveInitStart::LateUpdate_State(const _float& fTimeDelta)
{
}

void CDiveDaveInitStart::Render_State()
{
	CTexture* pPlayerTextureCom = m_pOwner->Get_TextureCom();

	_float fFrame = m_pOwner->Get_Frame();

	pPlayerTextureCom->Set_Texture((_uint)fFrame);
}

void CDiveDaveInitStart::Exit()
{
	_float fWidth = 63.f;
	_float fHeight = 39.f;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
	m_pOwner->Multiply_Scale(&vScale);

	_vec3 vRotateDir = { 0.f, 0.f, 0.f };
	m_pOwner->Set_RotateDir(&vRotateDir);

	Clear();
}

void CDiveDaveInitStart::Clear()
{
	CTransform* pTransform = m_pOwner->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform");
	dynamic_cast<CDiveDaveCam*>(CCameraMgr::GetInstance()->Get_CurCamera())->Set_Target(&pTransform->m_vInfo[INFO_POS]);
	m_fInitTime = 0.00001f;
	m_fInitReady = 0.f;
}

CDiveDaveInitStart* CDiveDaveInitStart::Create(CDiveDave* pOwner)
{
	CDiveDaveInitStart* pState = new CDiveDaveInitStart(pOwner);

	return pState;
}

void CDiveDaveInitStart::Free()
{
}
