#include "CGunSkill02Aim.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CGraphicDev.h"
#include "CJohn2.h"
#include "CColliderMgr.h"
#include "CJohnMine.h"
#include "CSoundMgr.h"
CGunSkill02Aim::CGunSkill02Aim(CJohn2* pOwner)
	: CBaseState<CJohn2>(pOwner)
{
}

CGunSkill02Aim::~CGunSkill02Aim()
{
}

void CGunSkill02Aim::Enter()
{
	m_pOwner->Init_Frame();
	m_wsTexName = L"John2GunSkill02_Aim";

	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(m_wsTexName)->at(0))->Get_ImgInfo();
	imgInfo.Width;

	_float fWidth = imgInfo.Width;;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	m_pOwner->Multiply_Scale(&vScale);
}

void CGunSkill02Aim::Input(const _float& fTimeDelta)
{
}

_int CGunSkill02Aim::Update_State(const _float& fTimeDelta)
{
	m_pOwner->AddFrame(fTimeDelta, 5.f, 3, true);

	m_fSplashDelay += fTimeDelta;
	if (m_fSplashDelay > 3.f)
	{
		m_wsTexName = L"John2GunSkill02_Shoot";
		m_pOwner->Splash_Mine();
		m_pOwner->Reset_BreakTime();

		m_fSplashDelay = 0.f;
		m_iSplashCnt++;
	}

	if (m_wsTexName == L"John2GunSkill02_Shoot")
		m_fChangeAnimTime += fTimeDelta;

	if (m_fChangeAnimTime >= 0.5f)
	{
		m_wsTexName = L"John2GunSkill02_Aim";
		m_fChangeAnimTime = 0.f;
	}

	if (m_iSplashCnt > 3)
	{
		m_pOwner->Set_State(JOHN2STATE::IDLE);
	}

	return 0;
}

void CGunSkill02Aim::LateUpdate_State(const _float& fTimeDelta)
{

}

void CGunSkill02Aim::Render_State()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(m_wsTexName))
	{
		_uint iFrame = (_uint)m_pOwner->Get_Frame();
		if (m_wsTexName == L"John2GunSkill02_Shoot")
			iFrame = 0;

		if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(iFrame)))
		{
			pGraphicDev->SetTexture(0, pTexture->Get_Texture());
		}
	}
}

void CGunSkill02Aim::Exit()
{
	Clear();
}

void CGunSkill02Aim::Clear()
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

CGunSkill02Aim* CGunSkill02Aim::Create(CJohn2* pOwner)
{
	CGunSkill02Aim* pState = new CGunSkill02Aim(pOwner);

	return pState;
}

void CGunSkill02Aim::Free()
{
}