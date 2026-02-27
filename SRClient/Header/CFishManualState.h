#pragma once
#include "CFishState.h"

namespace Fish
{
	class CFishManualState : public CFishState
	{
	public:
		CFishManualState(CFishGameObject* pFish)
			: CFishState(pFish)
		{
		}
		~CFishManualState() = default;

	public:
		void Enter() override;
		void Update(const float& fTimeDelta) override;
		void LateUpdate(const float& fTimeDelta) override;
		void Exit() override;
	};
}