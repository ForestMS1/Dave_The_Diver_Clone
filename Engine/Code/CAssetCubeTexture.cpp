#include "CAssetCubeTexture.h"
#include "CGraphicDev.h"
CAssetCubeTexture::CAssetCubeTexture(std::wstring_view m_sAssetPath)
	: CAsset(m_sAssetPath)
	, m_pCubeTexture(nullptr)
{
}

CAssetCubeTexture::~CAssetCubeTexture()
{
}

HRESULT CAssetCubeTexture::Load()
{
	m_eAssetState = LOADING;
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	if (FAILED(D3DXCreateCubeTextureFromFile(pGraphicDev, m_sAssetPath.c_str(), &m_pCubeTexture)))
	{
		m_eAssetState = LOADFAIL;
		wstring msg = L"CAssetCubeTexture Load Fail, Path: " + m_sAssetPath;
		MSG_BOX_STR(msg.c_str());
		return E_FAIL;
	}
	m_eAssetState = LOADED;
	return S_OK;
}

HRESULT CAssetCubeTexture::Unload()
{
	Safe_Release(m_pCubeTexture);
	m_eAssetState = UNLOAD;
	return S_OK;
}

CAssetCubeTexture* CAssetCubeTexture::Create(std::wstring_view svPath)
{
	return new CAssetCubeTexture{ svPath };
}

void CAssetCubeTexture::Free()
{
	Unload();
	CAsset::Free();
}
