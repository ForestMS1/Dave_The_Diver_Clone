#pragma once
#include "CFishGameObject.h"

namespace Fish
{
	class CAtlanticAnglerfish : public CFishGameObject
	{
	private:
		explicit CAtlanticAnglerfish(float fPosX, float fPosY, float fScale);
		explicit CAtlanticAnglerfish(const CAtlanticAnglerfish& rhs) = delete;
		virtual ~CAtlanticAnglerfish();

	public:
		virtual			HRESULT		Ready_GameObject();
		virtual			_int		Update_GameObject(const _float& fTimeDelta);
		virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
		virtual			void		Render_GameObject();

	private:
		HRESULT			Ready_Component();

	public:
		static CAtlanticAnglerfish* Create(float fPosX, float fPosY, float fScale);

	private:
		virtual void Free();
	};

}