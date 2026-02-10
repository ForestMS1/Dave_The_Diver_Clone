#pragma once
#include "CAsset.h"
#include "CSoundMgr.h"

BEGIN(Engine)

class ENGINE_DLL CAssetFmodSound : public CAsset
{
private:
	explicit CAssetFmodSound(std::wstring_view m_sAssetPath);
	virtual ~CAssetFmodSound();

public:
	FMOD_SOUND* Get_FmodSound() const { return m_pFmodSound; }

public:
	HRESULT Load() override;
	HRESULT Unload() override;

private:
	FMOD_SOUND* m_pFmodSound;

public:
	static CAssetFmodSound* Create(std::wstring_view m_sAssetPath);

private:
	void Free() override;
};

END