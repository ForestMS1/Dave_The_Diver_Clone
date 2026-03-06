#include "pch.h"
#include "CJohn2Hit.h"
#include "CJohn2.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
CJohn2Hit::CJohn2Hit(CJohn2* pOwner)
	:CBaseState<CJohn2>(pOwner)
{
}

CJohn2Hit::~CJohn2Hit()
{
}

void CJohn2Hit::Enter()
{
	m_pOwner->Init_Frame();
	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"John2Hit")->at(0))->Get_ImgInfo();
	imgInfo.Width;

	_float fWidth = imgInfo.Width;;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	m_pOwner->Multiply_Scale(&vScale);
}

void CJohn2Hit::Input(const _float& fTimeDelta)
{

}

_int CJohn2Hit::Update_State(const _float& fTimeDelta)
{
	Input(fTimeDelta);
	m_pOwner->AddFrame(fTimeDelta, 10.f, 2, true);
	m_pOwner->Acc_HitTime(fTimeDelta);
	if (m_pOwner->Get_HitTime() > 0.5f)
		m_pOwner->Set_State(JOHN2STATE::IDLE);
	//m_pOwner->AddFrame(fTimeDelta, 10.f, 2);
	return 0;
}

void CJohn2Hit::LateUpdate_State(const _float& fTimeDelta)
{
}

void CJohn2Hit::Render_State()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"John2Hit"))
	{
		//_uint iFrame = (_uint)m_pOwner->Get_Frame();
		if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
		{
			pGraphicDev->SetTexture(0, pTexture->Get_Texture());
		}
	}
}

void CJohn2Hit::Exit()
{
	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"John2Hit")->at(0))->Get_ImgInfo();
	imgInfo.Width;

	_float fWidth = imgInfo.Width;;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;
	_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
	m_pOwner->Multiply_Scale(&vScale);

	Clear();
}

void CJohn2Hit::Clear()
{
	m_pOwner->Hit_Free();
	m_pOwner->Reset_HitTime();
}


CJohn2Hit* CJohn2Hit::Create(CJohn2* pOwner)
{
	CJohn2Hit* pState = new CJohn2Hit(pOwner);

	return pState;
}

void CJohn2Hit::Free()
{
}
