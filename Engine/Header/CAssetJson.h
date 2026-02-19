#pragma once
#include "CAsset.h"
#include "Engine_Define.h"

BEGIN(Engine)


class ENGINE_DLL CAssetJson : public CAsset
{
private:
	explicit CAssetJson(std::wstring_view m_sAssetPath);
	virtual ~CAssetJson();


public:
	void   Save(nlohmann::json _fileJson);
	const nlohmann::json  Get_Json() { return m_jsonFile; }

public:
	static CAssetJson* Create(std::wstring_view m_sAssetPath);

private:
	nlohmann::json m_jsonFile;
public:

	HRESULT Load() override;
	HRESULT Unload() override;
};

END