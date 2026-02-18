#include "CAssetTextureFromInMem.h"
#include "CGraphicDev.h"
CAssetTextureFromInMem::CAssetTextureFromInMem(void * pData, _uint iSize)
	: CAssetTexture({})
	, m_iSize(iSize)
	, m_pData(pData)
{
}
CAssetTextureFromInMem::~CAssetTextureFromInMem()
{
}

HRESULT CAssetTextureFromInMem::Load()
{
	m_eAssetState = LOADING;
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
	HRESULT hr = D3DXCreateTextureFromFileInMemory(
		pGraphicDev,
		m_pData,
		m_iSize,
		&m_pTexture);

	if (FAILED(hr))
	{
		m_eAssetState = LOADFAIL;
		wstring msg = L"CAssetTextureFromInMem Load Fail, Path: " + m_sAssetPath;
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

HRESULT CAssetTextureFromInMem::Unload()
{
	Safe_Release(m_pTexture);
	m_eAssetState = UNLOAD;
	return S_OK;
}

CAssetTextureFromInMem* CAssetTextureFromInMem::Create(void* pData, _uint iSize)
{
	return new CAssetTextureFromInMem{ pData , iSize};
}

void CAssetTextureFromInMem::Free()
{
	Unload();
	CAsset::Free();
}
