#pragma once

#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
}

class CBackGroundSea : public CGameObject
{
private:
	explicit CBackGroundSea();
	explicit CBackGroundSea(const CGameObject& rhs);
	virtual ~CBackGroundSea();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

private:
	HRESULT			Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

public:
	static CBackGroundSea* Create();

private:
	virtual void Free();

};

