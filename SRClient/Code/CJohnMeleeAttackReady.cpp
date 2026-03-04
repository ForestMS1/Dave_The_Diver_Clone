#include "CJohnMeleeAttackReady.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CGraphicDev.h"
#include "CJohn.h"
CJohnMeleeAttackReady::CJohnMeleeAttackReady(CJohn* pOwner)
	: CBaseState<CJohn>(pOwner)
{
}

CJohnMeleeAttackReady::~CJohnMeleeAttackReady()
{
}

void CJohnMeleeAttackReady::Enter()
{
	m_pOwner->Init_Frame();

	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"Tex_JohnMeleeSkill01_Ready")->at(0))->Get_ImgInfo();
	imgInfo.Width;

	_float fWidth = imgInfo.Width;;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	m_pOwner->Multiply_Scale(&vScale);
}

void CJohnMeleeAttackReady::Input(const _float& fTimeDelta)
{
}

_int CJohnMeleeAttackReady::Update_State(const _float& fTimeDelta)
{
	m_pOwner->AddFrame(fTimeDelta, 10.f, 8);
	m_pOwner->Update_ToTargetDir();

	m_fReadyTime += fTimeDelta;

	if (m_fReadyTime > 4.f)
	{
		//m_pOwner->Shot_Bullet();
		m_pOwner->Set_State(JOHNSTATE::MELEEATTACK);
	}

	return 0;
}

void CJohnMeleeAttackReady::LateUpdate_State(const _float& fTimeDelta)
{
}

void CJohnMeleeAttackReady::Render_State()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_JohnMeleeSkill01_Ready"))
	{
		_uint iFrame = (_uint)m_pOwner->Get_Frame();
		if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(iFrame)))
		{
			pGraphicDev->SetTexture(0, pTexture->Get_Texture());
		}
	}
}

void CJohnMeleeAttackReady::Exit()
{
	Clear();
}

void CJohnMeleeAttackReady::Clear()
{
	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"Tex_JohnMeleeSkill01_Ready")->at(0))->Get_ImgInfo();
	imgInfo.Width;

	_float fWidth = imgInfo.Width;;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
	m_pOwner->Multiply_Scale(&vScale);
	

	m_fReadyTime = 0.f;
}

CJohnMeleeAttackReady* CJohnMeleeAttackReady::Create(CJohn* pOwner)
{
	CJohnMeleeAttackReady* pState = new CJohnMeleeAttackReady(pOwner);

	return pState;
}

void CJohnMeleeAttackReady::Free()
{
}
