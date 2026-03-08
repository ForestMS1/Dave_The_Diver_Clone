#pragma once
#include "CFishGameObject.h"

namespace Fish
{
	class CCuttleFish : public CFishGameObject
	{
	private:
		explicit CCuttleFish(float fPosX, float fPosY, float fScale);
		explicit CCuttleFish(const CCuttleFish& rhs) = delete;
		virtual ~CCuttleFish();

	public:
		virtual			HRESULT		Ready_GameObject();
		virtual			_int		Update_GameObject(const _float& fTimeDelta);
		virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
		virtual			void		Render_GameObject();

	private:
		HRESULT			Ready_Component();

	public:
		static CCuttleFish* Create(float fPosX, float fPosY, float fScale);

	private:
		virtual void Free();
	};

}