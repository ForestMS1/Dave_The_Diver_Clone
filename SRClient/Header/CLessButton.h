#pragma once
#include "CGameObject.h"
#include "CAABB.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
}

class CLessButton : public CGameObject
{
private:
	explicit CLessButton();
	explicit CLessButton(const CGameObject& rhs);
	virtual ~CLessButton();


public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();
private:
	HRESULT			Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pLessTextureCom;
	Engine::CTransform* m_pTransformCom;
	CAABB* m_pAABB;

public:
	static CLessButton* Create();
private:
	virtual void Free();

};

