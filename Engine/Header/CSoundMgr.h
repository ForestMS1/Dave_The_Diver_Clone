#pragma once
#include "CBase.h"
#include "Engine_Define.h"

// 전방선언
// 클라에서 fmod 헤더 노출안시키기위해
struct FMOD_SOUND;
struct FMOD_CHANNEL;
struct FMOD_SYSTEM;

BEGIN(Engine)

// TODO1:
// 쌤코드인데
// 현재 사운드 여러번 플레이하면 중첩되서 소리가 나는거로 기능 개발함 
// 기존 사운드 중지시키고 새롭게 사운드 내는게 필요할수도
// TODO2:
// 사운드 로딩 시점을 싱글톤 레디시점에 할거야?
class ENGINE_DLL CSoundMgr :  public CBase
{
	DECLARE_SINGLETON(CSoundMgr)
public:
	enum CHANNELID
	{
		BGM,
		SFX,
		SFX1,
		SFX2,
		SFX_DIVE_UI,
		SFX_DIVE_SUBMARINE_COLL1, SFX_DIVE_SUBMARINE_COLL2, SFX_DIVE_SUBMARINE_COLL3,
		BOSS_BGM,
		BOSS_SFX,
		BOSS_SFX1,
		BOSS_SFX2,
		SFX_SHIP_DIVE_BTN,
		SFX_SHIP_DAVE_FOOT,
		SFX_SHIP_UI_APP_CLICK,
		SFX_SHIP_UI_CLICK,
		SFX_SHIP_UI_CHOICE,
		BGM_SHIP_LOBBY,
		BGM_SHIP_BIRD,
		BGM_SHIP_LOOP,
		SFX_FIREWORK1,
		SFX_FIREWORK2,
		MAXCHANNEL
	};
private:
	explicit CSoundMgr();
	virtual ~CSoundMgr();

public:
	FMOD_SYSTEM* Get_System() const { return m_pSystem; }

public:
	HRESULT Ready_SoundMgr();

public:
	void PlaySoundLoop(std::wstring_view svSoundKey, CHANNELID eID, float fVolume);
	void PlaySoundOne(std::wstring_view svSoundKey, CHANNELID eID, float fVolume);
	bool IsChannelPlaying(CHANNELID eID);
	void StopSound(CHANNELID eID);
	void StopAll();
	void SetChannelVolume(CHANNELID eID, float fVolume);

	void UpdateSound();

private:

private:
	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];

	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem;

private:
	virtual void	Free();
};

END