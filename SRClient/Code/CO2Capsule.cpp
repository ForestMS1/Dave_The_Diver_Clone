#include "CO2Capsule.h"
#include "CGraphicDev.h"
#include "CRenderer.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CColliderMgr.h"
#include "CDiveDave.h"
#include "CDiveItemDescUI.h"
#include "CManagement.h"
#include "CSoundMgr.h"
CO2Capsule::CO2Capsule(_vec3 vOriginPos)
	: CDiveItem(vOriginPos)
{
}

CO2Capsule::CO2Capsule(const CO2Capsule& rhs)
	: CDiveItem(rhs)
{
}

CO2Capsule::~CO2Capsule()
{
}

HRESULT CO2Capsule::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_eItemType = ITEMTYPE::USEITEM;

	_vec3 vScale = { 0.2f, 0.2f, 1.f };
	m_pTransformCom->Multiply_Scale(&vScale);

	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"Tex_O2Capsule")->at(0))->Get_ImgInfo();
	imgInfo.Width;
	m_wsTexName = L"Tex_O2Capsule";

	_float fWidth = imgInfo.Width;;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	vScale = { fWidth / fAspect, fHeight / fAspect, 0.f };
	m_pTransformCom->Multiply_Scale(&vScale);

	m_pTransformCom->Set_Pos(m_vOriginPos.x, m_vOriginPos.y, m_vOriginPos.z);


	//-------------AABB Collider With DiveDave----------------
	_vec3 vExtents = { 1.0f, 1.0f, 1.0f };
	_vec3 vPos = { 00.0f, 0.0f, 0.0f };
	m_pAABB = CAABB::Create(&vPos, &vExtents, L"AABB_Item", this);

	return S_OK;
}

_int CO2Capsule::Update_GameObject(const _float& fTimeDelta)
{
	if (m_eCurState == ITEMSTATE::ACQUIRED)
		return 0;

	CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_Item", m_pAABB);
	m_pAABB->Transform(m_pTransformCom->Get_World());
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
	CDiveItem::FSM(fTimeDelta);
	CGameObject::Update_GameObject(fTimeDelta);

	return 0;
}

void CO2Capsule::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (m_eCurState == ITEMSTATE::ACQUIRED)
		return;
	CDiveItem::Collision_With_DiveDave();
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CO2Capsule::Render_GameObject()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_O2Capsule"))
	{
		if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
		{
			pGraphicDev->SetTexture(0, pTexture->Get_Texture());
		}
	}
	m_pBufferCom->Render_Buffer();
}

void CO2Capsule::GetItem()
{
	CDiveItem::GetItem();
}

HRESULT CO2Capsule::Ready_Component()
{
	// ¹öÆÛ
	if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
		return E_FAIL;

	// Æ®·£½ºÆû
	if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
		return E_FAIL;


	return S_OK;
}

CO2Capsule* CO2Capsule::Create(_vec3 vOriginPos)
{
	CO2Capsule* pItem = new CO2Capsule(vOriginPos);
	if (FAILED(pItem->Ready_GameObject()))
	{
		Safe_Release(pItem);
		MSG_BOX("CO2Capsule Create Failed");
		return nullptr;
	}

	return pItem;
}

void CO2Capsule::Free()
{
	CDiveItem::Free();
	CGameObject::Free();
}

void CO2Capsule::UseItem(CGameObject* pUser)
{
	CDiveDave* pDiveDave = static_cast<CDiveDave*>(pUser);

	pDiveDave->Restore_Hp(50.f);

	CSoundMgr::GetInstance()->PlaySoundOne(L"Sound_O2CapsuleUse", CSoundMgr::SFX, 1.f);
}

void CO2Capsule::OpenItemDesc()
{
	if (auto pUI = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_UI_Layer")->Get_GameObjectFirst<CDiveItemDescUI>(L"DiveItemDescUI"))
	{
		if (!pUI->Get_Render())
		{
			pUI->Set_Title(L"O2Ä¸½¶");
			pUI->Set_Desc(L"¼ûÀ» ½®´Ù.");
			pUI->Set_Render(true);
		}
	}
}
