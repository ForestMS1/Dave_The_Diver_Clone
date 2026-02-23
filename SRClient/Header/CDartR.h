#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
}

class CDartR : public CGameObject
{
private:
	explicit CDartR();
	explicit CDartR(const CGameObject& rhs);
	virtual ~CDartR();

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
	static CDartR* Create();

private:
	virtual void Free();

};

