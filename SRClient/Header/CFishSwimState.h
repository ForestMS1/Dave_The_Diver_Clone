#pragma once

#include "CFishState.h"

namespace Fish
{
	class CFishSwimState : public CFishState
	{
	public:
		CFishSwimState(CFishGameObject* pFish)
			: CFishState(pFish)
		{
		}
		~CFishSwimState() = default;

	public:
		void Enter() override;
		void Update(const float& fTimeDelta) override;
		void LateUpdate(const float& fTimeDelta) override;
		void Exit() override;
	};
}