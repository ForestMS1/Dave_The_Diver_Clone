#include "CJohnIdle.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CGraphicDev.h"
#include "CJohn.h"
CJohnIdle::CJohnIdle(CJohn* pOwner)
    : CBaseState<CJohn>(pOwner)
{
}

CJohnIdle::~CJohnIdle()
{
}

void CJohnIdle::Enter()
{
	m_pOwner->Init_Frame();

	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"Tex_JohnIdle")->at(0))->Get_ImgInfo();
	imgInfo.Width;

	_float fWidth = imgInfo.Width;;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	m_pOwner->Multiply_Scale(&vScale);
}

void CJohnIdle::Input(const _float& fTimeDelta)
{
}

_int CJohnIdle::Update_State(const _float& fTimeDelta)
{
	m_pOwner->AddFrame(fTimeDelta, 10.f, 8);


	if (!m_pOwner->Check_TargetInRange())
	{
		m_pOwner->Set_State(JOHNSTATE::CHASE);
	}
	else
	{
		m_pOwner->Set_State(JOHNSTATE::ATTACK_READY);
	}
    return 0;
}

void CJohnIdle::LateUpdate_State(const _float& fTimeDelta)
{
}

void CJohnIdle::Render_State()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_JohnIdle"))
	{
		_uint iFrame = (_uint)m_pOwner->Get_Frame();
		if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(iFrame)))
		{
			pGraphicDev->SetTexture(0, pTexture->Get_Texture());
		}
	}
}

void CJohnIdle::Exit()
{
	Clear();
}

void CJohnIdle::Clear()
{
	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"Tex_JohnIdle")->at(0))->Get_ImgInfo();
	imgInfo.Width;

	_float fWidth = imgInfo.Width;;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
	m_pOwner->Multiply_Scale(&vScale);
}

CJohnIdle* CJohnIdle::Create(CJohn* pOwner)
{
    CJohnIdle* pState = new CJohnIdle(pOwner);

    return pState;
}

void CJohnIdle::Free()
{
}
