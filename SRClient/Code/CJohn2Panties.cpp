#include "pch.h"
#include "CJohn2Panties.h"
#include "CJohn2.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
CJohn2Panties::CJohn2Panties(CJohn2* pOwner)
	:CBaseState<CJohn2>(pOwner)
{
}

CJohn2Panties::~CJohn2Panties()
{
}

void CJohn2Panties::Enter()
{
	m_pOwner->Init_Frame();
	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"JohnWatson2_Panties")->at(0))->Get_ImgInfo();

	_float fWidth = imgInfo.Width;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	vScale *= 0.4f;
	m_pOwner->Multiply_Scale(&vScale);
}

void CJohn2Panties::Input(const _float& fTimeDelta)
{

}

_int CJohn2Panties::Update_State(const _float& fTimeDelta)
{
	Input(fTimeDelta);
	m_pOwner->AddFrame(fTimeDelta, 5.f, 4, false);

	if (m_pOwner->Get_Frame() == 3)
		m_pOwner->Set_State(JOHN2STATE::PANTIES_SWIM);

	return 0;
}

void CJohn2Panties::LateUpdate_State(const _float& fTimeDelta)
{
}

void CJohn2Panties::Render_State()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"JohnWatson2_Panties"))
	{
		_uint iFrame = (_uint)m_pOwner->Get_Frame();
		if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(iFrame)))
		{
			pGraphicDev->SetTexture(0, pTexture->Get_Texture());
		}
	}
}

void CJohn2Panties::Exit()
{
	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"JohnWatson2_Panties")->at(0))->Get_ImgInfo();

	_float fWidth = imgInfo.Width;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;
	_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
	vScale /= 0.4f;
	m_pOwner->Multiply_Scale(&vScale);

	Clear();
}

void CJohn2Panties::Clear()
{

}


CJohn2Panties* CJohn2Panties::Create(CJohn2* pOwner)
{
	CJohn2Panties* pState = new CJohn2Panties(pOwner);

	return pState;
}

void CJohn2Panties::Free()
{
}
