#pragma once
#include "CGameObject.h"
#include "CAABB.h"
class CShipDiverBoxInventory : public CGameObject
{
public:
	typedef struct tagDiverBoxItem
	{
		std::wstring sItemName;
		std::wstring sItemDesc;
		_uint iCnt;
		std::wstring sAssetName;
	} DIVERBOX_ITEM;
private:
	explicit CShipDiverBoxInventory();
	explicit CShipDiverBoxInventory(const CShipDiverBoxInventory& rhs);
	virtual ~CShipDiverBoxInventory();

public:
	bool Is_Open() const { return m_bOpen ; }

public:
	Engine::CTransform* Get_Transform() const { return m_pTransformCom; }

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
	//CAABB* m_pAABB;


	bool m_bOpen;
	tweeny::tween<float> m_tweenOpen;
	bool m_bOpenTween;
	tweeny::tween<float> m_tweenClose;
	bool m_bCloseTween;

	int m_iSelectItemIdx;

	map<_uint, DIVERBOX_ITEM> m_mapInventory;
public:
	static CShipDiverBoxInventory* Create();

private:
	virtual void Free();
};

