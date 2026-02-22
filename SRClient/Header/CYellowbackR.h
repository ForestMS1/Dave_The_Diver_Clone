#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
}

class CYellowbackR : public CGameObject
{
private:
	explicit CYellowbackR();
	explicit CYellowbackR(const CGameObject& rhs);
	virtual ~CYellowbackR();

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
	static CYellowbackR* Create();

private:
	virtual void Free();

};

