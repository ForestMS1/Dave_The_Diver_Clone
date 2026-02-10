#include "CAssetFmodSound.h"
#include "fmod.h"
#include "fmod.hpp"
#include "CHelper.h"

CAssetFmodSound::CAssetFmodSound(std::wstring_view m_sAssetPath)
	: CAsset(m_sAssetPath)
	, m_pFmodSound(nullptr)
{
}
CAssetFmodSound::~CAssetFmodSound()
{
}

HRESULT CAssetFmodSound::Load()
{
	m_eAssetState = LOADING;

	FMOD_RESULT eRes = FMOD_System_CreateSound(
		CSoundMgr::GetInstance()->Get_System(),
		CHelper::WStringToString(m_sAssetPath).c_str(),
		FMOD_DEFAULT, 0, &m_pFmodSound);

	if (eRes != FMOD_OK)
	{
		m_eAssetState = LOADFAIL;
		wstring msg = L"CAssetFmodSound Create Failed, Path: " + m_sAssetPath;
		MSG_BOX_STR(msg.c_str());
		return E_FAIL;
	}
	m_eAssetState = LOADED;
	return S_OK;
}

HRESULT CAssetFmodSound::Unload()
{
	FMOD_Sound_Release(m_pFmodSound);
	m_pFmodSound = nullptr;
	m_eAssetState = UNLOAD;
	return S_OK;
}

void CAssetFmodSound::Free()
{
	Unload();
	CAsset::Free();
}

CAssetFmodSound* CAssetFmodSound::Create(std::wstring_view m_sAssetPath)
{
	return new CAssetFmodSound{ m_sAssetPath };
}