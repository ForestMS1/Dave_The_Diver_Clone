#pragma once
#include "CGameObject.h"
#include "CAABB.h"
class CShipDiverBoxInventoryItemArea : public CGameObject
{
private:
	explicit CShipDiverBoxInventoryItemArea(_uint iAppIdx, _float fPosX, _float fPosY);
	explicit CShipDiverBoxInventoryItemArea(const CShipDiverBoxInventoryItemArea& rhs) = delete;
	virtual ~CShipDiverBoxInventoryItemArea();

public:
	void Set_OffsetScaleX(float fScaleX) { m_fScaleX = fScaleX; };
	void Set_OffsetScaleY(float fScaleY) { m_fScaleY = fScaleY; };
	_uint Get_Idx() const { return m_iIdx; }

public:
	void Update_ImGui() override;

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject();

private:
	HRESULT			Ready_Component();

private:
	//Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	CAABB* m_pAABB;

	const _uint m_iIdx;
	 _float m_fPosX;
	 _float m_fPosY;
	 _float m_fScaleX;
	 _float m_fScaleY;

public:
	static CShipDiverBoxInventoryItemArea* Create(_uint iAppIdx, _float fPosX, _float fPosY);

private:
	virtual void Free();
};

