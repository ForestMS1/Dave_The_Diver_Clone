#include "CItemTripleAxel.h"
#include "CGraphicDev.h"
#include "CRenderer.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CColliderMgr.h"
#include "CDiveDave.h"
#include "CManagement.h"
#include "CDiveGetWeaponUI.h"

CItemTripleAxel::CItemTripleAxel(_vec3 vOriginPos)
	: CDiveItem(vOriginPos)
{
}

CItemTripleAxel::CItemTripleAxel(const CItemTripleAxel& rhs)
	: CDiveItem(rhs)
{
}

CItemTripleAxel::~CItemTripleAxel()
{
}

HRESULT CItemTripleAxel::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_eItemType = ITEMTYPE::WEAPONITEM;
	m_eGunType = CGameMemMgr::CDaveInfo::DAVE_GUN::GUN_TRIPLE_ACCEL;

	_vec3 vScale = { 0.4f, 0.4f, 1.f };
	m_pTransformCom->Multiply_Scale(&vScale);

	m_wsTexName = L"TripleAxel";
	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(m_wsTexName)->at(0))->Get_ImgInfo();

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

_int CItemTripleAxel::Update_GameObject(const _float& fTimeDelta)
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

void CItemTripleAxel::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (m_eCurState == ITEMSTATE::ACQUIRED)
		return;
	CDiveItem::Collision_With_DiveDave();
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CItemTripleAxel::Render_GameObject()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(m_wsTexName))
	{
		if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
		{
			pGraphicDev->SetTexture(0, pTexture->Get_Texture());
		}
	}
	m_pBufferCom->Render_Buffer();
}

void CItemTripleAxel::GetItem()
{
	CDiveItem::GetItem();

	if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"2_Fish_Layer"))
	{
		auto pGetWeaponUI = CDiveGetWeaponUI::Create(500.f, -150.f);
		pGetWeaponUI->Set_ImgAssetName(L"Tex_UI_Gun_Triple_Accel");
		pGetWeaponUI->Set_Title(L"Æ®¸®ÇÃ ¾Ç¼¿");
		pGetWeaponUI->Set_Desc(L"Æ®¸®ÇÃ ¾Ç¼¼·ç´Ù.");
		pGetWeaponUI->Ready_AfterCreate();
		pLayer->Add_GameObject(L"GetWeaponUI", pGetWeaponUI);
	}
}

HRESULT CItemTripleAxel::Ready_Component()
{
	// ¹öÆÛ
	if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
		return E_FAIL;

	// Æ®·£½ºÆû
	if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
		return E_FAIL;


	return S_OK;
}

CItemTripleAxel* CItemTripleAxel::Create(_vec3 vOriginPos)
{
	CItemTripleAxel* pItem = new CItemTripleAxel(vOriginPos);
	if (FAILED(pItem->Ready_GameObject()))
	{
		Safe_Release(pItem);
		MSG_BOX("CItemTripleAxel Create Failed");
		return nullptr;
	}

	return pItem;
}

void CItemTripleAxel::Free()
{
	CDiveItem::Free();
	CGameObject::Free();
}

void CItemTripleAxel::UseItem(CGameObject* pUser)
{
	CDiveDave* pDiveDave = static_cast<CDiveDave*>(pUser);

	//pDiveDave->Restore_Hp(50.f);
}
