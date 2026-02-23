#pragma once
#include "CGameObject.h"
#include "CAABB.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
}

class CUpgradeButton : public CGameObject
{
private:
	explicit CUpgradeButton();
	explicit CUpgradeButton(const CGameObject& rhs);
	virtual ~CUpgradeButton();


public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();
private:
	HRESULT			Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pUpgradeTextureCom;
	Engine::CTransform* m_pTransformCom;
	CAABB* m_pAABB;

public:
	static CUpgradeButton* Create();
private:
	virtual void Free();

};

