#pragma once
#include "CFishGameObject.h"

namespace Fish
{
	class CGiantTrevally : public CFishGameObject
	{
	private:
		explicit CGiantTrevally(float fPosX, float fPosY, float fScale);
		explicit CGiantTrevally(const CGiantTrevally& rhs) = delete;
		virtual ~CGiantTrevally();

	public:
		virtual			HRESULT		Ready_GameObject();
		virtual			_int		Update_GameObject(const _float& fTimeDelta);
		virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
		virtual			void		Render_GameObject();

	private:
		HRESULT			Ready_Component();

	public:
		static CGiantTrevally* Create(float fPosX, float fPosY, float fScale);

	private:
		virtual void Free();
	};
}
