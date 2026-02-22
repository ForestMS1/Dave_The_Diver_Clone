#pragma once
#include "CGameObject.h"
class CShipDiverBoxInventoryEdge : public CGameObject
{
private:
	explicit CShipDiverBoxInventoryEdge(float fPosX, float fPosY);
	virtual ~CShipDiverBoxInventoryEdge();

public:
	void Set_ScaleX(float fScaleX) { m_fCustomScaleX = fScaleX; }
	void Set_ScaleY(float fScaleY) { m_fCustomScaleY = fScaleY; }
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

	float m_fFirstScaleX;
	float m_fFirstScaleY;
	float m_fCustomScaleX;
	float m_fCustomScaleY;

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;

public:
	static CShipDiverBoxInventoryEdge* Create(float fPosX, float fPosY);

private:
	virtual void Free();
};

