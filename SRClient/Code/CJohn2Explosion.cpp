#include "pch.h"
#include "CJohn2Explosion.h"
#include "CJohn2.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CSoundMgr.h"
CJohn2Explosion::CJohn2Explosion(CJohn2* pOwner)
	:CBaseState<CJohn2>(pOwner)
{
}

CJohn2Explosion::~CJohn2Explosion()
{
}

void CJohn2Explosion::Enter()
{
	m_pOwner->Init_Frame();
	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"JohnBulletExplosion")->at(0))->Get_ImgInfo();

	_float fWidth = imgInfo.Width;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	m_pOwner->Multiply_Scale(&vScale);

	CSoundMgr::GetInstance()->PlaySoundOne(L"Sound_JW2_Explosion_01", CSoundMgr::BOSS_SFX, 1.f);
}

void CJohn2Explosion::Input(const _float& fTimeDelta)
{

}

_int CJohn2Explosion::Update_State(const _float& fTimeDelta)
{
	Input(fTimeDelta);
	m_pOwner->AddFrame(fTimeDelta, 5.f, 9, false);

	if (m_pOwner->Get_Frame() == 8)
		m_pOwner->Set_State(JOHN2STATE::PANTIES);

	return 0;
}

void CJohn2Explosion::LateUpdate_State(const _float& fTimeDelta)
{
}

void CJohn2Explosion::Render_State()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"JohnBulletExplosion"))
	{
		_uint iFrame = (_uint)m_pOwner->Get_Frame();
		if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(iFrame)))
		{
			pGraphicDev->SetTexture(0, pTexture->Get_Texture());
		}
	}
}

void CJohn2Explosion::Exit()
{
	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"JohnBulletExplosion")->at(0))->Get_ImgInfo();
	
	_float fWidth = imgInfo.Width;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;
	
	_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
	m_pOwner->Multiply_Scale(&vScale);

	Clear();
}

void CJohn2Explosion::Clear()
{

}


CJohn2Explosion* CJohn2Explosion::Create(CJohn2* pOwner)
{
	CJohn2Explosion* pState = new CJohn2Explosion(pOwner);

	return pState;
}

void CJohn2Explosion::Free()
{
}
