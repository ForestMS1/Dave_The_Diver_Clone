#include "CDiveDaveSubMarine.h"
#include "CDInputMgr.h"
#include "CDiveDave.h"
#include "CCameraMgr.h"
#include "CDiveDaveCam.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CColliderMgr.h"
#include "CFishGameObject.h"
CDiveDaveSubMarine::CDiveDaveSubMarine(CDiveDave* pOwner)
	:CBaseState<CDiveDave>(pOwner)
{
}

CDiveDaveSubMarine::~CDiveDaveSubMarine()
{
}

void CDiveDaveSubMarine::Enter()
{
	m_pOwner->Init_Frame();
	m_wsTexName = L"SubMarine";

	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(m_wsTexName)->at(0))->Get_ImgInfo();
	imgInfo.Width;

	_float fWidth = imgInfo.Width;;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	vScale *= 2.f;
	m_pOwner->Multiply_Scale(&vScale);
}

void CDiveDaveSubMarine::Input(const _float& fTimeDelta)
{
	if (CDInputMgr::GetInstance()->Key_Pressing(DIK_W))
	{
		if (CDInputMgr::GetInstance()->Key_Pressing(DIK_A))
			m_eDir = UP_LEFT;
		else if (CDInputMgr::GetInstance()->Key_Pressing(DIK_D))
			m_eDir = UP_RIGHT;
		else
			m_eDir = UP;
	}

	else if (CDInputMgr::GetInstance()->Key_Pressing(DIK_A))
	{
		if (CDInputMgr::GetInstance()->Key_Pressing(DIK_W))
			m_eDir = UP_LEFT;
		else if (CDInputMgr::GetInstance()->Key_Pressing(DIK_S))
			m_eDir = DOWN_LEFT;
		else
			m_eDir = LEFT;
	}

	else if (CDInputMgr::GetInstance()->Key_Pressing(DIK_S))
	{
		if (CDInputMgr::GetInstance()->Key_Pressing(DIK_A))
			m_eDir = DOWN_LEFT;
		else if (CDInputMgr::GetInstance()->Key_Pressing(DIK_D))
			m_eDir = DOWN_RIGHT;
		else
			m_eDir = DOWN;
	}
	else if (CDInputMgr::GetInstance()->Key_Pressing(DIK_D))
	{
		if (CDInputMgr::GetInstance()->Key_Pressing(DIK_W))
			m_eDir = UP_RIGHT;
		else if (CDInputMgr::GetInstance()->Key_Pressing(DIK_S))
			m_eDir = DOWN_RIGHT;
		else
			m_eDir = RIGHT;
	}

	else
	{
		m_eDir = DIR_END;
	}

}

_int CDiveDaveSubMarine::Update_State(const _float& fTimeDelta)
{
	Input(fTimeDelta);
	Restore_Fov(fTimeDelta);
	Go_Dir(fTimeDelta);
	//m_pOwner->AddFrame(fTimeDelta, 10.f, 1);

	if (!m_pOwner->Is_SubMarine())
		m_pOwner->Set_State(DIVEDAVESTATE::IDLE);

	return 0;
}

void CDiveDaveSubMarine::LateUpdate_State(const _float& fTimeDelta)
{
	// 잠수함이랑 물고기
	if (auto pColliders = CColliderMgr::GetInstance()->Get_Colliders(L"Coll_FishesHitbox"))
	{
		for (auto& pCollider : *pColliders)
		{
			if (m_pOwner->Get_AABB()->Intersect(pCollider))
			{
				if (pCollider->Get_Tag() == L"AABB_FishHitbox")
				{
					reinterpret_cast<CFishGameObject*>(pCollider->Get_VoidPtr())->Damaged(100000);
				}
			}
		}
	}
}

void CDiveDaveSubMarine::Render_State()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(m_wsTexName))
	{
		//_uint iFrame = (_uint)m_pOwner->Get_Frame();
		if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
		{
			pGraphicDev->SetTexture(0, pTexture->Get_Texture());
		}
	}
}

