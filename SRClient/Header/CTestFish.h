#pragma once
#include "CGameObject.h"
#include "CAABB.h"
class CTestFish :
    public CGameObject
{
private:
	explicit CTestFish();
	explicit CTestFish(const CTestFish& rhs);
	virtual ~CTestFish();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

private:
	HRESULT			Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;
	CAABB* m_pAABB;

public:
	static CTestFish* Create();

private:
	virtual void Free();
};

