#pragma once
#include "CFishGameObject.h"

namespace Fish
{
	class CAtlanticBonito : public CFishGameObject
	{
	private:
		explicit CAtlanticBonito(float fPosX, float fPosY, float fScale);
		explicit CAtlanticBonito(const CAtlanticBonito& rhs) = delete;
		virtual ~CAtlanticBonito();

	public:
		virtual			HRESULT		Ready_GameObject();
		virtual			_int		Update_GameObject(const _float& fTimeDelta);
		virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
		virtual			void		Render_GameObject();

	private:
		HRESULT			Ready_Component();

	public:
		static CAtlanticBonito* Create(float fPosX, float fPosY, float fScale);

	private:
		virtual void Free();
	};

}