void CDiveDaveSubMarine::Exit()
{
	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(m_wsTexName)->at(0))->Get_ImgInfo();
	imgInfo.Width;

	_float fWidth = imgInfo.Width;;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
	vScale *= 0.5f;
	m_pOwner->Multiply_Scale(&vScale);

	_vec3 vRotateDir = { 0.f, 0.f, 0.f };
	m_pOwner->Set_RotateDir(&vRotateDir);
}

void CDiveDaveSubMarine::Clear()
{
}

void CDiveDaveSubMarine::Go_Dir(const _float& fTimeDelta)
{
	CDiveDave* pPlayer = m_pOwner;
	_vec3 vDir;
	_vec3 vRotateDir;

	switch (m_eDir)
	{
	case UP:
		vDir = { 0.f, 1.f, 0.f };
		pPlayer->Move(&vDir, fTimeDelta);
		vRotateDir = { 0.f, 0.f, 90.f };
		pPlayer->Set_RotateDir(&vRotateDir);
		break;
	case UP_RIGHT:
		vDir = { 1.f, 1.f, 0.f };
		D3DXVec3Normalize(&vDir, &vDir);
		pPlayer->Move(&vDir, fTimeDelta);
		vRotateDir = { 0.f, 0.f, 45.f };
		pPlayer->Set_RotateDir(&vRotateDir);
		break;
	case UP_LEFT:
		vDir = { -1.f, 1.f, 0.f };
		D3DXVec3Normalize(&vDir, &vDir);
		pPlayer->Move(&vDir, fTimeDelta);
		vRotateDir = { 0.f, -180.f, -45.f };
		pPlayer->Set_RotateDir(&vRotateDir);
		break;
	case RIGHT:
		vDir = { 1.f, 0.f, 0.f };
		pPlayer->Move(&vDir, fTimeDelta);
		vRotateDir = { 0.f, 0.f, 0.f };
		pPlayer->Set_RotateDir(&vRotateDir);
		break;
	case LEFT:
		vDir = { -1.f, 0.f, 0.f };
		pPlayer->Move(&vDir, fTimeDelta);
		vRotateDir = { 0.f, -180.f, 0.f };
		pPlayer->Set_RotateDir(&vRotateDir);
		break;
	case DOWN:
		vDir = { 0.f, -1.f, 0.f };
		pPlayer->Move(&vDir, fTimeDelta);
		vRotateDir = { 0.f, 0.f, -90.f };
		pPlayer->Set_RotateDir(&vRotateDir);
		break;
	case DOWN_RIGHT:
		vDir = { 1.f, -1.f, 0.f };
		D3DXVec3Normalize(&vDir, &vDir);
		pPlayer->Move(&vDir, fTimeDelta);
		vRotateDir = { 0.f, 0.f, -45.f };
		pPlayer->Set_RotateDir(&vRotateDir);
		break;
	case DOWN_LEFT:
		vDir = { -1.f, -1.f, 0.f };
		D3DXVec3Normalize(&vDir, &vDir);
		pPlayer->Move(&vDir, fTimeDelta);
		vRotateDir = { 0.f, -180.f, 45.f };
		pPlayer->Set_RotateDir(&vRotateDir);
		break;
	case DIR_END:
		break;
	default:
		break;
	}
}

void CDiveDaveSubMarine::Restore_Fov(const _float& fTimeDelta)
{
	CDiveDaveCam* pCam = static_cast<CDiveDaveCam*>(CCameraMgr::GetInstance()->Get_CurCamera());
	if (pCam == nullptr)
		return;
	if (D3DXToDegree(pCam->GetFov()) < 60.f)
		pCam->ZoomOut(fTimeDelta * 10.f);
}

CDiveDaveSubMarine* CDiveDaveSubMarine::Create(CDiveDave* pOwner)
{
	CDiveDaveSubMarine* pState = new CDiveDaveSubMarine(pOwner);

	return pState;
}

void CDiveDaveSubMarine::Free()
{
}
