#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
}

class CBanchoR : public CGameObject
{
private:
	explicit CBanchoR();
	explicit CBanchoR(const CGameObject& rhs);
	virtual ~CBanchoR();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();
private:
	HRESULT			Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pSushiTextureCom;
	Engine::CTransform* m_pTransformCom;

public:
	static CBanchoR* Create();

private:
	virtual void Free();

};

