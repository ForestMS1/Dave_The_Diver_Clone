#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
}

class CRecipe : public CGameObject
{
private:
	explicit CRecipe();
	explicit CRecipe(const CGameObject& rhs);
	virtual ~CRecipe();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();
private:
	HRESULT			Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pRecipeTextureCom;
	Engine::CTransform* m_pTransformCom;

public:
	static CRecipe* Create();
	bool render;

private:
	virtual void Free();

};

