#pragma once
#include "CAssetTexture.h"

BEGIN(Engine)

class ENGINE_DLL CAssetTextureFromInMem : public CAssetTexture
{
private:
	explicit CAssetTextureFromInMem(void * pData, _uint iSize);
	virtual ~CAssetTextureFromInMem();

public:
	HRESULT Load() override;
	HRESULT Unload() override;

public:
	static CAssetTextureFromInMem* Create(void* pData, _uint iSize);

private:
	void * m_pData;
	const _uint m_iSize;

private:
	void Free() override;
};

END