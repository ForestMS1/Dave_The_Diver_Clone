#include "CJohn2Intro.h"
#include "CGraphicDev.h"
#include "CColliderMgr.h"
#include "CRenderer.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CDiveDave.h"
#include "CO2Capsule.h"
#include "CManagement.h"


CJohn2Intro::CJohn2Intro()
{

}

CJohn2Intro::CJohn2Intro(const CJohn2Intro& rhs)
	: CGameObject(rhs)
{
}

CJohn2Intro::~CJohn2Intro()
{
}

HRESULT CJohn2Intro::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	_vec3 vScale = { 640.f, 360.f, 0.f };
	m_pTransformCom->Set_Scale(&vScale);

	CDiveDave* pDiveDave = dynamic_cast<CDiveDave*>
		(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Get_GameObjectFirst(L"DiveDave"));

	Event e;
	e.type = EVENTTYPE::BOSS_INTRO_START;
	if (pDiveDave != nullptr)
		pDiveDave->State_Notify(e);

	return S_OK;
}

_int CJohn2Intro::Update_GameObject(const _float& fTimeDelta)
{
	// 충돌체 그룹에 넣어줘야한다.
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ORTHO_UI, this);

	AddFrame(fTimeDelta, 50.f, 142, false);

	if (m_fFrame == 141)
	{
		CDiveDave* pDiveDave = dynamic_cast<CDiveDave*>
			(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Get_GameObjectFirst(L"DiveDave"));
		if (pDiveDave == nullptr)
			return 0;

		Event e;
		e.type = EVENTTYPE::BOSS_INTRO_END;
		pDiveDave->State_Notify(e);
		m_bDead = true;
	}

	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	Compute_ViewZ(&vPos);

	return 0;
}

void CJohn2Intro::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CJohn2Intro::Render_GameObject()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Boss_JW2_Intro"))
	{
		if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at((_uint)m_fFrame)))
		{
			pGraphicDev->SetTexture(0, pTexture->Get_Texture());
		}
	}

	m_pBufferCom->Render_Buffer();
}

HRESULT CJohn2Intro::Ready_Component()
{
	// 버퍼
	if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
		return E_FAIL;

	// 트랜스폼
	if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
		return E_FAIL;


	return S_OK;
}

CJohn2Intro* CJohn2Intro::Create()
{
	CJohn2Intro* pIntro = new CJohn2Intro;
	if (FAILED(pIntro->Ready_GameObject()))
	{
		Safe_Release(pIntro);
		MSG_BOX("CJohn2Intro Create Failed");
		return nullptr;
	}
	return pIntro;
}

void CJohn2Intro::Free()
{
	CGameObject::Free();
}

void CJohn2Intro::AddFrame(const _float& fTimeDelta, const _float& fSpeed, _uint size, _bool loop)
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
