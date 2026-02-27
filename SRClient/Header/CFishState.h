#pragma once
#include "CBase.h"
#include "Engine_Define.h"
//#include "CFishGameObject.h"
class CFishGameObject;

namespace Fish
{
	enum FISH_TYPE
	{
		FT_NORMAL,
		FT_AGRESSIVE,
		FT_END
	};

	enum FISH_STATE
	{
		FS_MANUAL,
		FS_STOP,
		FS_SWIM,
		FS_RUNFROM,
		FS_ATTACKTO,
		FS_ACQUIRETO,
		FS_SPRINT,
		FS_DIE,
		FS_QTE,
		FS_END
	};

	static std::wstring Get_FishStateStr(FISH_STATE eFishState)
	{
		switch (eFishState)
		{
		case Fish::FS_STOP: return L"FS_STOP";
		case Fish::FS_SWIM:return L"FS_SWIM";
		case Fish::FS_SPRINT:return L"FS_SPRINT";
		case Fish::FS_DIE:return L"FS_DIE";
		case Fish::FS_QTE:return L"FS_QTE";
		case Fish::FS_END:return L"FS_END";
		case Fish::FS_ATTACKTO: return L"FS_ATTACKTO";
		case Fish::FS_RUNFROM: return L"FS_RUNFROM";
		default: return L"FS_NOTFOUND_DEFAULT";
		}
	}

	class CFishState
	{
	public:
		CFishState(CFishGameObject* pFish)
			:m_pFish(pFish)
		{
		}
		virtual ~CFishState() = default;

	public:
		virtual void Enter() {};
		virtual void Update(const float& fTimeDelta) {};
		virtual void LateUpdate(const float& fTimeDelta) {};
		virtual void Render() {};
		virtual void Exit() {};

	protected:
		CFishGameObject* const m_pFish;
	};
}
