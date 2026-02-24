#pragma once
#include "CFishGameObject.h"

namespace Fish
{
	class CClownfish : public CFishGameObject
	{
	private:
		explicit CClownfish(float fPosX, float fPosY);
		explicit CClownfish(const CClownfish& rhs) = delete;
		virtual ~CClownfish();

	public:
		virtual			HRESULT		Ready_GameObject();
		virtual			_int		Update_GameObject(const _float& fTimeDelta);
		virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
		virtual			void		Render_GameObject();

	private:
		HRESULT			Ready_Component();

	private:
		const float m_fPosX;
		const float m_fPosY;

	public:
		static CClownfish* Create(float fPosX, float fPosY);

	private:
		virtual void Free();
	};

}