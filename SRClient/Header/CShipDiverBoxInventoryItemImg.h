#pragma once
#include "CGameObject.h"
class CShipDiverBoxInventoryItemImg : public CGameObject
{
private:
	explicit CShipDiverBoxInventoryItemImg(float fPosX, float fPosY);
	virtual ~CShipDiverBoxInventoryItemImg();

public:
	void Set_AssetName(std::wstring_view svAssetName) { m_sAssetName = svAssetName ; }
	void Set_ItemCnt(int iCnt) { m_iCnt = iCnt; }

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

private:
	HRESULT			Ready_Component();

private:
	const float m_fPosX;
	const float m_fPosY;

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	std::wstring m_sAssetName;
	int m_iCnt;

public:
	static CShipDiverBoxInventoryItemImg* Create(float fPosX, float fPosY);

private:
	virtual void Free();
};

