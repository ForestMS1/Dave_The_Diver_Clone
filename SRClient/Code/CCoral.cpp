#include "pch.h"
#include "CCoral.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CAssetMgr.h"
#include "CColliderMgr.h"

CCoral::CCoral()
	: CGameObject()
{
}

CCoral::CCoral(const wstring_view _TextureName, wstring _objectName) : CGameObject(), m_TextureName(_TextureName), m_nameCoralObject(_objectName)
{
}

CCoral::CCoral(const CGameObject& rhs)
	: CGameObject(rhs)
{
}

CCoral::~CCoral()
{
}

HRESULT CCoral::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	_vec3 vScale = { 0.5f, 0.5f, 1.f };
	m_pTransformCom->Multiply_Scale(&vScale);


	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(m_TextureName)->at(0))->Get_ImgInfo();
	imgInfo.Width;

	_float fWidth = imgInfo.Width;;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	m_pTransformCom->Multiply_Scale(&vScale);
		//-------------AABB Collider With ItemBox----------------
	_vec3 vExtents = { 1.0f, 1.0f, 1.0f };
	_vec3 vPos = { 0.0f, 0.0f, 0.0f };
	m_pAABB = CAABB::Create(&vPos, &vExtents, L"AABB_Coral", this);



	return S_OK;
}

_int CCoral::Update_GameObject(const _float& fTimeDelta)
{
	// 충돌체 그룹에 넣어줘야한다.
	if (b_CoralTerrian) {
		CColliderMgr::GetInstance()->AddColliderGroup(m_nameCoralObject, m_pAABB);
		m_pAABB->Transform(m_pTransformCom->Get_World());
	}
	
	

	if (b_CoralTerrian) {
		_int iExit = CGameObject::Update_GameObject(fTimeDelta);
		CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

		_vec3 vPos{};
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		Compute_ViewZ(&vPos);
		return iExit;
	}



}

void CCoral::LateUpdate_GameObject(const _float& fTimeDelta)
{
	
	if (b_CoralTerrian) {

		CGameObject::LateUpdate_GameObject(fTimeDelta);
		ColliderFrustom();
	}




}

void CCoral::Render_GameObject()
{
	if (e_Coral == CORAL_ON) {
		LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

		if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(m_TextureName.data()))
		{
			if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
			{
				pGraphicDev->SetTexture(0, pTexture->Get_Texture());
			}
		}

		pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

		m_pBufferCom->Render_Buffer();

		D3DXMATRIX matTmp;
		D3DXMatrixIdentity(&matTmp);
		pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);
	}
	


}

HRESULT CCoral::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;
	// Buffer
	if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
		return E_FAIL;

	// Transform
	if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
		return E_FAIL;

}


CCoral* CCoral::Create(const wstring_view _TextureName, wstring _objectName)
{
	CCoral* pCoral = new CCoral(_TextureName, _objectName);

	if (FAILED(pCoral->Ready_GameObject()))
	{
		Safe_Release(pCoral);
		MSG_BOX("pCoral Create Failed");
		return nullptr;
	}

	return pCoral;
}

void CCoral::Free()
{
	Safe_Release(m_pAABB);
	CGameObject::Free();
}

void CCoral::ColliderFrustom() {
	if (CColliderMgr::GetInstance()->Get_Colliders(L"Coll_TestCamera") != nullptr) {
		CCollider* CameraCollider = CColliderMgr::GetInstance()->Get_Colliders(L"Coll_TestCamera")->front();
		if (m_nameCoralObject != L"") {
			list<CCollider*>* ColliderList = CColliderMgr::GetInstance()->Get_Colliders(m_nameCoralObject);
			if (ColliderList != nullptr) {
				for (auto& pCollider : *ColliderList)
				{

					if (CameraCollider->Intersect(pCollider) && CORAL_OFF == e_Coral)
					{
						e_Coral = CORAL_ON;


					}

					if (!CameraCollider->Intersect(pCollider) && CORAL_ON == e_Coral) {
						e_Coral = CORAL_OFF;

					}

				}
			}
		

		}

	}
}
