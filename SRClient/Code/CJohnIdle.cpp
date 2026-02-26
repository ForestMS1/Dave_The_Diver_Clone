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
}

void CJohnIdle::Input(const _float& fTimeDelta)
{
}

_int CJohnIdle::Update_State(const _float& fTimeDelta)
{
	m_pOwner->AddFrame(fTimeDelta, 10.f, 8);

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
}

CJohnIdle* CJohnIdle::Create(CJohn* pOwner)
{
    CJohnIdle* pState = new CJohnIdle(pOwner);

    return pState;
}

void CJohnIdle::Free()
{
}
