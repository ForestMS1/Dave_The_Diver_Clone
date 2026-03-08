#pragma once
#include "CFishGameObject.h"

namespace Fish
{
	class CCowPatternSnapper : public CFishGameObject
	{
	private:
		explicit CCowPatternSnapper(float fPosX, float fPosY, float fScale);
		explicit CCowPatternSnapper(const CCowPatternSnapper& rhs) = delete;
		virtual ~CCowPatternSnapper();

	public:
		virtual			HRESULT		Ready_GameObject();
		virtual			_int		Update_GameObject(const _float& fTimeDelta);
		virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
		virtual			void		Render_GameObject();

	private:
		HRESULT			Ready_Component();

	public:
		static CCowPatternSnapper* Create(float fPosX, float fPosY, float fScale);

	private:
		virtual void Free();

	};

}