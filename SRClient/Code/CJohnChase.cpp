#include "CJohnChase.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CGraphicDev.h"
#include "CJohn.h"
#include "CManagement.h"
CJohnChase::CJohnChase(CJohn* pOwner)
	: CBaseState<CJohn>(pOwner)
{
}

CJohnChase::~CJohnChase()
{
}

void CJohnChase::Enter()
{
	m_pOwner->Init_Frame();
	m_wsTexName = L"Tex_JohnIdle";

	Set_Size();

	m_pOwnerTransform = m_pOwner->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform");
}

void CJohnChase::Input(const _float& fTimeDelta)
{
}

_int CJohnChase::Update_State(const _float& fTimeDelta)
{
	Switch_TexName();
	MoveTo_Target(fTimeDelta);
	m_pOwner->AddFrame(fTimeDelta, 10.f, 8);

	return 0;
}

void CJohnChase::LateUpdate_State(const _float& fTimeDelta)
{
}

void CJohnChase::Render_State()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(m_wsTexName))
	{
		_uint iFrame = (_uint)m_pOwner->Get_Frame();
		if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(iFrame)))
		{
			pGraphicDev->SetTexture(0, pTexture->Get_Texture());
		}
	}
}

void CJohnChase::Exit()
{
	Clear();
}

void CJohnChase::Clear()
{
	Reset_Size();

	_vec3 vRotateDir = { 0.f, 0.f, 0.f };
	m_pOwner->Set_RotateDir(&vRotateDir);
}

CJohnChase* CJohnChase::Create(CJohn* pOwner)
{
	CJohnChase* pState = new CJohnChase(pOwner);

	return pState;
}

void CJohnChase::MoveTo_Target(const _float& fTimeDelta)
{
	if (m_pOwnerTransform == nullptr)
		return;


	// 근접했다면 더 쫓아가지 않음.
	if (m_pOwner->Check_TargetInRange())
	{
		m_pOwner->Set_State(JOHNSTATE::IDLE);
		return;
	}

	_vec3 vDir = m_pOwner->Get_ToTargetDir();
	D3DXVec3Normalize(&vDir, &vDir);
	m_pOwnerTransform->Move_Pos(&vDir, 5.f, fTimeDelta);
}

void CJohnChase::Switch_TexName()
{
	Reset_Size();
	_vec3 vDir = m_pOwner->Get_ToTargetDir();

	if (vDir.y > 0.f)
	{
		if (vDir.x > 0.f)
			m_eDir = UP_RIGHT;
		else if (vDir.x < 0.f)
			m_eDir = UP_LEFT;
		else if (vDir.x < 1.f || vDir.x > -1.f)
			m_eDir = UP;
	}
	else if (vDir.y < 0.f)
	{
		if (vDir.x > 0.f)
			m_eDir = DOWN_RIGHT;
		else if (vDir.x < 0.f)
			m_eDir = DOWN_LEFT;
		else if (vDir.x < 1.f || vDir.x > -1.f)
			m_eDir = DOWN;
	}
	else if (vDir.y < 1.f || vDir.y > -1.f && vDir.x <= 0.f)
		m_eDir = LEFT;
	else if (vDir.y < 1.f || vDir.y > -1.f && vDir.x >= 0.f)
		m_eDir = RIGHT;

	_vec3 vRotateDir;

	switch (m_eDir)
	{
	case UP:
		m_wsTexName = L"Tex_JohnMoveUp";
		vRotateDir = { 0.f, 0.f, 90.f };
		m_pOwner->Set_RotateDir(&vRotateDir);
		break;
	case UP_RIGHT:
		m_wsTexName = L"Tex_JohnMoveSideUp";
		vRotateDir = { 0.f, 0.f, 45.f };
		m_pOwner->Set_RotateDir(&vRotateDir);
		break;
	case UP_LEFT:
		m_wsTexName = L"Tex_JohnMoveSideUp";
		vRotateDir = { 0.f, -180.f, -45.f };
		m_pOwner->Set_RotateDir(&vRotateDir);
		break;
	case RIGHT:
		m_wsTexName = L"Tex_JohnMoveSide";
		vRotateDir = { 0.f, 0.f, 0.f };
		m_pOwner->Set_RotateDir(&vRotateDir);
		break;
	case LEFT:
		m_wsTexName = L"Tex_JohnMoveSide";
		vRotateDir = { 0.f, -180.f, 0.f };
		m_pOwner->Set_RotateDir(&vRotateDir);
		break;
	case DOWN:
		m_wsTexName = L"Tex_JohnMoveDown";
		vRotateDir = { 0.f, 0.f, -90.f };
		m_pOwner->Set_RotateDir(&vRotateDir);
		break;
	case DOWN_RIGHT:
		m_wsTexName = L"Tex_JohnMoveSideDown";
		vRotateDir = { 0.f, 0.f, -45.f };
		m_pOwner->Set_RotateDir(&vRotateDir);
		break;
	case DOWN_LEFT:
		m_wsTexName = L"Tex_JohnMoveSideDown";
		vRotateDir = { 0.f, -180.f, 45.f };
		m_pOwner->Set_RotateDir(&vRotateDir);
		break;
	case DIR_END:
		break;
	default:
		break;
	}
	Set_Size();
}

void CJohnChase::Reset_Size()
{
	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(m_wsTexName)->at(0))->Get_ImgInfo();
	imgInfo.Width;

	_float fWidth = imgInfo.Width;;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
	m_pOwner->Multiply_Scale(&vScale);
}

void CJohnChase::Set_Size()
{
	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(m_wsTexName)->at(0))->Get_ImgInfo();
	imgInfo.Width;

	_float fWidth = imgInfo.Width;;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	m_pOwner->Multiply_Scale(&vScale);
}

void CJohnChase::Free()
{
}
