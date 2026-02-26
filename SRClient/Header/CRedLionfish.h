#pragma once
#include "CFishGameObject.h"
namespace Fish
{
	class CRedLionfish : public CFishGameObject
	{
	private:
		explicit CRedLionfish(float fPosX, float fPosY, float fScale);
		explicit CRedLionfish(const CRedLionfish& rhs) = delete;
		virtual ~CRedLionfish();

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
		const float m_fScale;

	public:
		static CRedLionfish* Create(float fPosX, float fPosY, float fScale);

	private:
		virtual void Free();
	};
}
