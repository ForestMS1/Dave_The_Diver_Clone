#include "CJohnBattleAngry.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CGraphicDev.h"
#include "CJohn.h"
#include "CColliderMgr.h"
#include "CJohnMine.h"
CJohnBattleAngry::CJohnBattleAngry(CJohn* pOwner)
	: CBaseState<CJohn>(pOwner)
{
}

CJohnBattleAngry::~CJohnBattleAngry()
{
}

void CJohnBattleAngry::Enter()
{
	m_pOwner->Init_Frame();
	m_wsTexName = L"JohnBattleAngry";

	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(m_wsTexName)->at(0))->Get_ImgInfo();
	imgInfo.Width;

	_float fWidth = imgInfo.Width;;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	m_pOwner->Multiply_Scale(&vScale);
}

void CJohnBattleAngry::Input(const _float& fTimeDelta)
{
}

_int CJohnBattleAngry::Update_State(const _float& fTimeDelta)
{
	m_pOwner->AddFrame(fTimeDelta, 5.f, 7, true);

	m_fSplashDelay += fTimeDelta;
	if (m_fSplashDelay > 3.f)
	{
		//for (int i = 0; i < 15; ++i)
			m_pOwner->Splash_Mine();

		m_fSplashDelay = 0.f;
		m_iSplashCnt++;
	}

	if (m_iSplashCnt > 3)
		m_pOwner->Set_State(JOHNSTATE::IDLE);

	return 0;
}

void CJohnBattleAngry::LateUpdate_State(const _float& fTimeDelta)
{

}

void CJohnBattleAngry::Render_State()
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

void CJohnBattleAngry::Exit()
{
	Clear();
}

void CJohnBattleAngry::Clear()
{
	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(m_wsTexName)->at(0))->Get_ImgInfo();
	imgInfo.Width;

	_float fWidth = imgInfo.Width;;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
	m_pOwner->Multiply_Scale(&vScale);

	m_fSplashDelay = 0.f;
	m_iSplashCnt = 0;
}

CJohnBattleAngry* CJohnBattleAngry::Create(CJohn* pOwner)
{
	CJohnBattleAngry* pState = new CJohnBattleAngry(pOwner);

	return pState;
}

void CJohnBattleAngry::Free()
{
}