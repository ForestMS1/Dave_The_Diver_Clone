#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
}

class CSushiList : public CGameObject
{
private:
	explicit CSushiList();
	explicit CSushiList(const CGameObject& rhs);
	virtual ~CSushiList();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();
private:
	HRESULT			Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pListextureCom;
	Engine::CTransform* m_pTransformCom;

public:
	static CSushiList* Create();
	bool render;

private:
	virtual void Free();

};

