#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
}

class CConfirm : public CGameObject
{
private:
	explicit CConfirm();
	explicit CConfirm(const CGameObject& rhs);
	virtual ~CConfirm();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();
private:
	HRESULT			Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pConfirmTextureCom;
	Engine::CTransform* m_pTransformCom;

public:
	static CConfirm* Create();
	bool render;

private:
	virtual void Free();

};

