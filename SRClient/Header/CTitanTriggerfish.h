#pragma once
#include "CFishGameObject.h"

namespace Fish
{
	class CTitanTriggerfish : public CFishGameObject
	{
	private:
		explicit CTitanTriggerfish(float fPosX, float fPosY, float fScale);
		explicit CTitanTriggerfish(const CTitanTriggerfish& rhs) = delete;
		virtual ~CTitanTriggerfish();

	public:
		virtual			HRESULT		Ready_GameObject();
		virtual			_int		Update_GameObject(const _float& fTimeDelta);
		virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
		virtual			void		Render_GameObject();

	private:
		HRESULT			Ready_Component();

	public:
		static CTitanTriggerfish* Create(float fPosX, float fPosY, float fScale);

	private:
		virtual void Free();
	};
}