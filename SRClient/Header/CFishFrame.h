#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
}

class CFishFrame : public CGameObject
{
private:
	explicit CFishFrame();
	explicit CFishFrame(const CGameObject& rhs);
	virtual ~CFishFrame();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();
private:
	HRESULT			Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pFishframeTextureCom;
	Engine::CTransform* m_pTransformCom;

public:
	static CFishFrame* Create();

private:
	virtual void Free();
	vector<CGameObject*> pictures;
	wstring m_sFishName;
	wstring m_sFishQuantity;
	_vec3 screen;
	_vec3 screen1;
	
};

