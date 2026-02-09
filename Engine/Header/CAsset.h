#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CAsset : public CBase
{
public:
	enum AssetState
	{
		UNLOAD,
		LOADING,
		LOADFAIL,
		LOADED
	};

protected:
	explicit CAsset(std::wstring_view m_sAssetPath);
	virtual ~CAsset();

public:
	void Set_Tag(){}

public:
	virtual HRESULT Load() = 0;
	virtual HRESULT Unload() = 0;
	AssetState Get_State() const { return m_eAssetState; }

protected:
	std::wstring m_sAssetTag;
	std::wstring m_sAssetPath;
	AssetState m_eAssetState;

public:
	void Free() override;
};

END

