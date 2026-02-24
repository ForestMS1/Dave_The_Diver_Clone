#pragma once
#include "CFishGameObject.h"

namespace Fish
{
	class CYellowbackFusilier : public CFishGameObject
	{
	private:
		explicit CYellowbackFusilier(float fPosX, float fPosY);
		explicit CYellowbackFusilier(const CYellowbackFusilier& rhs) = delete;
		virtual ~CYellowbackFusilier();

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
		static CYellowbackFusilier* Create(float fPosX, float fPosY);

	private:
		virtual void Free();
	};

}