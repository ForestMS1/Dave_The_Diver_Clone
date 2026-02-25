#pragma once
#include "CGameObject.h"
#include "CAABB.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
}

class CUpgradeConfirmButton : public CGameObject
{
private:
	explicit CUpgradeConfirmButton();
	explicit CUpgradeConfirmButton(const CGameObject& rhs);
	virtual ~CUpgradeConfirmButton();


public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();
	void			Set_WhichFish(wstring fish) { whichFish = fish; }
private:
	HRESULT			Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pUpgradeTextureCom;
	Engine::CTransform* m_pTransformCom;
	CAABB* m_pAABB;
	wstring whichFish;

public:
	static CUpgradeConfirmButton* Create();
private:
	virtual void Free();

};

