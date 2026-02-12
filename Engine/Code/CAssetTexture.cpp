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
	

	HRESULT hr = D3DXCreateTextureFromFileEx(
		pGraphicDev,           // 디바이스
		m_sAssetPath.c_str(), // 파일 경로
		D3DX_DEFAULT_NONPOW2,                     // Width (가로 크기 지정)
		D3DX_DEFAULT_NONPOW2,                     // Height (세로 크기 지정)
		D3DX_DEFAULT,                       // MipLevels (1로 설정 시 미팹 생성 안 함)
		0,                       // Usage (기본값 0)
		D3DFMT_UNKNOWN,          // Format (파일 포맷 그대로 사용)
		D3DPOOL_MANAGED,         // Pool (메모리 관리 방식)
		D3DX_DEFAULT,            // Filter (확대/축소 필터)
		D3DX_DEFAULT,            // MipFilter (미팹 필터)
		0,                       // ColorKey (투명화 처리할 색상, 0은 없음)
		&m_ImgInfo,                    // pSrcInfo (원본 정보 구조체 주소, 필요 없으면 NULL)
		NULL,                    // pPalette (팔레트 정보)
		&m_pTexture                // 결과 저장 변수
	);

	if (FAILED(hr))
	{
		m_eAssetState = LOADFAIL;
		wstring msg = L"CAssetTexture Load Fail, Path: " + m_sAssetPath;
		MSG_BOX_STR(msg.c_str());
		return E_FAIL;
	}

	//if (FAILED(D3DXCreateTextureFromFile(pGraphicDev, m_sAssetPath.c_str(), &m_pTexture)))
	//{
	//	m_eAssetState = LOADFAIL;
	//	wstring msg = L"CAssetTexture Load Fail, Path: " + m_sAssetPath;
	//	MSG_BOX_STR(msg.c_str());
	//	return E_FAIL;
	//}

	//if (FAILED(D3DXGetImageInfoFromFile(m_sAssetPath.c_str(), &m_ImgInfo)))
	//{
	//	m_eAssetState = LOADFAIL;
	//	wstring msg = L"CAssetTexture GetImgInfo Fail, Path: " + m_sAssetPath;
	//	MSG_BOX_STR(msg.c_str());
	//	return E_FAIL;
	//}


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
