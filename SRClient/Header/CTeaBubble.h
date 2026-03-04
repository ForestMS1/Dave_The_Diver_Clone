#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
}

class CTeaBubble : public CGameObject
{
private:
	explicit CTeaBubble();
	explicit CTeaBubble(const CGameObject& rhs);
	virtual ~CTeaBubble();
	void	Update_ImGui() override;

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();
private:
	HRESULT			Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pMenuBubbleTextureCom;
	Engine::CTransform* m_pTransformCom;

public:
	static CTeaBubble* Create();

private:
	virtual void Free();

public:
	float tempY = -1.f;
	bool  gettingTea;
private:
	float deltaTime = 0;


};

