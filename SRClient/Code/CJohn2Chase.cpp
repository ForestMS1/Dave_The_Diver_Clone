#include "CJohn2Chase.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CGraphicDev.h"
#include "CJohn2.h"
#include "CManagement.h"
CJohn2Chase::CJohn2Chase(CJohn2* pOwner)
	: CBaseState<CJohn2>(pOwner)
{
}

CJohn2Chase::~CJohn2Chase()
{
}

void CJohn2Chase::Enter()
{
	m_pOwner->Init_Frame();
	m_wsTexName = L"John2Move";

	Set_Size();

	m_pOwnerTransform = m_pOwner->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform");
}

void CJohn2Chase::Input(const _float& fTimeDelta)
{
}

_int CJohn2Chase::Update_State(const _float& fTimeDelta)
{
	m_pOwner->Update_ToTargetDir();

	_vec3 vRotDir;
	if (m_pOwner->Get_ToTargetDir().x > 0.f)
		vRotDir = { 0.f, 0.f, 0.f };
	else
		vRotDir = { 0.f,-180.f, 0.f };
	m_pOwner->Set_RotateDir(&vRotDir);


	MoveTo_Target(fTimeDelta);
	m_pOwner->AddFrame(fTimeDelta, 10.f, 8);

	return 0;
}

void CJohn2Chase::LateUpdate_State(const _float& fTimeDelta)
{
}

void CJohn2Chase::Render_State()
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

void CJohn2Chase::Exit()
{
	Clear();
}

void CJohn2Chase::Clear()
{
	Reset_Size();

	_vec3 vRotateDir = { 0.f, 0.f, 0.f };
	m_pOwner->Set_RotateDir(&vRotateDir);
}

CJohn2Chase* CJohn2Chase::Create(CJohn2* pOwner)
{
	CJohn2Chase* pState = new CJohn2Chase(pOwner);

	return pState;
}

void CJohn2Chase::MoveTo_Target(const _float& fTimeDelta)
{
	if (m_pOwnerTransform == nullptr)
		return;


	// 근접했다면 더 쫓아가지 않음.
	if (m_pOwner->Check_TargetInRange())
	{
		m_pOwner->Set_State(JOHN2STATE::IDLE);
		return;
	}

	_vec3 vDir = m_pOwner->Get_ToTargetDir();
	D3DXVec3Normalize(&vDir, &vDir);
	m_pOwnerTransform->Move_Pos(&vDir, 5.f, fTimeDelta);
}

void CJohn2Chase::Reset_Size()
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

void CJohn2Chase::Set_Size()
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

void CJohn2Chase::Free()
{
}
