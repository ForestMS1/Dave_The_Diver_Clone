#pragma once
#include "CFishGameObject.h"

namespace Fish
{
	class CSmallspottedDart : public CFishGameObject
	{
	private:
		explicit CSmallspottedDart(float fPosX, float fPosY);
		explicit CSmallspottedDart(const CSmallspottedDart& rhs) = delete;
		virtual ~CSmallspottedDart();

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
		static CSmallspottedDart* Create(float fPosX, float fPosY);

	private:
		virtual void Free();
	};

}