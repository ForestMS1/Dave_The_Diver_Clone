#include "CJohn2MeleeAttackReturn.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CGraphicDev.h"
#include "CJohn2.h"
#include "CColliderMgr.h"
CJohn2MeleeAttackReturn::CJohn2MeleeAttackReturn(CJohn2* pOwner)
	: CBaseState<CJohn2>(pOwner)
{
}

CJohn2MeleeAttackReturn::~CJohn2MeleeAttackReturn()
{
}

void CJohn2MeleeAttackReturn::Enter()
{
	m_pOwner->Init_Frame();
	m_wsTexName = L"John2MeleeSkill01_Return";

	Set_Size();

	m_pOwner->Update_ToTargetDir();
	m_pOwner->Reset_BreakTime();
}

void CJohn2MeleeAttackReturn::Input(const _float& fTimeDelta)
{
}

_int CJohn2MeleeAttackReturn::Update_State(const _float& fTimeDelta)
{
	m_pOwner->AddFrame(fTimeDelta, 2.f, 2, false);

	if(m_pOwner->Get_Frame() == 1)
		m_pOwner->Set_State(JOHN2STATE::IDLE);

	return 0;
}

void CJohn2MeleeAttackReturn::LateUpdate_State(const _float& fTimeDelta)
{

}

void CJohn2MeleeAttackReturn::Render_State()
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

void CJohn2MeleeAttackReturn::Exit()
{
	Clear();
}

void CJohn2MeleeAttackReturn::Clear()
{
	Reset_Size();
}

CJohn2MeleeAttackReturn* CJohn2MeleeAttackReturn::Create(CJohn2* pOwner)
{
	CJohn2MeleeAttackReturn* pState = new CJohn2MeleeAttackReturn(pOwner);

	return pState;
}

void CJohn2MeleeAttackReturn::Free()
{
}

void CJohn2MeleeAttackReturn::Reset_Size()
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

void CJohn2MeleeAttackReturn::Set_Size()
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