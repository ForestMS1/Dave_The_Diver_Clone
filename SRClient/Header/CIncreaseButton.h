#pragma once
#include "CGameObject.h"
#include "CAABB.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
}

class CIncreaseButton : public CGameObject
{
private:
	explicit CIncreaseButton();
	explicit CIncreaseButton(const CGameObject& rhs);
	virtual ~CIncreaseButton();


public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();
private:
	HRESULT			Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pIncreaseTextureCom;
	Engine::CTransform* m_pTransformCom;
	CAABB* m_pAABB;

public:
	static CIncreaseButton* Create();
private:
	virtual void Free();

};

