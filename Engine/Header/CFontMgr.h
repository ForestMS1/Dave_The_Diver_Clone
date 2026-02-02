#pragma once

#include "CBase.h"
#include "Engine_Define.h"
#include "CFont.h"

BEGIN(Engine)

class ENGINE_DLL CFontMgr : public CBase
{
	DECLARE_SINGLETON(CFontMgr)

private:
	explicit CFontMgr();
	virtual ~CFontMgr();

public:
	HRESULT				Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev,
									std::wstring_view svFontTag,
									std::wstring_view svFontType,
									const _uint& iWidth,
									const _uint& iHeight,
									const _uint& iWeight);

	void				Render_Font(std::wstring_view svFontTag,
									std::wstring_view pString,
									const _vec2* pPos,
									D3DXCOLOR Color);

private:
	CFont* Find_Font(std::wstring_view svFontTag);

private:
	map<const std::wstring, CFont*>			m_mapFont;

private:
	virtual void	Free();
};

END