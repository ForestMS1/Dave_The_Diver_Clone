#pragma once
#include "CFishGameObject.h"

namespace Fish
{
	class CSmallspottedDart : public CFishGameObject
	{
	private:
		explicit CSmallspottedDart(float fPosX, float fPosY, float fScale);
		explicit CSmallspottedDart(const CSmallspottedDart& rhs) = delete;
		virtual ~CSmallspottedDart();

	public:
		virtual			HRESULT		Ready_GameObject();
		virtual			_int		Update_GameObject(const _float& fTimeDelta);
		virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
		virtual			void		Render_GameObject();

	private:
		HRESULT			Ready_Component();

	public:
		static CSmallspottedDart* Create(float fPosX, float fPosY, float fScale);

	private:
		virtual void Free();
	};

}