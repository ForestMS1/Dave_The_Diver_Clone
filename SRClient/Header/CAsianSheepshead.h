#pragma once
#include "CFishGameObject.h"

namespace Fish
{
	class CAsianSheepshead : public CFishGameObject
	{
	private:
		explicit CAsianSheepshead(float fPosX, float fPosY, float fScale);
		explicit CAsianSheepshead(const CAsianSheepshead& rhs) = delete;
		virtual ~CAsianSheepshead();

	public:
		virtual			HRESULT		Ready_GameObject();
		virtual			_int		Update_GameObject(const _float& fTimeDelta);
		virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
		virtual			void		Render_GameObject();

	private:
		HRESULT			Ready_Component();



	public:
		static CAsianSheepshead* Create(float fPosX, float fPosY, float fScale);

	private:
		virtual void Free();
	};
}

