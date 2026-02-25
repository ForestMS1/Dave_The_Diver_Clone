#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
}

class CMenuBubble : public CGameObject
{
private:
	explicit CMenuBubble();
	explicit CMenuBubble(const CGameObject& rhs);
	virtual ~CMenuBubble();
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
	static CMenuBubble* Create();

private:
	virtual void Free();

public:
	wstring m_sFishName;
	wstring m_sTexName;
private:
	float tempY = -1.f;
	float deltaTime = 0;
	
};

