#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
}

class CFishConfirmFrame : public CGameObject
{
private:
	explicit CFishConfirmFrame();
	explicit CFishConfirmFrame(const CGameObject& rhs);
	virtual ~CFishConfirmFrame();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();
public:
	int quantitySelected;
	wstring m_sFishName;
	wstring m_sSushiName;
	wstring m_sFishQuantity;
	wstring m_sQuantitySelected;
private:
	HRESULT			Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pFishConfirmframeTextureCom;
	Engine::CTransform* m_pTransformCom;

public:
	static CFishConfirmFrame* Create();
	vector<CGameObject*> &Get_CurObjects() { return frameGameObjects; }
private:
	virtual void Free();
	vector<CGameObject*> fishPictures;
	vector<CGameObject*> sushiPictures;
	vector<CGameObject*> frameGameObjects;
	_vec3 screen;
	_vec3 screen1;
	_vec3 screen2;
	_vec3 screen3;
	_vec3 screen4;

};

