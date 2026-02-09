#pragma once

#include "CAsset.h"

BEGIN(Engine)

class ENGINE_DLL CAssetTexture : public CAsset
{
private:
	explicit CAssetTexture(std::wstring_view m_sAssetPath);
	virtual ~CAssetTexture();

public:
	LPDIRECT3DTEXTURE9 const Get_Texture() const { return m_pTexture; }

public:
	HRESULT Load() override;
	HRESULT Unload() override;

public:
	static CAssetTexture* Create(std::wstring_view svPath);

private:
	LPDIRECT3DTEXTURE9 m_pTexture;

private:
	void Free() override;
};

END