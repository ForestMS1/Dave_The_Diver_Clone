#include "pch.h"
#include "CPlayerGun.h"
#include "CManagement.h"
#include "CDSPlayer.h"
#include "CPlayerCam.h"
#include "CGraphicDev.h"
#include "CPlayerBullet.h"
CPlayerGun::CPlayerGun()
	: m_pBufferCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pTransformCom(nullptr)
{
}

CPlayerGun::CPlayerGun(const CPlayerGun& rhs)
{
}

CPlayerGun::~CPlayerGun()
{
}

HRESULT CPlayerGun::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;
	return S_OK;
}

_int CPlayerGun::Update_GameObject(const _float& fTimeDelta)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
	Set_ParentTransform(fTimeDelta);
	Shot(fTimeDelta);
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CPlayerGun::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Set_BillBoard();
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CPlayerGun::Render_GameObject()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

#ifdef _DEBUG
	_vec3 vPos, vLook;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	ImGui::Begin("PlayerGun");
	ImGui::DragFloat3("Pos", (float*)&vPos, 1.f - 1.f, 1.f);
	ImGui::DragFloat3("Look", (float*)&vLook, 1.f - 1.f, 1.f);
	ImGui::DragFloat3("Scale", (float*)&m_pTransformCom->m_vScale, 1.f - 1.f, 1.f);
	ImGui::End();
#endif
}

HRESULT	CPlayerGun::Add_Component()
{
	CComponent* pComponent = nullptr;

	// Buffer
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>
		(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTex"));

	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	// Texture
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>
		(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_GunTexture"));
	m_mapComponent[ID_STATIC].insert({ L"Com_Texutre", pComponent });

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>
		(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));

	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });


	return S_OK;
}

void CPlayerGun::Set_ParentTransform(const _float& fTimeDelta)
{
	if (m_pParentGameObject == nullptr)
	{
		// Ready에서 해주면 씬 넘어오기 전에 Object를 찾게되어 에러
		CGameObject* pPlayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"Environment_Layer")->Get_GameObjectFirst(L"Player");
		Set_Parent(pPlayer);
	}
	else
	{
		CTransform* pPlayerTransform = dynamic_cast<CTransform*>(m_pParentGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
		_vec3 vPlayerPos, vPlayerLook;
		pPlayerTransform->Get_Info(INFO_POS, &vPlayerPos);
		pPlayerTransform->Get_Info(INFO_LOOK, &vPlayerLook);

		_vec3 vPos = vPlayerPos + vPlayerLook * 10.f;
		m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
	}
}

void CPlayerGun::Shot(const _float& fTimeDelta)
{
	// 총알 발사
	if (m_pParentGameObject != nullptr)
	{
		PlayerState ePlayerState = static_cast<CDSPlayer*>(m_pParentGameObject)->Get_State();

		if (ePlayerState == PlayerState::ATTACK)
		{
			CTransform* pPlayerTransform = dynamic_cast<CTransform*>(m_pParentGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
			_vec3 vPlayerLook;
			pPlayerTransform->Get_Info(INFO_LOOK, &vPlayerLook);

			_vec3 vCurPos;
			m_pTransformCom->Get_Info(INFO_POS, &vCurPos);
			CGameObject* pBullet = CPlayerBullet::Create(vCurPos, vPlayerLook, 20.f);
			CManagement::GetInstance()->Get_Scene()->Get_Layer(L"GameLogic_Layer")->Add_GameObject(L"PlayerBullet", pBullet);
		}
	}
}

void CPlayerGun::Set_BillBoard()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
	_matrix		matBill, matWorld, matView;

	matWorld = *m_pTransformCom->Get_World();
	pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixIdentity(&matBill);

	// y축 회전만 제거
	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, 0, &matBill);

	// 주의 할 것
	matWorld = matBill * matWorld;

	m_pTransformCom->Set_World(&matWorld);

	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Compute_ViewZ(&vPos);
}

CPlayerGun* CPlayerGun::Create()
{
	CPlayerGun* pGun = new CPlayerGun;
	if (FAILED(pGun->Ready_GameObject()))
	{
		Safe_Release(pGun);
		MSG_BOX("Gun Create Failed");
		return nullptr;
	}
	return pGun;
}

void CPlayerGun::Free()
{
	CGameObject::Free();
}
