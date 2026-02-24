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
	static			CSushiList* Create();

private:
	HRESULT			Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pListextureCom;
	Engine::CTransform* m_pTransformCom;

	_vec3 screen;
	vector<CGameObject*> pictures;
public:
	wstring m_sFishName;
	wstring m_sSushiName;
	wstring m_sFishInfo;
	wstring m_sLevel;
	wstring m_sCost;
	wstring m_sQuality;
	wstring m_sPlate;
	wstring m_sQuanity;

private:
	virtual void Free();

};

