
#pragma once
#include "CGameObject.h"
#include "CAABB.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
}

class CWasabiObject : public CGameObject
{
private:
	explicit CWasabiObject();
	explicit CWasabiObject(const CGameObject& rhs);
	virtual ~CWasabiObject();
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
	Engine::CTexture* m_pTextureCom;
	Engine::CTransform* m_pTransformCom;

public:
	static CWasabiObject* Create();
	bool  created;
	float gauge = 0.03f;
	float percent;
private:
	virtual void Free();
	CAABB* m_pAABB;
	
};

