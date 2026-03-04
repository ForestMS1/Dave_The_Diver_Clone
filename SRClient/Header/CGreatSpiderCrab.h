#pragma once
#include "CFishGameObject.h"

namespace Fish
{
	class CGreatSpiderCrab : public CFishGameObject
	{
	private:
		explicit CGreatSpiderCrab(float fPosX, float fPosY, float fScale);
		explicit CGreatSpiderCrab(const CGreatSpiderCrab& rhs) = delete;
		virtual ~CGreatSpiderCrab();

	public:
		virtual			HRESULT		Ready_GameObject();
		virtual			_int		Update_GameObject(const _float& fTimeDelta);
		virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
		virtual			void		Render_GameObject();

	private:
		HRESULT			Ready_Component();

	public:
		static CGreatSpiderCrab* Create(float fPosX, float fPosY, float fScale);

	private:
		virtual void Free();
	};
}