#include "pch.h"
#include "CPlayerGun.h"
#include "CManagement.h"
#include "CDSPlayer.h"
#include "CPlayerCam.h"
#include "CGraphicDev.h"
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
	if (m_pParentGameObject == nullptr)
	{
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
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	//ÃÑ ¹ß»ç ¸ð¼Ç
	if (m_pParentGameObject != nullptr)
	{
		PlayerState ePlayerState = static_cast<CDSPlayer*>(m_pParentGameObject)->Get_State();

		if (ePlayerState == PlayerState::ATTACK)
		{

		}
		else
		{

		}
	}

	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CPlayerGun::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CPlayerGun::Render_GameObject()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
	//_matrix ortho;
	//D3DXMatrixOrthoLH(&ortho, WINCX, WINCY, 1.f, 100.f);
	//pGraphicDev->SetTransform(D3DTS_PROJECTION, &ortho);

	pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
	//CPlayerCam* pCam = dynamic_cast<CPlayerCam*>
	//	(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"Environment_Layer")->Get_GameObjectFirst(L"PlayerCam"));
	//_matrix origin = pCam->Get_ProjMatrix();
	//pGraphicDev->SetTransform(D3DTS_PROJECTION, &origin);

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
