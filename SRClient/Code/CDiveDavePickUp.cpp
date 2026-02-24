#include "pch.h"
#include "CDiveDavePickUp.h"
#include "CDInputMgr.h"
#include "CDiveDave.h"
#include "CCameraMgr.h"
#include "CDiveDaveCam.h"
#include "CDiveItem.h"
CDiveDavePickUp::CDiveDavePickUp(CDiveDave* pOwner)
	:CBaseState<CDiveDave>(pOwner)
{
}

CDiveDavePickUp::~CDiveDavePickUp()
{
}

void CDiveDavePickUp::Enter()
{
	static_cast<CDiveDave*>(m_pOwner)->Init_Frame();
	_float fWidth = 50.f;
	_float fHeight = 56.f;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	static_cast<CDiveDave*>(m_pOwner)->Multiply_Scale(&vScale);
	static_cast<CDiveDave*>(m_pOwner)->Set_TextureCom(L"Com_PickUpTexture");
}

void CDiveDavePickUp::Input(const _float& fTimeDelta)
{
	
}

_int CDiveDavePickUp::Update_State(const _float& fTimeDelta)
{
	Input(fTimeDelta);
	static_cast<CDiveDave*>(m_pOwner)->AddFrame(fTimeDelta, 5.f, 2);
	if (static_cast<CDiveDave*>(m_pOwner)->Get_Frame() > 1.f)
		static_cast<CDiveDave*>(m_pOwner)->Set_State(DIVEDAVESTATE::IDLE);
	return 0;
}

void CDiveDavePickUp::LateUpdate_State(const _float& fTimeDelta)
{

}

void CDiveDavePickUp::Render_State()
{
	CTexture* pPlayerTextureCom = static_cast<CDiveDave*>(m_pOwner)->Get_TextureCom();

	_float fFrame = static_cast<CDiveDave*>(m_pOwner)->Get_Frame();

	pPlayerTextureCom->Set_Texture((_uint)fFrame);
}

void CDiveDavePickUp::Exit()
{
	Clear();

	_float fWidth = 50.f;
	_float fHeight = 56.f;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
	static_cast<CDiveDave*>(m_pOwner)->Multiply_Scale(&vScale);
}

void CDiveDavePickUp::Clear()
{
	if(static_cast<CDiveItem*>(static_cast<CDiveDave*>(m_pOwner)->m_pCurOnItem) != nullptr)
		static_cast<CDiveItem*>(static_cast<CDiveDave*>(m_pOwner)->m_pCurOnItem)->GetItem();

	static_cast<CDiveDave*>(m_pOwner)->Set_IsOnItem(false);
	static_cast<CDiveDave*>(m_pOwner)->m_pCurOnItem = nullptr;
}


CDiveDavePickUp* CDiveDavePickUp::Create(CDiveDave* pOwner)
{
	CDiveDavePickUp* pState = new CDiveDavePickUp(pOwner);

	return pState;
}

void CDiveDavePickUp::Free()
{
}
