#include "CAssetJson.h"
#include <fstream>
CAssetJson::CAssetJson(std::wstring_view m_sAssetPath) : CAsset(m_sAssetPath)
{

}

CAssetJson::~CAssetJson()
{
}

void CAssetJson::Save(nlohmann::json _fileJson)
{
	using json = nlohmann::json;

	std::ofstream file(m_sAssetPath);

	if (file.is_open()) {
		file << _fileJson;
	
		file.close();
		m_eAssetState = LOADED;
	
	}
	else {
		m_eAssetState = LOADFAIL;
		MSG_BOX("File Open FAILED");
	}

	m_jsonFile = _fileJson;
}



HRESULT CAssetJson::Load()
{
	m_eAssetState = LOADING;

	using json = nlohmann::json;

	std::ifstream file(m_sAssetPath);
    
	if (file.is_open()) {
		file >> m_jsonFile;
		
		file.close();
		//MSG_BOX("File Load");
	}
	else {
		m_eAssetState = LOADFAIL;
		MSG_BOX("File Open FAILED");
	}





    m_eAssetState = LOADED;
    return S_OK;
}

HRESULT CAssetJson::Unload()
{
    m_eAssetState = UNLOAD;
    return S_OK;
}

CAssetJson* CAssetJson::Create(std::wstring_view m_sAssetPath)
{
    return new CAssetJson{ m_sAssetPath  };
}