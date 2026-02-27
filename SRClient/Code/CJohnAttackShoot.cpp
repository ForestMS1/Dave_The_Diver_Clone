#include "CJohnAttackShoot.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CGraphicDev.h"
#include "CJohn.h"
CJohnAttackShoot::CJohnAttackShoot(CJohn* pOwner)
	: CBaseState<CJohn>(pOwner)
{
}

CJohnAttackShoot::~CJohnAttackShoot()
{
}

void CJohnAttackShoot::Enter()
{
	m_pOwner->Init_Frame();

	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"Tex_JohnTL_Shoot")->at(0))->Get_ImgInfo();
	imgInfo.Width;

	_float fWidth = imgInfo.Width;;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	m_pOwner->Multiply_Scale(&vScale);
}

void CJohnAttackShoot::Input(const _float& fTimeDelta)
{
}

_int CJohnAttackShoot::Update_State(const _float& fTimeDelta)
{
	m_pOwner->AddFrame(fTimeDelta, 10.f, 2);

	if (m_pOwner->Get_Frame() >= 1.8f)
		m_pOwner->Set_State(JOHNSTATE::IDLE);

	return 0;
}

void CJohnAttackShoot::LateUpdate_State(const _float& fTimeDelta)
{
}

void CJohnAttackShoot::Render_State()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_JohnTL_Shoot"))
	{
		_uint iFrame = (_uint)m_pOwner->Get_Frame();
		if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(iFrame)))
		{
			pGraphicDev->SetTexture(0, pTexture->Get_Texture());
		}
	}
}

void CJohnAttackShoot::Exit()
{
	Clear();
}

void CJohnAttackShoot::Clear()
{
	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"Tex_JohnTL_Shoot")->at(0))->Get_ImgInfo();
	imgInfo.Width;

	_float fWidth = imgInfo.Width;;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
	m_pOwner->Multiply_Scale(&vScale);
}

CJohnAttackShoot* CJohnAttackShoot::Create(CJohn* pOwner)
{
	CJohnAttackShoot* pState = new CJohnAttackShoot(pOwner);

	return pState;
}

void CJohnAttackShoot::Free()
{
}
