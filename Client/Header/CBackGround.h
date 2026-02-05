#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
}

class CBackGround : public CGameObject
{
private:
	explicit CBackGround();
	explicit CBackGround(const CGameObject& rhs);
	virtual ~CBackGround();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

private:
	HRESULT			Add_Component();

private:
	Engine::CRcTex*			m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;

public:
	static CBackGround* Create();

private:
	virtual void Free();

};

