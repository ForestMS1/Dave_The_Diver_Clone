#include "CAssetDefaultFont.h"
#include "CInfoMgr.h"

#include "CGraphicDev.h"

CAssetDefaultFont::CAssetDefaultFont(std::wstring_view svFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
	: CAsset({})
	, m_sFontType(svFontType)
	, m_iWidth(iWidth)
	, m_iHeight(iHeight)
	, m_iWeight(iWeight)
	, m_pSprite(nullptr)
	, m_pFont(nullptr)
{
}

CAssetDefaultFont::~CAssetDefaultFont()
{
}

HRESULT CAssetDefaultFont::Load()
{
	m_eAssetState = LOADING;
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
	D3DXFONT_DESC			tFont_Desc;
	ZeroMemory(&tFont_Desc, sizeof(D3DXFONT_DESC));

	_float fScaleFactor = CInfoMgr::GetInstance()->Get_ScaleFactor();
	_float scaledWidth = m_iWidth * fScaleFactor;
	_float scaledHeight = m_iHeight * fScaleFactor;

	tFont_Desc.CharSet = HANGUL_CHARSET;
	tFont_Desc.Width = (UINT)scaledWidth;
	tFont_Desc.Height = (UINT)scaledHeight;
	tFont_Desc.Weight = m_iWeight;
	lstrcpy(tFont_Desc.FaceName, m_sFontType.data());

	if (FAILED(D3DXCreateFontIndirect(pGraphicDev, &tFont_Desc, &m_pFont)))
	{
		m_eAssetState = LOADFAIL;
		wstring msg = L"CAssetDefaultFont Font Create Failed, Path: " + m_sAssetPath;
		MSG_BOX_STR(msg.c_str());
		return E_FAIL;
	}

	if (FAILED(D3DXCreateSprite(pGraphicDev, &m_pSprite)))
	{
		m_eAssetState = LOADFAIL;
		wstring msg = L"CAssetDefaultFont Sprite Create Failed, Path: " + m_sAssetPath;
		MSG_BOX_STR(msg.c_str());
		return E_FAIL;
	}
	m_eAssetState = LOADED;

	return S_OK;
}

void CAssetDefaultFont::Render_Font(std::wstring_view svString, const _vec2* pPos, D3DXCOLOR Color)
{
	if (m_pSprite != nullptr && m_pFont != nullptr)
	{
		RECT rc{ (_long)pPos->x, (_long)pPos->y };

		m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

		m_pFont->DrawTextW(m_pSprite, svString.data(), lstrlen(svString.data()), &rc, DT_NOCLIP, Color);

		m_pSprite->End();
	}
}

HRESULT CAssetDefaultFont::Unload()
{
	// TODO: FIXME 이거왜 오류나는지?
	// DEVICE LOST 되면 지워야다시 만들어야한다는데 그거 안해서 그런가??
	//Safe_Release(m_pSprite);
	//Safe_Release(m_pFont);
	m_eAssetState = UNLOAD;
	return S_OK;
}

CAssetDefaultFont* CAssetDefaultFont::Create(std::wstring_view svFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	return new CAssetDefaultFont{ svFontType, iWidth, iHeight, iWeight };
}

void CAssetDefaultFont::Free()
{
	Unload();
	CAsset::Free();
}
