#include "CDaveGoldBall.h"
#include "CManagement.h"
#include "CGraphicDev.h"
#include "CDiveDave.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"

CDaveGoldBall::CDaveGoldBall()
{
}

CDaveGoldBall::CDaveGoldBall(const CDaveGoldBall& rhs)
	: CGameObject(rhs)
{
}

CDaveGoldBall::~CDaveGoldBall()
{
}

HRESULT CDaveGoldBall::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	_vec3 vScale = { 0.4f, 0.4f, 1.f };
	m_pTransformCom->Multiply_Scale(&vScale);

	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"Tex_DaveGoldBall")->at(0))->Get_ImgInfo();

	_float fWidth = imgInfo.Width;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	m_pTransformCom->Multiply_Scale(&vScale);

	m_bRender = false;

	return S_OK;
}

void CDaveGoldBall::Init()
{
	if (m_bInitComplete)
		return;

	m_sTexName = L"Tex_DaveGoldBall";
	m_bInitComplete = true;
}
_int CDaveGoldBall::Update_GameObject(const _float& fTimeDelta)
{
	CDaveGoldBall::Init();

	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
	Set_ParentTransform();
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	m_fViewZ = 10.1f;

	m_bIsFlip = static_cast<CDiveDave*>(m_pParentGameObject)->Is_Flip();

	CTransform* pParentTransformCom = static_cast<CDiveDave*>(m_pParentGameObject)->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform");

	_vec3 vDavePos;
	pParentTransformCom->Get_Info(INFO_POS, &vDavePos);
	if (vDavePos.y <= -130.f)
	{
		m_bRender = true;
	}
	else
	{
		m_bRender = false;
	}
	

	_float yAngle = pParentTransformCom->m_vAngle.y;
	_float zAngle = pParentTransformCom->m_vAngle.z;

	if (static_cast<CDiveDave*>(m_pParentGameObject)->Get_State() == DIVEDAVESTATE::MOVE)
	{
		m_pTransformCom->m_vAngle.x = 0.f;
		m_pTransformCom->m_vAngle.y = yAngle;
		m_pTransformCom->m_vAngle.z = zAngle;
	}
	else if (static_cast<CDiveDave*>(m_pParentGameObject)->Get_State() == DIVEDAVESTATE::IDLE)
	{
		m_pTransformCom->m_vAngle.x = 0.f;
		m_pTransformCom->m_vAngle.y = 0.f;
		m_pTransformCom->m_vAngle.z = 90.f;
	}
	return iExit;
}

void CDaveGoldBall::LateUpdate_GameObject(const _float& fTimeDelta)
{

	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CDaveGoldBall::Render_GameObject()
{
	if (!m_bRender)
		return;

	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());


	if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(m_sTexName))
	{
		if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
		{
			pGraphicDev->SetTexture(0, pTexture->Get_Texture());
		}
	}
	m_pBufferCom->Render_Buffer();

	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT CDaveGoldBall::Ready_Component()
{
	// 버퍼
	if (FAILED((AddComponent<Engine::CGoldBallBuffer, ID_STATIC>(L"Proto_GoldBallBuffer", L"Com_Buffer", &m_pBufferCom))))
		return E_FAIL;

	// 트랜스폼
	if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
		return E_FAIL;

	return S_OK;
}

void CDaveGoldBall::Set_ParentTransform()
{
	_vec3 vParentPos;
	m_pParentGameObject->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &vParentPos);
	m_pTransformCom->Set_Pos(vParentPos.x, vParentPos.y, vParentPos.z);
}

CDaveGoldBall* CDaveGoldBall::Create()
{
	CDaveGoldBall* pGoldBall= new CDaveGoldBall;

	if (FAILED(pGoldBall->Ready_GameObject()))
	{
		Safe_Release(pGoldBall);
		MSG_BOX("pGoldBall Create Failed");
		return nullptr;
	}
	return pGoldBall;
}

void CDaveGoldBall::Free()
{
	CGameObject::Free();
}
