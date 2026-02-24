#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
}

class COverlay : public CGameObject
{
private:
	explicit COverlay();
	explicit COverlay(const CGameObject& rhs);
	virtual ~COverlay();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();
private:
	HRESULT			Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pOverlayTextureCom;
	Engine::CTransform* m_pTransformCom;

public:
	static COverlay* Create();

private:
	virtual void Free();

};

