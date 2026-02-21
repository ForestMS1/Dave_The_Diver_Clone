#pragma once
#include "CGameObject.h"
#include "CAABB.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
}

class CSushiFrame : public CGameObject
{
private:
	explicit CSushiFrame();
	explicit CSushiFrame(const CGameObject& rhs);
	virtual ~CSushiFrame();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();
private:
	HRESULT			Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pFrameTextureCom;
	Engine::CTexture* m_pSelectedTextureCom;
	//Engine::CTexture* m_pFishTextureCom;
	Engine::CTransform* m_pTransformCom;
	CAABB* m_pAABB;
	_vec3 screen;

public:
	static CSushiFrame* Create(wstring fishName);
	bool m_bSelected;
	bool m_bImageCreated;
	wstring fishName;
	wstring m_sQuanity;
	wstring m_sLevel;
private:
	virtual void Free();
	
};

