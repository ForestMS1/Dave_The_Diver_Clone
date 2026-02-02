#include "CFontMgr.h"

IMPLEMENT_SINGLETON(CFontMgr)

CFontMgr::CFontMgr()
{
}

CFontMgr::~CFontMgr()
{
	Free();
}

HRESULT CFontMgr::Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev, std::wstring_view svFontTag, std::wstring_view svFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	CFont* pMyFont = Find_Font(svFontTag);

	if (nullptr != pMyFont)
		return E_FAIL;

	pMyFont = CFont::Create(pGraphicDev, svFontType, iWidth, iHeight, iWeight);

	if (nullptr == pMyFont)
		return E_FAIL;

	m_mapFont.insert({ std::wstring(svFontTag), pMyFont });

	return S_OK;
}

void CFontMgr::Render_Font(std::wstring_view svFontTag, std::wstring_view svString,
	const _vec2* pPos, D3DXCOLOR Color)
{
	CFont* pFont = Find_Font(svFontTag);

	pFont->Render_Font(svString, pPos, Color);
}

CFont* CFontMgr::Find_Font(std::wstring_view svFontTag)
{
	auto	iter = find_if(m_mapFont.begin(), m_mapFont.end(), CTag_FinderSV(svFontTag));

	if (iter == m_mapFont.end())
		return nullptr;

	return iter->second;
}

void CFontMgr::Free()
{
	for_each(m_mapFont.begin(), m_mapFont.end(), CDeleteMap());
	m_mapFont.clear();
}
