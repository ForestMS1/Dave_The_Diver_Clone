#include "CAsset.h"

CAsset::CAsset(std::wstring_view svAssetPath)
	: m_eAssetState(UNLOAD)
	, m_sAssetPath(svAssetPath)
	
{
}

CAsset::~CAsset()
{
}
