#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
}

class CBluejongP : public CGameObject
{
private:
	explicit CBluejongP();
	explicit CBluejongP(const CGameObject& rhs);
	virtual ~CBluejongP();

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
	static CBluejongP* Create();

private:
	virtual void Free();

};

