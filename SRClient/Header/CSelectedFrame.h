#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CRcTex;
	class CTexture;
	class CTransform;
}

class CSelectedFrame : public CGameObject
{
private:
	explicit CSelectedFrame();
	explicit CSelectedFrame(const CGameObject& rhs);
	virtual ~CSelectedFrame();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();
	void			Set_Plate(wstring number) { m_sPlate = number; }
	void			Set_Quantity(wstring quantity) { m_sQuantity = quantity; }
	//void			Set_Cost(wstring cost) { m_sCost = cost; }
private:
	HRESULT			Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pSelectTextureCom;
	Engine::CTransform* m_pTransformCom;

public:

	static CSelectedFrame* Create();
	bool created;
	CGameObject* pSushiGameObject;

private:
	virtual void Free();
	wstring m_sFishName;
	wstring m_sSushiName;
	wstring m_sQuantity;
	wstring m_sCost;
	wstring m_sPlate;
	wstring m_sQuality;
	_vec3 screen1, screen2, screen3, screen4;

};

