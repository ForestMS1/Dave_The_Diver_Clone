#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
}

class CUpgradeFrame : public CGameObject
{
private:
	explicit CUpgradeFrame();
	explicit CUpgradeFrame(const CGameObject& rhs);
	virtual ~CUpgradeFrame();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();
public:
	wstring m_sFishName;
	wstring m_sFishQuantity;
	wstring m_sQuantityRequired;
	wstring m_sCost1;
	wstring m_sCost2;
	wstring m_sQuality1;
	wstring m_sQuality2;
private:
	HRESULT			Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pFishConfirmframeTextureCom;
	Engine::CTransform* m_pTransformCom;

public:
	static CUpgradeFrame* Create();
	vector<CGameObject*>& Get_CurObjects() { return frameGameObjects; }
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
	_vec3 screen5;
	_vec3 screen6;
public:
	bool quantityRequired;


};

