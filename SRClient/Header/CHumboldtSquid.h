#pragma once
#include "CFishGameObject.h"

namespace Fish
{
	class CHumboldtSquid : public CFishGameObject
	{
	private:
		explicit CHumboldtSquid(float fPosX, float fPosY, float fScale);
		explicit CHumboldtSquid(const CHumboldtSquid& rhs) = delete;
		virtual ~CHumboldtSquid();

	public:
		virtual			HRESULT		Ready_GameObject();
		virtual			_int		Update_GameObject(const _float& fTimeDelta);
		virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
		virtual			void		Render_GameObject();

	private:
		HRESULT			Ready_Component();

	public:
		static CHumboldtSquid* Create(float fPosX, float fPosY, float fScale);

	private:
		virtual void Free();
	};
}