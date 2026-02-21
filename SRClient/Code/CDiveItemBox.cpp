#include "CDiveItemBox.h"
#include "CGraphicDev.h"
#include "CColliderMgr.h"
#include "CRenderer.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"

wstring wsItemBoxTex[(_uint)ITEMBOXTEX::CHEST_END] = { L"Tex_Chest_A", L"Tex_Chest_Aopen", L"Tex_Chest_Weapon", L"Tex_Chest_WeaponOpen"};

CDiveItemBox::CDiveItemBox()
	: m_pBufferCom(nullptr)
	, m_pTransformCom(nullptr)
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

	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(wsItemBoxTex[(_uint)m_eCurBoxTex])->at(0))->Get_ImgInfo();
	imgInfo.Width;

	_float fWidth = imgInfo.Width;;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 0.f };
	m_pTransformCom->m_vScale = vScale;

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

    return 0;
}

void CDiveItemBox::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	Compute_ViewZ(&vPos);
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

CDiveItemBox* CDiveItemBox::Create()
{
	CDiveItemBox* pItemBox = new CDiveItemBox;
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
