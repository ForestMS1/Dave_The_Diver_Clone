#include "pch.h"
#include "CJohn2PantiesSwim.h"
#include "CJohn2.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
CJohn2PantiesSwim::CJohn2PantiesSwim(CJohn2* pOwner)
	:CBaseState<CJohn2>(pOwner)
{
}

CJohn2PantiesSwim::~CJohn2PantiesSwim()
{
}

void CJohn2PantiesSwim::Enter()
{
	m_pOwner->Init_Frame();
	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"JohnWatson2_PantiesSwim")->at(0))->Get_ImgInfo();

	_float fWidth = imgInfo.Width;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	vScale *= 0.4f;
	m_pOwner->Multiply_Scale(&vScale);
}

void CJohn2PantiesSwim::Input(const _float& fTimeDelta)
{

}

_int CJohn2PantiesSwim::Update_State(const _float& fTimeDelta)
{
	Input(fTimeDelta);
	m_pOwner->AddFrame(fTimeDelta, 5.f, 7, false);

	_vec3 vDir = { 0.f, 1.f, 0.f };
	//m_pOwner->Move(&vDir, fTimeDelta);
	m_pOwner->GetTransformCom()->Move_Pos(&vDir, 2.f, fTimeDelta);

	if (m_pOwner->Get_Frame() == 6)
		m_pOwner->Set_State(JOHN2STATE::SLICABLE);


	return 0;
}

void CJohn2PantiesSwim::LateUpdate_State(const _float& fTimeDelta)
{
}

void CJohn2PantiesSwim::Render_State()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"JohnWatson2_PantiesSwim"))
	{
		_uint iFrame = (_uint)m_pOwner->Get_Frame();
		if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(iFrame)))
		{
			pGraphicDev->SetTexture(0, pTexture->Get_Texture());
		}
	}
}

void CJohn2PantiesSwim::Exit()
{
	D3DXIMAGE_INFO imgInfo = *static_cast<CAssetTexture*>(CAssetMgr::GetInstance()->Get_Asset(L"JohnWatson2_PantiesSwim")->at(0))->Get_ImgInfo();

	_float fWidth = imgInfo.Width;
	_float fHeight = imgInfo.Height;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;
	_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
	vScale /= 0.4f;
	m_pOwner->Multiply_Scale(&vScale);

	Clear();
}

void CJohn2PantiesSwim::Clear()
{

}


CJohn2PantiesSwim* CJohn2PantiesSwim::Create(CJohn2* pOwner)
{
	CJohn2PantiesSwim* pState = new CJohn2PantiesSwim(pOwner);

	return pState;
}

void CJohn2PantiesSwim::Free()
{
}
