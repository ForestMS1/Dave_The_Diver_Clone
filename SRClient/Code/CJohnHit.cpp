#include "pch.h"
#include "CJohnHit.h"
#include "CJohn.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
CJohnHit::CJohnHit(CJohn* pOwner)
	:CBaseState<CJohn>(pOwner)
{
}

CJohnHit::~CJohnHit()
{
}

void CJohnHit::Enter()
{
	m_pOwner->Init_Frame();
	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"JohnHit")->at(0))->Get_ImgInfo();
	imgInfo.Width;

	_float fWidth = imgInfo.Width;;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	m_pOwner->Multiply_Scale(&vScale);
}

void CJohnHit::Input(const _float& fTimeDelta)
{

}

_int CJohnHit::Update_State(const _float& fTimeDelta)
{
	Input(fTimeDelta);
	m_pOwner->Acc_HitTime(fTimeDelta);
	m_pOwner->Add_BreakTime(fTimeDelta);
	if (m_pOwner->Get_HitTime() > 0.5f)
		m_pOwner->Set_State(JOHNSTATE::IDLE);
	//m_pOwner->AddFrame(fTimeDelta, 10.f, 2);
	return 0;
}

void CJohnHit::LateUpdate_State(const _float& fTimeDelta)
{
}

void CJohnHit::Render_State()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"JohnHit"))
	{
		//_uint iFrame = (_uint)m_pOwner->Get_Frame();
		if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
		{
			pGraphicDev->SetTexture(0, pTexture->Get_Texture());
		}
	}
}

void CJohnHit::Exit()
{
	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"JohnHit")->at(0))->Get_ImgInfo();
	imgInfo.Width;

	_float fWidth = imgInfo.Width;;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;
	_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
	m_pOwner->Multiply_Scale(&vScale);

	Clear();
}

void CJohnHit::Clear()
{
	m_pOwner->Hit_Free();
	m_pOwner->Reset_HitTime();
}


CJohnHit* CJohnHit::Create(CJohn* pOwner)
{
	CJohnHit* pState = new CJohnHit(pOwner);

	return pState;
}

void CJohnHit::Free()
{
}
