#pragma once
#include "CGameObject.h"
class CStockMarket : public CGameObject
{
private:
	explicit CStockMarket();
	virtual ~CStockMarket();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();


private:
	HRESULT			Ready_Component();

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

public:
	static CStockMarket* Create();

private:
	virtual void Free();
	bool moneyChanged = false;
	int pictureIndex;
	wstring pictureOriginalName;
	wstring pictureName;
};

