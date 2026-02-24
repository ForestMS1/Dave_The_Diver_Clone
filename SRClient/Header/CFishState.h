#pragma once
#include "CBase.h"
#include "Engine_Define.h"
//#include "CFishGameObject.h"
class CFishGameObject;

namespace Fish
{
	enum FISH_STATE
	{
		FS_STOP,
		FS_SWIM,
		FS_SPRINT,
		FS_DIE,
		FS_QTE,
		FS_END
	};

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
