#pragma once
#include "CFishGameObject.h"

// swim, die
namespace Fish
{
	class CBlueTang : public CFishGameObject
	{
	private:
		explicit CBlueTang(float fPosX, float fPosY);
		explicit CBlueTang(const CBlueTang& rhs) = delete;
		virtual ~CBlueTang();

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
		static CBlueTang* Create(float fPosX, float fPosY);

	private:
		virtual void Free();
	};

}