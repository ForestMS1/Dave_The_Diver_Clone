#pragma once
#include "CFishGameObject.h"

namespace Fish
{
	class CYellowTang : public CFishGameObject
	{
	private:
		explicit CYellowTang(float fPosX, float fPosY);
		explicit CYellowTang(const CYellowTang& rhs) = delete;
		virtual ~CYellowTang();

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
		static CYellowTang* Create(float fPosX, float fPosY);

	private:
		virtual void Free();
	};

}