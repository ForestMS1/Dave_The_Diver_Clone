#include "CJohnIntro.h"
#include "CGraphicDev.h"
#include "CColliderMgr.h"
#include "CRenderer.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CDiveDave.h"
#include "CO2Capsule.h"
#include "CManagement.h"


CJohnIntro::CJohnIntro()
{

}

CJohnIntro::CJohnIntro(const CJohnIntro& rhs)
	: CGameObject(rhs)
{
}

CJohnIntro::~CJohnIntro()
{
}

HRESULT CJohnIntro::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	_vec3 vScale = { 640.f, 360.f, 0.f };
	m_pTransformCom->Set_Scale(&vScale);

	return S_OK;
}

_int CJohnIntro::Update_GameObject(const _float& fTimeDelta)
{
	// 충돌체 그룹에 넣어줘야한다.
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ORTHO_UI, this);

	AddFrame(fTimeDelta, 50.f, 138, false);

	if (m_fFrame == 137)
		m_bDead = true;

	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	Compute_ViewZ(&vPos);

	return 0;
}

void CJohnIntro::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CJohnIntro::Render_GameObject()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Boss_JW_Intro"))
	{
		if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at((_uint)m_fFrame)))
		{
			pGraphicDev->SetTexture(0, pTexture->Get_Texture());
		}
	}

	m_pBufferCom->Render_Buffer();
}

HRESULT CJohnIntro::Ready_Component()
{
	// 버퍼
	if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
		return E_FAIL;

	// 트랜스폼
	if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
		return E_FAIL;


	return S_OK;
}

CJohnIntro* CJohnIntro::Create()
{
	CJohnIntro* pIntro = new CJohnIntro;
	if (FAILED(pIntro->Ready_GameObject()))
	{
		Safe_Release(pIntro);
		MSG_BOX("CJohnIntro Create Failed");
		return nullptr;
	}
	return pIntro;
}

void CJohnIntro::Free()
{
	CGameObject::Free();
}

void CJohnIntro::AddFrame(const _float& fTimeDelta, const _float& fSpeed, _uint size, _bool loop)
{
	m_fFrame += fSpeed * fTimeDelta;
	if (loop)
	{
		if (m_fFrame > size)
			m_fFrame = 0.f;
	}
	else
	{
		if (m_fFrame > size)
			m_fFrame = size - 1;
	}
}
