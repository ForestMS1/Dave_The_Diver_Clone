#include "CJohn2AttackShoot.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CGraphicDev.h"
#include "CJohn2.h"
CJohn2AttackShoot::CJohn2AttackShoot(CJohn2* pOwner)
	: CBaseState<CJohn2>(pOwner)
{
}

CJohn2AttackShoot::~CJohn2AttackShoot()
{
}

void CJohn2AttackShoot::Enter()
{
	m_pOwner->Init_Frame();

	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"John2GunSkill01_Shoot")->at(0))->Get_ImgInfo();
	imgInfo.Width;

	_float fWidth = imgInfo.Width;;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	m_pOwner->Multiply_Scale(&vScale);
}

void CJohn2AttackShoot::Input(const _float& fTimeDelta)
{
}

_int CJohn2AttackShoot::Update_State(const _float& fTimeDelta)
{
	m_pOwner->AddFrame(fTimeDelta, 10.f, 4, false);
	m_pOwner->Update_ToTargetDir();

	if (m_pOwner->Get_Frame() == 3)
	{
		m_pOwner->Shot_Bullet();
		m_pOwner->Set_State(JOHN2STATE::IDLE);
	}

	return 0;
}

void CJohn2AttackShoot::LateUpdate_State(const _float& fTimeDelta)
{
}

void CJohn2AttackShoot::Render_State()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"John2GunSkill01_Shoot"))
	{
		_uint iFrame = (_uint)m_pOwner->Get_Frame();
		if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(iFrame)))
		{
			pGraphicDev->SetTexture(0, pTexture->Get_Texture());
		}
	}
}

void CJohn2AttackShoot::Exit()
{
	Clear();
}

void CJohn2AttackShoot::Clear()
{
	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"John2GunSkill01_Shoot")->at(0))->Get_ImgInfo();
	imgInfo.Width;

	_float fWidth = imgInfo.Width;;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
	m_pOwner->Multiply_Scale(&vScale);
}

CJohn2AttackShoot* CJohn2AttackShoot::Create(CJohn2* pOwner)
{
	CJohn2AttackShoot* pState = new CJohn2AttackShoot(pOwner);

	return pState;
}

void CJohn2AttackShoot::Free()
{
}
