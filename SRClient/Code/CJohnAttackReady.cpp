#include "CJohnAttackReady.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CGraphicDev.h"
#include "CJohn.h"
CJohnAttackReady::CJohnAttackReady(CJohn* pOwner)
	: CBaseState<CJohn>(pOwner)
{
}

CJohnAttackReady::~CJohnAttackReady()
{
}

void CJohnAttackReady::Enter()
{
	m_pOwner->Init_Frame();

	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"Tex_JohnTL_Aim")->at(0))->Get_ImgInfo();
	imgInfo.Width;

	_float fWidth = imgInfo.Width;;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	m_pOwner->Multiply_Scale(&vScale);
}

void CJohnAttackReady::Input(const _float& fTimeDelta)
{
}

_int CJohnAttackReady::Update_State(const _float& fTimeDelta)
{
	m_pOwner->AddFrame(fTimeDelta, 10.f, 2);

	m_pOwner->Update_ToTargetDir();

	_vec3 vRotDir;
	if (m_pOwner->Get_ToTargetDir().x > 0.f)
		vRotDir = { 0.f, 0.f, 0.f };
	else
		vRotDir = { 0.f,-180.f, 0.f };
	m_pOwner->Set_RotateDir(&vRotDir);

	m_fReadyTime += fTimeDelta;

	if (m_fReadyTime > 1.5f)
		m_pOwner->Set_State(JOHNSTATE::SHOT);
	return 0;
}

void CJohnAttackReady::LateUpdate_State(const _float& fTimeDelta)
{
}

void CJohnAttackReady::Render_State()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_JohnTL_Aim"))
	{
		_uint iFrame = (_uint)m_pOwner->Get_Frame();
		if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(iFrame)))
		{
			pGraphicDev->SetTexture(0, pTexture->Get_Texture());
		}
	}
}

void CJohnAttackReady::Exit()
{
	Clear();
}

void CJohnAttackReady::Clear()
{
	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"Tex_JohnTL_Aim")->at(0))->Get_ImgInfo();
	imgInfo.Width;

	_float fWidth = imgInfo.Width;;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
	m_pOwner->Multiply_Scale(&vScale);


	m_fReadyTime = 0.f;
}

CJohnAttackReady* CJohnAttackReady::Create(CJohn* pOwner)
{
	CJohnAttackReady* pState = new CJohnAttackReady(pOwner);

	return pState;
}

void CJohnAttackReady::Free()
{
}
