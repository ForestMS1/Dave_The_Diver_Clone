#pragma once
#include "CAssetTexture.h"

BEGIN(Engine)

class ENGINE_DLL CAssetTextureFromCreate : public CAssetTexture
{
private:
	explicit CAssetTextureFromCreate(_uint iWidth, _uint iHeight);
	virtual ~CAssetTextureFromCreate();

public:
	HRESULT Load() override;
	HRESULT Unload() override;

public:
	static CAssetTextureFromCreate* Create(_uint iWidth, _uint iHeight);

private:
	const _uint m_iWidth;
	const _uint m_iHeight;

private:
	void Free() override;
};

END