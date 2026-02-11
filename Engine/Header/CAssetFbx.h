#pragma once

#include "CAsset.h"

BEGIN(Engine)

class ENGINE_DLL CAssetFbx : public CAsset
{
private:
	explicit CAssetFbx(std::wstring_view m_sAssetPath);
	virtual ~CAssetFbx();

public:
	HRESULT Load() override;
	HRESULT Unload() override;

public:
	static CAssetFbx* Create(std::wstring_view m_sAssetPath);

private:
	// 클라에 assimp를 노출안시키기위한 pimpl 패턴
	struct Impl;   
	Impl* m_pImpl; 

private:
	void Free() override;
};


END