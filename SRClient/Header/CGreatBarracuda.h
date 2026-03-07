#pragma once
#include "CFishGameObject.h"

namespace Fish
{
	class CGreatBarracuda : public CFishGameObject
	{
	private:
		explicit CGreatBarracuda(float fPosX, float fPosY, float fScale);
		explicit CGreatBarracuda(const CGreatBarracuda& rhs) = delete;
		virtual ~CGreatBarracuda();

	public:
		virtual			HRESULT		Ready_GameObject();
		virtual			_int		Update_GameObject(const _float& fTimeDelta);
		virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
		virtual			void		Render_GameObject();

	private:
		HRESULT			Ready_Component();

	public:
		static CGreatBarracuda* Create(float fPosX, float fPosY, float fScale);

	private:
		virtual void Free();
	};

}