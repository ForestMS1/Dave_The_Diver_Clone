#include "pch.h"
#include "CDiveDavePickUp.h"
#include "CDInputMgr.h"
#include "CDiveDave.h"
#include "CCameraMgr.h"
#include "CDiveDaveCam.h"
#include "CDiveItem.h"
#include "CDiveDaveGun.h"
CDiveDavePickUp::CDiveDavePickUp(CDiveDave* pOwner)
	:CBaseState<CDiveDave>(pOwner)
{
}

CDiveDavePickUp::~CDiveDavePickUp()
{
}

void CDiveDavePickUp::Enter()
{
	m_pOwner->Init_Frame();
	_float fWidth = 50.f;
	_float fHeight = 56.f;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	m_pOwner->Multiply_Scale(&vScale);
	m_pOwner->Set_TextureCom(L"Com_PickUpTexture");
}

void CDiveDavePickUp::Input(const _float& fTimeDelta)
{
	
}

_int CDiveDavePickUp::Update_State(const _float& fTimeDelta)
{
	Input(fTimeDelta);
	m_pOwner->AddFrame(fTimeDelta, 5.f, 2);
	if (m_pOwner->Get_Frame() > 1.f)
		m_pOwner->Set_State(DIVEDAVESTATE::IDLE);
	return 0;
}

void CDiveDavePickUp::LateUpdate_State(const _float& fTimeDelta)
{

}

void CDiveDavePickUp::Render_State()
{
	CTexture* pPlayerTextureCom = m_pOwner->Get_TextureCom();

	_float fFrame = m_pOwner->Get_Frame();

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
	m_pOwner->Multiply_Scale(&vScale);
}

void CDiveDavePickUp::Clear()
{
	if (m_pOwner->m_pCurOnItem != nullptr)
	{
		if (static_cast<CDiveItem*>(m_pOwner->m_pCurOnItem)->GetItemType() == ITEMTYPE::USEITEM)
		{
			Event e;
			e.type = EVENTTYPE::GET_ITEM;
			e.ItemTextureName = static_cast<CDiveItem*>(m_pOwner->m_pCurOnItem)->Get_TexName();

			if (m_pOwner->m_mapCanUseItemSlot[L"ItemSlot1"] == nullptr)
			{
				m_pOwner->m_mapCanUseItemSlot[L"ItemSlot1"] = m_pOwner->m_pCurOnItem;
				static_cast<CDiveItem*>(m_pOwner->m_pCurOnItem)->GetItem();
				e.value = 1;
				m_pOwner->Notify(e);
			}
			else if (m_pOwner->m_mapCanUseItemSlot[L"ItemSlot2"] == nullptr)
			{
				m_pOwner->m_mapCanUseItemSlot[L"ItemSlot2"] = m_pOwner->m_pCurOnItem;
				static_cast<CDiveItem*>(m_pOwner->m_pCurOnItem)->GetItem();
				e.value = 2;
				m_pOwner->Notify(e);
			}
		}
		else if (static_cast<CDiveItem*>(m_pOwner->m_pCurOnItem)->GetItemType() == ITEMTYPE::WEAPONITEM)
		{
			Event e;
			e.type = EVENTTYPE::GET_WEAPON;
			e.ItemTextureName = static_cast<CDiveItem*>(m_pOwner->m_pCurOnItem)->Get_TexName();
			e.value = 2; // GunSlot UI¿¡ ¾ÆÀÌÅÛ È¹µæÀ» ¾Ë¸²
			m_pOwner->Notify(e);

			CGameMemMgr::CDaveInfo::DAVE_GUN eGun = static_cast<CDiveItem*>(m_pOwner->m_pCurOnItem)->GetGunType();
			static_cast<CDiveDaveGun*>(m_pOwner->Get_WeponSlot(EQUIPPED::GUN))->Change_Gun(eGun);
			static_cast<CDiveItem*>(m_pOwner->m_pCurOnItem)->GetItem();
		}
		else
			return;
	}

	m_pOwner->Set_IsOnItem(false);
	m_pOwner->m_pCurOnItem = nullptr;
}


CDiveDavePickUp* CDiveDavePickUp::Create(CDiveDave* pOwner)
{
	CDiveDavePickUp* pState = new CDiveDavePickUp(pOwner);

	return pState;
}

void CDiveDavePickUp::Free()
{
}
