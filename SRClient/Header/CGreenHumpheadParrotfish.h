#pragma once
#include "CFishGameObject.h"

namespace Fish
{
	class CGreenHumpheadParrotfish : public CFishGameObject
	{
	private:
		explicit CGreenHumpheadParrotfish(float fPosX, float fPosY, float fScale);
		explicit CGreenHumpheadParrotfish(const CGreenHumpheadParrotfish& rhs) = delete;
		virtual ~CGreenHumpheadParrotfish();

	public:
		virtual			HRESULT		Ready_GameObject();
		virtual			_int		Update_GameObject(const _float& fTimeDelta);
		virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
		virtual			void		Render_GameObject();

	private:
		HRESULT			Ready_Component();

	public:
		static CGreenHumpheadParrotfish* Create(float fPosX, float fPosY, float fScale);

	private:
		virtual void Free();
	};
}

