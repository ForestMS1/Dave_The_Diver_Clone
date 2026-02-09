#pragma once

#include "CAsset.h"

class ENGINE_DLL CAssetCubeTexture : public CAsset
{
private:
	explicit CAssetCubeTexture(std::wstring_view m_sAssetPath);
	virtual ~CAssetCubeTexture();

public:
	LPDIRECT3DCUBETEXTURE9 const Get_CubeTexture() const { return m_pCubeTexture; }

public:
	HRESULT Load() override;
	HRESULT Unload() override;

public:
	static CAssetCubeTexture* Create(std::wstring_view svPath);

private:
	LPDIRECT3DCUBETEXTURE9 m_pCubeTexture;

private:
	void Free() override;
};

