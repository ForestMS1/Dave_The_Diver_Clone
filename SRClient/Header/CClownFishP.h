#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
}

class CClownFishP : public CGameObject
{
private:
	explicit CClownFishP();
	explicit CClownFishP(const CGameObject& rhs);
	virtual ~CClownFishP();

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
	static CClownFishP* Create();

private:
	virtual void Free();

};

