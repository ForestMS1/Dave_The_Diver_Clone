#include "CAssetTextureFromCreate.h"
#include "CGraphicDev.h"

CAssetTextureFromCreate::CAssetTextureFromCreate(_uint iWidth, _uint iHeight)
	: CAssetTexture({})
	, m_iWidth(iWidth)
	, m_iHeight(iHeight)
{
}
CAssetTextureFromCreate::~CAssetTextureFromCreate()
{
}

HRESULT CAssetTextureFromCreate::Load()
{
	m_eAssetState = LOADING;
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
	HRESULT hr = D3DXCreateTexture(
		pGraphicDev,                         
		m_iWidth,
		m_iHeight,
		1,                              
		0,
		D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		&m_pTexture);


	if (FAILED(hr))
	{
		m_eAssetState = LOADFAIL;
		wstring msg = L"CAssetTextureFromCreate Load Fail, Path: " + m_sAssetPath;
		MSG_BOX_STR(msg.c_str());
		return E_FAIL;
	}

	D3DSURFACE_DESC desc;
	m_pTexture->GetLevelDesc(0, &desc);
	m_ImgInfo.Width = desc.Width;
	m_ImgInfo.Height = desc.Height;
	m_ImgInfo.Format = desc.Format;
	m_ImgInfo.MipLevels = 1;
	m_ImgInfo.ResourceType = D3DRTYPE_TEXTURE;

	m_eAssetState = LOADED;
	return S_OK;
}

HRESULT CAssetTextureFromCreate::Unload()
{
	Safe_Release(m_pTexture);
	m_eAssetState = UNLOAD;
	return S_OK;
}

CAssetTextureFromCreate* CAssetTextureFromCreate::Create(_uint iWidth, _uint iHeight)
{
	return new CAssetTextureFromCreate{ iWidth , iHeight };
}

void CAssetTextureFromCreate::Free()
{
	Unload();
	CAsset::Free();
}
