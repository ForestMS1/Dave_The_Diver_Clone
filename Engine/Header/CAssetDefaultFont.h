#pragma once
#include "CAsset.h"

BEGIN(Engine)

class ENGINE_DLL CAssetDefaultFont : public CAsset
{
private:
	explicit CAssetDefaultFont(std::wstring_view svFontType,
		const _uint& iWidth,
		const _uint& iHeight,
		const _uint& iWeight);
	explicit CAssetDefaultFont(std::wstring_view svPath, 
		std::wstring_view svFontType,
		const _uint& iWidth,
		const _uint& iHeight,
		const _uint& iWeight);
	virtual ~CAssetDefaultFont();

public:
	LPD3DXFONT Get_Font() const { return m_pFont ; }

public:
	HRESULT Load() override;
	HRESULT Unload() override;

public:
	void Render_Font(std::wstring_view svString, const _vec2* pPos, D3DXCOLOR Color);
	void Render_Font(std::wstring_view svString, const _vec2* pPos, D3DXCOLOR Color, DWORD dwOption);

public:
	static CAssetDefaultFont* Create(std::wstring_view svFontType,
		const _uint& iWidth,
		const _uint& iHeight,
		const _uint& iWeight);

	static CAssetDefaultFont* Create(std::wstring_view svPath,
		std::wstring_view svFontType,
		const _uint& iWidth,
		const _uint& iHeight,
		const _uint& iWeight);

private:
	LPD3DXSPRITE				m_pSprite;		// 2d 텍스처 출력 담당
	LPD3DXFONT					m_pFont;		// 폰트 출력 담당

	const std::wstring m_sFontType;
	const _uint m_iWidth;
	const _uint m_iHeight;
	const _uint m_iWeight;

private:
	void Free() override;
};

END