#include "CAssetTexture.h"
#include "CGraphicDev.h"
CAssetTexture::CAssetTexture(std::wstring_view m_sAssetPath)
	: CAsset(m_sAssetPath)
	, m_pTexture(nullptr)
{
}

CAssetTexture::~CAssetTexture()
{
}

HRESULT CAssetTexture::Load()
{
	m_eAssetState = LOADING;
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
	
	if (FAILED(D3DXCreateTextureFromFile(pGraphicDev, m_sAssetPath.c_str(), &m_pTexture)))
	{
		m_eAssetState = LOADFAIL;
		wstring msg = L"CAssetTexture Load Fail, Path: " + m_sAssetPath;
		MSG_BOX_STR(msg.c_str());
		return E_FAIL;
	}
	m_eAssetState = LOADED;
	return S_OK;
}

HRESULT CAssetTexture::Unload()
{
	Safe_Release(m_pTexture);
	m_eAssetState = UNLOAD;
	return S_OK;
}

CAssetTexture* CAssetTexture::Create(std::wstring_view svPath)
{
	return new CAssetTexture{ svPath };
}

void CAssetTexture::Free()
{
	Unload();
	CAsset::Free();
}
