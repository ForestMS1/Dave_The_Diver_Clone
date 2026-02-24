#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
}

class CDepthP : public CGameObject
{
private:
	explicit CDepthP();
	explicit CDepthP(const CGameObject& rhs);
	virtual ~CDepthP();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();
private:
	HRESULT			Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pFishTextureCom;
	Engine::CTransform* m_pTransformCom;

public:
	static CDepthP* Create();

private:
	virtual void Free();

};

