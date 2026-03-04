#include "pch.h"
#include "CDiveDaveIdle.h"
#include "CDInputMgr.h"
#include "CDiveDave.h"
#include "CCameraMgr.h"
#include "CDiveDaveCam.h"
#include "CDiveItem.h"
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

	m_fIdleMoveLastSpeed = m_pOwner->Get_LastMoveSpeed();

	_vec3 vDir;
	if (m_pOwner->Get_LastMoveDir().x < 0)
	{
		vDir = { 0.f, -180.f, 0.f };
		m_pOwner->Set_RotateDir(&vDir);
	}
	else
	{
		vDir = { 0.f, 0.f, 0.f };
		m_pOwner->Set_RotateDir(&vDir);
	}
}

void CDiveDaveIdle::Input(const _float& fTimeDelta)
{
	// ItemBox 충돌상태에서 SPACE누르면 OPEN 진입
	if (m_pOwner->m_bIsOnItemBox && CDInputMgr::GetInstance()->Key_Down(DIK_SPACE))
		m_pOwner->Set_State(DIVEDAVESTATE::OPEN);

	// Item 충돌상태에서 SPACE 누르면 아이템 줍기
	if(m_pOwner->m_bIsOnItem && CDInputMgr::GetInstance()->Key_Down(DIK_SPACE))
		m_pOwner->Set_State(DIVEDAVESTATE::PICKUP);

	// Slicalbe 물고기랑 충돌상태에서 SPACE 누르면 물고기 슬라이스
	if (m_pOwner->Get_SlicableFish() != nullptr && CDInputMgr::GetInstance()->Key_Down(DIK_SPACE))
		m_pOwner->Set_State(DIVEDAVESTATE::TANNING);

	// Item 사용 (Item사용은 무조건 ItemSlot1에서만)
	if (CDInputMgr::GetInstance()->Key_Down(DIK_C))
	{
		CDiveItem* pItem = static_cast<CDiveItem*>(m_pOwner->m_mapCanUseItemSlot[L"ItemSlot1"]);
		if (pItem != nullptr)
		{
			pItem->UseItem(m_pOwner);
			Event e;
			e.type = EVENTTYPE::USE_ITEM;
			m_pOwner->Notify(e);
			m_pOwner->m_mapCanUseItemSlot[L"ItemSlot1"] = nullptr;
		}
	}

	// Item 슬롯 체인지
	if (CDInputMgr::GetInstance()->Key_Down(DIK_R) && !m_bItemSlotDefense)
	{
		m_bItemSlotDefense = true;
		CGameObject* pItem = m_pOwner->m_mapCanUseItemSlot[L"ItemSlot1"];
		m_pOwner->m_mapCanUseItemSlot[L"ItemSlot1"] = m_pOwner->m_mapCanUseItemSlot[L"ItemSlot2"];
		m_pOwner->m_mapCanUseItemSlot[L"ItemSlot2"] = pItem;
		Event e;
		e.type = EVENTTYPE::ITEMSLOT_CHANGE;
		m_pOwner->Notify(e);
	}

	// Weapon 슬롯 체인지
	if (CDInputMgr::GetInstance()->Key_Down(DIK_TAB) && !m_bWeaponSlotDefense)
	{
		m_bWeaponSlotDefense = true;
		m_pOwner->m_eCurEquipped = static_cast<EQUIPPED>((((_uint)m_pOwner->m_eCurEquipped) + 1) % (_uint)EQUIPPED::EQUIPPED_END);
		Event e;
		e.type = EVENTTYPE::WEAPONSLOT_CHANGE;
		m_pOwner->Notify(e);
	}
		


	if (!m_pOwner->Get_CanMouseInput())
		return;


	if (CDInputMgr::GetInstance()->Key_Down(DIK_W) || CDInputMgr::GetInstance()->Key_Down(DIK_A)
		|| CDInputMgr::GetInstance()->Key_Down(DIK_S) || CDInputMgr::GetInstance()->Key_Down(DIK_D))
		m_pOwner->Set_State(DIVEDAVESTATE::MOVE);
}

_int CDiveDaveIdle::Update_State(const _float& fTimeDelta)
{
	if (m_fIdleMoveLastSpeed > 0.f && m_pOwner->Get_PrevState() == DIVEDAVESTATE::MOVE)
	{
		_vec3 vLastDir = m_pOwner->Get_LastMoveDir();
		m_pOwner->Move(&vLastDir, fTimeDelta, m_fIdleMoveLastSpeed);
		m_fIdleMoveLastSpeed -= fTimeDelta * 5.f;
	}
	else
		m_fIdleMoveLastSpeed = 0.f;

	Input(fTimeDelta);
	Restore_Fov(fTimeDelta);
	m_pOwner->AddFrame(fTimeDelta, 10.f, 8);


	if (m_bItemSlotDefense)
	{
		m_fItemSlotChangeDelay -= fTimeDelta;
		if (m_fItemSlotChangeDelay <= 0.f)
		{
			m_bItemSlotDefense = false;
			m_fItemSlotChangeDelay = 1.f;
		}
	}

	if (m_bWeaponSlotDefense)
	{
		m_fWeaponSlotChangeDelay -= fTimeDelta;
		if (m_fWeaponSlotChangeDelay <= 0.f)
		{
			m_bWeaponSlotDefense = false;
			m_fWeaponSlotChangeDelay = 1.f;
		}
	}
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

	Clear();
}

void CDiveDaveIdle::Clear()
{
	// 아이템 슬롯 체인지 딜레이
	m_bItemSlotDefense = false;
	m_fItemSlotChangeDelay = 1.f;

	// 무기 슬롯 체인지 딜레이
	m_bWeaponSlotDefense = false;
	m_fWeaponSlotChangeDelay = 1.f;

	m_fIdleMoveLastSpeed = 0.f;
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
