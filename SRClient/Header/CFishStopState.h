#pragma once
#include "CFishState.h"

namespace Fish
{

	class CFishStopState : public CFishState
	{
	public:
		CFishStopState(CFishGameObject* pFish)
			: CFishState(pFish)
		{
		}
		~CFishStopState() = default;

	public:
		void Enter() override;
		void Update(const float& fTimeDelta) override;
		void LateUpdate(const float& fTimeDelta) override;
		void Exit() override;
	};
}