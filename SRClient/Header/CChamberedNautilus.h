#pragma once
#include "CFishGameObject.h"
namespace Fish
{
	class CChamberedNautilus : public CFishGameObject
	{
	private:
		explicit CChamberedNautilus(float fPosX, float fPosY, float fScale);
		explicit CChamberedNautilus(const CChamberedNautilus& rhs) = delete;
		virtual ~CChamberedNautilus();

	public:
		virtual			HRESULT		Ready_GameObject();
		virtual			_int		Update_GameObject(const _float& fTimeDelta);
		virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
		virtual			void		Render_GameObject();

	private:
		HRESULT			Ready_Component();

	public:
		static CChamberedNautilus* Create(float fPosX, float fPosY, float fScale);

	private:
		virtual void Free();
	};

}