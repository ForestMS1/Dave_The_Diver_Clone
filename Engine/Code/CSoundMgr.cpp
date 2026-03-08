#include <io.h>
#include "CSoundMgr.h" 
#include "fmod.h"
#include "fmod.hpp"
#pragma comment(lib, "fmod_vc.lib")

#include "CAssetMgr.h"
#include "CAssetFmodSound.h"

IMPLEMENT_SINGLETON(CSoundMgr)

CSoundMgr::CSoundMgr()
	: m_pSystem(nullptr)
{
	ZeroMemory(&m_pChannelArr, sizeof(m_pChannelArr));
}

CSoundMgr::~CSoundMgr()
{
	Free();
}

HRESULT CSoundMgr::Ready_SoundMgr()
{
	// 사운드를 담당하는 대표객체를 생성하는 함수
	FMOD_System_Create(&m_pSystem, FMOD_VERSION);

	FMOD_System_SetDSPBufferSize(m_pSystem, 2048, 4);

	// 1. 시스템 포인터, 2. 사용할 가상채널 수 , 초기화 방식) 
	FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL);

	return S_OK;
}


void CSoundMgr::PlaySoundLoop(std::wstring_view svSoundKey, CHANNELID eID, float fVolume)
{
	if (auto pSound = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetFmodSound>(svSoundKey)->Get_FmodSound())
	{
		FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);
		FMOD_System_PlaySound(m_pSystem, pSound, 0, FALSE, &m_pChannelArr[eID]);
		FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);
		FMOD_Channel_SetMode(m_pChannelArr[eID], FMOD_LOOP_NORMAL);
		//// 사운드 원본에 루프 모드 설정
		//FMOD_Sound_SetMode(pSound, FMOD_LOOP_NORMAL);

		//// 바로 재생 (볼륨은 재생 직후 설정)
		//FMOD_System_PlaySound(m_pSystem, pSound, nullptr, FALSE, &m_pChannelArr[eID]);
		//FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);
	}

	//FMOD_System_Update(m_pSystem);
}

void CSoundMgr::PlaySoundOne(std::wstring_view svSoundKey, CHANNELID eID, float fVolume)
{
	//if (auto pSound = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetFmodSound>(svSoundKey)->Get_FmodSound())
	//{
	//	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);
	//	FMOD_System_PlaySound(m_pSystem, pSound, 0, FALSE, &m_pChannelArr[eID]);
	//	FMOD_Channel_SetMode(m_pChannelArr[eID], FMOD_DEFAULT);
	//	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);
	//	
	//}
	//FMOD_System_Update(m_pSystem);

	auto pAsset = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetFmodSound>(svSoundKey);
	if (!pAsset) return;

	FMOD_SOUND* pSound = pAsset->Get_FmodSound();
	if (pSound)
	{
		// 1. 우선 일시정지(Paused) 상태로 사운드 생성 (세 번째 인자 TRUE)
		FMOD_System_PlaySound(m_pSystem, pSound, nullptr, TRUE, &m_pChannelArr[eID]);

		// 2. 채널 핸들이 유효한지 확인 후 설정 적용
		if (m_pChannelArr[eID])
		{
			FMOD_Channel_SetMode(m_pChannelArr[eID], FMOD_DEFAULT); // 루프 해제 등 기본 설정
			FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);    // 여기서 0.f가 확실히 적용됨

			// 3. 설정이 끝났으니 일시정지 해제 (소리 출력 시작)
			FMOD_Channel_SetPaused(m_pChannelArr[eID], FALSE);
		}
	}

	// Update는 매 프레임 메인 루프에서 한 번만 호출하는 것이 성능상 좋습니다.
	//FMOD_System_Update(m_pSystem);
}

bool CSoundMgr::IsChannelPlaying(CHANNELID eID)
{
	FMOD_BOOL bPlay = FALSE;
	FMOD_Channel_IsPlaying(m_pChannelArr[eID], &bPlay);
	return bPlay;
}

void CSoundMgr::StopSound(CHANNELID eID)
{
	FMOD_Channel_Stop(m_pChannelArr[eID]);
}

void CSoundMgr::StopAll()
{
	for (int i = 0; i < MAXCHANNEL; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

void CSoundMgr::SetChannelVolume(CHANNELID eID, float fVolume)
{
	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);

	//FMOD_System_Update(m_pSystem);
}

void CSoundMgr::UpdateSound()
{
	FMOD_System_Update(m_pSystem);
}


void CSoundMgr::Free()
{
	FMOD_System_Close(m_pSystem);
	FMOD_System_Release(m_pSystem);
}