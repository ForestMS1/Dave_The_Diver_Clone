#include "CCommonItemFragment.h"
#include "CGraphicDev.h"
#include "CRenderer.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CColliderMgr.h"
#include "CDiveDave.h"
#include "CManagement.h"
#include "CGetItemUI.h"
#include "CDiveItemDescUI.h"
CCommonItemFragment::CCommonItemFragment(_vec3 vOriginPos)
	: CDiveItem(vOriginPos)
{
}

CCommonItemFragment::CCommonItemFragment(const CCommonItemFragment& rhs)
	: CDiveItem(rhs)
{
}

CCommonItemFragment::~CCommonItemFragment()
{
}

HRESULT CCommonItemFragment::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_eItemType = ITEMTYPE::COMMONITEM;
	m_eCurState = ITEMSTATE::DROPPED;

	_vec3 vScale = { 0.2f, 0.2f, 1.f };
	m_pTransformCom->Multiply_Scale(&vScale);

	m_wsTexName = L"Tex_UIITem_Fragment";
	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(m_wsTexName)->at(0))->Get_ImgInfo();
	imgInfo.Width;

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

_int CCommonItemFragment::Update_GameObject(const _float& fTimeDelta)
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

void CCommonItemFragment::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (m_eCurState == ITEMSTATE::ACQUIRED)
		return;
	CDiveItem::Collision_With_DiveDave();
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CCommonItemFragment::Render_GameObject()
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

void CCommonItemFragment::GetItem()
{
	CDiveItem::GetItem();

	if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"2_Fish_Layer"))
	{
		auto pGetItemUI = CGetItemUI::Create(-500.f, 250.f);
		pGetItemUI->Set_ImgAssetName(m_wsTexName);
		pGetItemUI->Set_Title(L"볼트와 너트");
		pGetItemUI->Set_Rank(L"Rank_");
		pGetItemUI->Set_Weight(L"0.43kg");
		pGetItemUI->Ready_AfterCreate();
		pLayer->Add_GameObject(L"GetItemUI", pGetItemUI);


		CGameMemMgr::CDiveInfo::DIVE_FISH fish{};
		fish.fWeight = 0.43f;
		//fish.iRank = m_iRank;
		//fish.iStar = m_iStar;
		fish.sFishName = L"볼트와 너트";
		fish.sThumbNailAssetName = m_wsTexName;
		//fish.iMeatCnt = m_iMeatCnt;
		//fish.fLength = m_fLength;
		//fish.sSushiThumbNailAssetName = m_sSushiThumbNailAssetName;
		//fish.iSushiLv = m_iSushiLv;
		//fish.iSushiMoney = m_iSushiMoney;
		fish.bFish = false;
		CGameMemMgr::GetInstance()->Get_DiveInfos().back().Add_FishFront(fish);

		if (auto pDave = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Get_GameObjectFirst<CDiveDave>(L"DiveDave"))
		{
			pDave->Change_Weight(fish.fWeight);
		}

	}
}

void CCommonItemFragment::OpenItemDesc()
{
	if (auto pUI = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_UI_Layer")->Get_GameObjectFirst<CDiveItemDescUI>(L"DiveItemDescUI"))
	{
		if (!pUI->Get_Render())
		{
			pUI->Set_Title(L"볼트와 너트");
			pUI->Set_Desc(L"볼트와 너트다. 무언가를 제작할 때 쓰이는 듯 하다.");
			pUI->Set_Render(true);
		}
	}
}

HRESULT CCommonItemFragment::Ready_Component()
{
	// 버퍼
	if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
		return E_FAIL;

	// 트랜스폼
	if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
		return E_FAIL;


	return S_OK;
}

CCommonItemFragment* CCommonItemFragment::Create(_vec3 vOriginPos)
{
	CCommonItemFragment* pItem = new CCommonItemFragment(vOriginPos);
	if (FAILED(pItem->Ready_GameObject()))
	{
		Safe_Release(pItem);
		MSG_BOX("CCommonItemFragment Create Failed");
		return nullptr;
	}

	return pItem;
}

void CCommonItemFragment::Free()
{
	CDiveItem::Free();
	CGameObject::Free();
}

