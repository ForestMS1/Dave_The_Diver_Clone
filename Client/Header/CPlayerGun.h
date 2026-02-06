#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
}

class CPlayerGun : public CGameObject
{
private:
	explicit CPlayerGun();
	explicit CPlayerGun(const CPlayerGun& rhs);
	virtual ~CPlayerGun();

public:
	virtual			HRESULT		Ready_GameObject()												override;
	virtual			_int		Update_GameObject(const _float& fTimeDelta)						override;
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta)					override;
	virtual			void		Render_GameObject()												override;
private:
	HRESULT			Add_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;


public:
	static CPlayerGun* Create();

private:
	virtual void Free();
};

