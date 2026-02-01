#include <io.h>
#include "CSoundMgr.h" 
#include "fmod.h"
#include "fmod.hpp"
#pragma comment(lib, "fmod_vc.lib")

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

	// 1. 시스템 포인터, 2. 사용할 가상채널 수 , 초기화 방식) 
	FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL);

	LoadSoundFile();
	return S_OK;
}

void CSoundMgr::Update_SoundMgr()
{
}

void CSoundMgr::PlaySoundLoop(const _tchar* pSoundKey, CHANNELID eID, float fVolume)
{
	map<const _tchar*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(),
		[&](auto& iter)->bool
		{
			return !lstrcmp(pSoundKey, iter.first);
		});

	if (iter == m_mapSound.end())
		return;

	//FMOD_BOOL bPlay = FALSE;
	//if (FMOD_Channel_IsPlaying(m_pChannelArr[eID], &bPlay))
	//{
	//}
	//if (bPlay)
	//{
	//	FMOD_Channel_Stop(m_pChannelArr[eID]);
	//}

	FMOD_System_PlaySound(m_pSystem, iter->second, 0, FALSE, &m_pChannelArr[eID]);
	FMOD_Channel_SetMode(m_pChannelArr[eID], FMOD_LOOP_NORMAL);
	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);
	

	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::PlaySoundOne(const _tchar* pSoundKey, CHANNELID eID, float fVolume)
{
	map<const _tchar*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(),
		[&](auto& iter)->bool
		{
			return !lstrcmp(pSoundKey, iter.first);
		});

	if (iter == m_mapSound.end())
		return;

	//FMOD_BOOL bPlay = FALSE;
	//if (FMOD_Channel_IsPlaying(m_pChannelArr[eID], &bPlay))
	//{
	//}
	FMOD_System_PlaySound(m_pSystem, iter->second, 0, FALSE, &m_pChannelArr[eID]);
	FMOD_Channel_SetMode(m_pChannelArr[eID], FMOD_DEFAULT);
	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);

	FMOD_System_Update(m_pSystem);
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

	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::LoadSoundFile()
{
	// _finddata_t : <io.h>에서 제공하며 파일 정보를 저장하는 구조체
	_finddata_t fd;

	// TODO: 파일 찾는거 루트가 어떻게 실행하냐로 바뀔수 있다.
	// visualStudio에서 실행하는거랑 exe눌러서 하는거랑 서치가 다를수 있다 
	// 지금 서치를 하드코딩으로하는데 변경하면 좋겠다.
	// 
	// 현재 워킹디렉토리 확인
	//#include <direct.h>
	//char cwd[1024];
	//_getcwd(cwd, sizeof(cwd))
	
	// _findfirst : <io.h>에서 제공하며 사용자가 설정한 경로 내에서 가장 첫 번째 파일을 찾는 함수
	long long handle = _findfirst("../Bin/Resource/Sound/*.*", &fd);

	if (handle == -1)
		return;

	int iResult = 0;

	char szCurPath[128] = "../Bin/Resource/Sound/";
	char szFullPath[128] = "";

	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPath);

		// "../ Sound/Success.wav"
		strcat_s(szFullPath, fd.name);

		FMOD_SOUND* pSound = nullptr;

		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath,
			FMOD_DEFAULT, 0, &pSound);

		if (eRes == FMOD_OK)
		{
			int iLength = strlen(fd.name) + 1;

			TCHAR* pSoundKey = new TCHAR[iLength];
			ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);

			// 아스키 코드 문자열을 유니코드 문자열로 변환시켜주는 함수
			MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pSoundKey, iLength);

			m_mapSound.insert({ pSoundKey, pSound });
		}
		//_findnext : <io.h>에서 제공하며 다음 위치의 파일을 찾는 함수, 
		// 더이상 없다면 -1을 리턴
		iResult = _findnext(handle, &fd);

	}

	FMOD_System_Update(m_pSystem);

	_findclose(handle);
}

void CSoundMgr::Free()
{
	for (auto& Mypair : m_mapSound)
	{
		delete[] Mypair.first;
		FMOD_Sound_Release(Mypair.second);
	}
	m_mapSound.clear();

	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}