#include "CDiveItemBox.h"
#include "CGraphicDev.h"
#include "CColliderMgr.h"
#include "CRenderer.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CDiveDave.h"
#include "CO2Capsule.h"
#include "CManagement.h"
wstring wsItemBoxTex[(_uint)ITEMBOXTEX::CHEST_END] = 
{ L"Tex_Chest_A", L"Tex_Chest_Aopen", L"Tex_Chest_Box", L"Tex_Chest_Box_open", L"Tex_Chest_Weapon", L"Tex_Chest_WeaponOpen"};

CDiveItemBox::CDiveItemBox(ITEMBOXTEX ItemBoxType, _float x, _float y, _float z)
	: m_pBufferCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_eCurBoxTex(ItemBoxType)
	, m_vInitPos({x,y,z})
{

}

CDiveItemBox::CDiveItemBox(const CDiveItemBox& rhs)
	: CGameObject(rhs)
{
}

CDiveItemBox::~CDiveItemBox()
{
}

HRESULT CDiveItemBox::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	_vec3 vScale = { 0.5f, 0.5f, 1.f };
	m_pTransformCom->Multiply_Scale(&vScale);

	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(wsItemBoxTex[(_uint)m_eCurBoxTex])->at(0))->Get_ImgInfo();
	imgInfo.Width;

	_float fWidth = imgInfo.Width;;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	m_pTransformCom->Multiply_Scale(&vScale);

	m_pTransformCom->Set_Pos(m_vInitPos.x, m_vInitPos.y, m_vInitPos.z);

	//-------------AABB Collider With ItemBox----------------
	_vec3 vExtents = { 1.0f, 1.0f, 1.0f };
	_vec3 vPos = { 0.0f, 0.0f, 0.0f };
	m_pAABB = CAABB::Create(&vPos, &vExtents, L"AABB_ItemBox", this);


    return S_OK;
}

_int CDiveItemBox::Update_GameObject(const _float& fTimeDelta)
{
	// 충돌체 그룹에 넣어줘야한다.
	CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_ItemBox", m_pAABB);
	m_pAABB->Transform(m_pTransformCom->Get_World());

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	Compute_ViewZ(&vPos);
    return 0;
}

void CDiveItemBox::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);

	Collision_With_DiveDave();
}

void CDiveItemBox::Render_GameObject()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(wsItemBoxTex[(_uint)m_eCurBoxTex]))
	{
		if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
		{
			pGraphicDev->SetTexture(0, pTexture->Get_Texture());
		}
	}

	m_pBufferCom->Render_Buffer();
}

void CDiveItemBox::Set_Open()
{
	if (m_bIsCollWithMe && !m_bIsOpen)
	{
		m_bIsOpen = true;
		m_bIsCollWithMe = false;
		m_eCurBoxTex = ITEMBOXTEX((_uint)m_eCurBoxTex + 1);

		CDiveDave* pDiveDave = static_cast<CDiveDave*>
			(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Get_GameObjectFirst(L"DiveDave"));
		pDiveDave->Set_IsOnItemBox(false);
		pDiveDave->Set_CurOnItemBox(nullptr);

		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		CO2Capsule* pItem = CO2Capsule::Create(vPos);
		if (pItem != nullptr)
		{
			CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Add_GameObject(L"Item", pItem);
		}
	}
}

HRESULT CDiveItemBox::Ready_Component()
{
	// 버퍼
	if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
		return E_FAIL;

	// 트랜스폼
	if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
		return E_FAIL;


	return S_OK;
}

void CDiveItemBox::Collision_With_DiveDave()
{
	if (m_bIsOpen)
		return;
	// Test 레이어에있는 충돌체 리스트를 들고온다. 널체크
	if (auto pColliders = CColliderMgr::GetInstance()->Get_Colliders(L"Coll_DiveDaveWithItemBox"))
	{
		// 충돌체 순회
		for (auto& pCollider : *pColliders)
		{
			// 내가 아닌것들과 체크
			if (m_pAABB != pCollider)
			{
				// 충돌체 끼리 충돌 체크
				if (m_pAABB->Intersect(pCollider))
				{
					if (pCollider->Get_Tag() == L"AABB_DiveDaveWithItemBox")
					{
						if (!m_bIsCollWithMe)
							OnCollisionEnter(pCollider);
						else
							OnCollisionStay(pCollider);

					}
				}
				else if(m_bIsCollWithMe)
					OnCollisionExit(pCollider);
			}
		}
	}

}

void CDiveItemBox::OnCollisionEnter(CCollider* pCollider)
{
	m_bIsCollWithMe = true; // 나랑 플레이어랑 충돌중임
	CDiveDave* pDiveDave = static_cast<CDiveDave*>(pCollider->Get_VoidPtr());
	if (!m_bIsOpen)
	{
		pDiveDave->Set_CurOnItemBox(this);
		pDiveDave->Set_IsOnItemBox(true);
	}
}

void CDiveItemBox::OnCollisionStay(CCollider* pCollider)
{
	CDiveDave* pDiveDave = static_cast<CDiveDave*>(pCollider->Get_VoidPtr());
	if (m_bIsOpen)
	{
		pDiveDave->Set_CurOnItemBox(nullptr);
		pDiveDave->Set_IsOnItemBox(false);
	}
}

void CDiveItemBox::OnCollisionExit(CCollider* pCollider)
{
	CDiveDave* pDiveDave = static_cast<CDiveDave*>(pCollider->Get_VoidPtr());
	m_bIsCollWithMe = false;
	pDiveDave->Set_IsOnItemBox(false);
	pDiveDave->Set_CurOnItemBox(nullptr);
}

CDiveItemBox* CDiveItemBox::Create(ITEMBOXTEX ItemBoxType, _float x, _float y, _float z)
{
	CDiveItemBox* pItemBox = new CDiveItemBox(ItemBoxType, x, y, z);
	if (FAILED(pItemBox->Ready_GameObject()))
	{
		Safe_Release(pItemBox);
		MSG_BOX("pItemBox Create Failed");
		return nullptr;
	}
	return pItemBox;
}

void CDiveItemBox::Free()
{
	Safe_Release(m_pAABB);
	CGameObject::Free();
}
