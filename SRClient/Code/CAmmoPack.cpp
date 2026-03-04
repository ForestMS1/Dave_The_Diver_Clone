#include "CAmmoPack.h"
#include "CGraphicDev.h"
#include "CRenderer.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CColliderMgr.h"
#include "CDiveDave.h"
#include "CDiveItemDescUI.h"
#include "CManagement.h"
#include "CSoundMgr.h"
#include "CDiveDaveGun.h"
#include "CDiveGetWeaponUI.h"
CAmmoPack::CAmmoPack(_vec3 vOriginPos)
	: CDiveItem(vOriginPos)
{
}

CAmmoPack::CAmmoPack(const CAmmoPack& rhs)
	: CDiveItem(rhs)
{
}

CAmmoPack::~CAmmoPack()
{
}

HRESULT CAmmoPack::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_eItemType = ITEMTYPE::USEIMMEDIATELY;

	_vec3 vScale = { 0.2f, 0.2f, 1.f };
	m_pTransformCom->Multiply_Scale(&vScale);

	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"Tex_AmmoPack")->at(0))->Get_ImgInfo();
	m_wsTexName = L"Tex_AmmoPack";

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

_int CAmmoPack::Update_GameObject(const _float& fTimeDelta)
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

void CAmmoPack::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (m_eCurState == ITEMSTATE::ACQUIRED)
		return;
	CDiveItem::Collision_With_DiveDave();
	CGameObject::LateUpdate_GameObject(fTimeDelta);

	m_fViewZ = 1.f;
}

void CAmmoPack::Render_GameObject()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_AmmoPack"))
	{
		if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
		{
			pGraphicDev->SetTexture(0, pTexture->Get_Texture());
		}
	}
	m_pBufferCom->Render_Buffer();
}

void CAmmoPack::GetItem()
{
	CDiveItem::GetItem();
	if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"2_Fish_Layer"))
	{
		auto pGetWeaponUI = CDiveGetWeaponUI::Create(500.f, -150.f);
		pGetWeaponUI->Set_ImgAssetName(L"Tex_AmmoPack");
		pGetWeaponUI->Set_Title(L"Åº¾à º¸Ãæ ÆÑ");
		pGetWeaponUI->Set_Desc(L"Åº¾àÀ» Áï½Ã º¸ÃæÇÑ´Ù.");
		pGetWeaponUI->Ready_AfterCreate();
		pLayer->Add_GameObject(L"GetWeaponUI", pGetWeaponUI);
	}
	CAmmoPack::UseItem(nullptr);
}

HRESULT CAmmoPack::Ready_Component()
{
	// ¹öÆÛ
	if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
		return E_FAIL;

	// Æ®·£½ºÆû
	if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
		return E_FAIL;


	return S_OK;
}

CAmmoPack* CAmmoPack::Create(_vec3 vOriginPos)
{
	CAmmoPack* pItem = new CAmmoPack(vOriginPos);
	if (FAILED(pItem->Ready_GameObject()))
	{
		Safe_Release(pItem);
		MSG_BOX("CAmmoPack Create Failed");
		return nullptr;
	}

	return pItem;
}

void CAmmoPack::Free()
{
	CDiveItem::Free();
	CGameObject::Free();
}

void CAmmoPack::UseItem(CGameObject* pUser)
{
	CDiveDaveGun* pGun = dynamic_cast<CDiveDaveGun*>
		(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Get_GameObjectFirst(L"DiveDaveGun"));
	if (pGun != nullptr)
		pGun->Add_AmmoCnt(5);
}

void CAmmoPack::OpenItemDesc()
{
	if (auto pUI = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_UI_Layer")->Get_GameObjectFirst<CDiveItemDescUI>(L"DiveItemDescUI"))
	{
		if (!pUI->Get_Render())
		{
			pUI->Set_Title(L"Åº¾à º¸Ãæ ÆÑ");
			pUI->Set_Desc(L"Åº¾àÀ» ÃæÀü ÇÑ´Ù.");
			pUI->Set_Render(true);
		}
	}
}
