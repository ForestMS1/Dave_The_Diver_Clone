#include "pch.h"
#include "CJohn2Die.h"
#include "CJohn2.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
CJohn2Die::CJohn2Die(CJohn2* pOwner)
	:CBaseState<CJohn2>(pOwner)
{
}

CJohn2Die::~CJohn2Die()
{
}

void CJohn2Die::Enter()
{
	m_pOwner->Init_Frame();
	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"JohnWatson2_DeathMove")->at(0))->Get_ImgInfo();

	_float fWidth = imgInfo.Width;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	m_pOwner->Multiply_Scale(&vScale);
}

void CJohn2Die::Input(const _float& fTimeDelta)
{

}

_int CJohn2Die::Update_State(const _float& fTimeDelta)
{
	Input(fTimeDelta);
	m_pOwner->AddFrame(fTimeDelta, 5.f, 3, false);

	if (m_pOwner->Get_Frame() == 2)
		m_pOwner->Set_State(JOHN2STATE::EXPLOSION);

	return 0;
}

void CJohn2Die::LateUpdate_State(const _float& fTimeDelta)
{
}

void CJohn2Die::Render_State()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"JohnWatson2_DeathMove"))
	{
		_uint iFrame = (_uint)m_pOwner->Get_Frame();
		if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(iFrame)))
		{
			pGraphicDev->SetTexture(0, pTexture->Get_Texture());
		}
	}
}

void CJohn2Die::Exit()
{
	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"JohnWatson2_DeathMove")->at(0))->Get_ImgInfo();

	_float fWidth = imgInfo.Width;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;
	_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
	m_pOwner->Multiply_Scale(&vScale);

	Clear();
}

void CJohn2Die::Clear()
{

}


CJohn2Die* CJohn2Die::Create(CJohn2* pOwner)
{
	CJohn2Die* pState = new CJohn2Die(pOwner);

	return pState;
}

void CJohn2Die::Free()
{
}
