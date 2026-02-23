#include "CAttackSubReady.h"
#include "CDiveDave.h"
#include "CHelper.h"
#include "CDiveDaveCam.h"
#include "CDInputMgr.h"
#include "CCameraMgr.h"
#include "CDiveDaveAttack.h"
CAttackSubReady::CAttackSubReady(CDiveDaveAttack* pParentState)
	: CAttackSubState(pParentState)
{
	m_pDiveDave = m_pOwner->Get_OwnerDave();
}

CAttackSubReady::~CAttackSubReady()
{
}

void CAttackSubReady::Enter()
{
	m_pDiveDave->Init_Frame();
	_float fWidth = 29.f;
	_float fHeight = 54.f;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	m_pDiveDave->Multiply_Scale(&vScale);
	m_pDiveDave->Set_TextureCom(L"Com_AttackReadyTexture");
}

void CAttackSubReady::Input(const _float& fTimeDelta)
{
	CDiveDaveCam* pCam = static_cast<CDiveDaveCam*>(CCameraMgr::GetInstance()->Get_CurCamera());
	//ChaseToPlayerCam
	if (pCam == nullptr)
		return;

	if (CDInputMgr::GetInstance()->Mouse_Pressing(DIM_RB))
	{
		if (D3DXToDegree(pCam->GetFov()) > 55.f)
			pCam->ZoomIn(fTimeDelta * 10.f);
	}
	else
	{
		//static_cast<CDiveDave*>(m_pOwner)->Set_State(DIVEDAVESTATE::IDLE);
		m_pOwner->Set_State(ATTACKSUBSTATE::ATTACK_FIRE);
	}
}

_int CAttackSubReady::Update_State(const _float& fTimeDelta)
{
	if (m_pDiveDave->Get_State() != DIVEDAVESTATE::ATTACK)
		return 0;

	Input(fTimeDelta);
	m_pDiveDave->AddFrame(fTimeDelta, 10.f, 1);
	Mouse_Check();

	return 0;
}

void CAttackSubReady::LateUpdate_State(const _float& fTimeDelta)
{
	if (m_pDiveDave->Get_State() != DIVEDAVESTATE::ATTACK)
		return;
}

void CAttackSubReady::Render_State()
{
	if (m_pDiveDave->Get_State() != DIVEDAVESTATE::ATTACK)
		return;

	CTexture* pPlayerTextureCom = m_pDiveDave->Get_TextureCom();

	_float fFrame = m_pDiveDave->Get_Frame();

	pPlayerTextureCom->Set_Texture((_uint)fFrame);
}

void CAttackSubReady::Exit()
{
	_float fWidth = 29.f;
	_float fHeight = 54.f;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
	m_pDiveDave->Multiply_Scale(&vScale);
}

void CAttackSubReady::Clear()
{
}

void CAttackSubReady::Mouse_Check()
{
	_vec3 vMousePos, vPlayerPos;
	CHelper::GetMousePointInWorld(&vMousePos);
	m_pDiveDave->Get_Pos(&vPlayerPos);

	if (vMousePos.x <= vPlayerPos.x)
	{
		_vec3 vRotateDir = { 0.f, -180.f, 0.f };
		m_pDiveDave->Set_RotateDir(&vRotateDir);
	}
	else
	{
		_vec3 vRotateDir = { 0.f, 0.f, 0.f };
		m_pDiveDave->Set_RotateDir(&vRotateDir);
	}
}

CAttackSubReady* CAttackSubReady::Create(CDiveDaveAttack* pParentState)
{
	CAttackSubReady* pSubState = new CAttackSubReady(pParentState);

	return pSubState;
}

void CAttackSubReady::Free()
{

}
