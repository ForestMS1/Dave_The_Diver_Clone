#pragma once
#include "CGameObject.h"
#include "CAABB.h"

enum class ITEMBOXTEX
{
	CHEST_A = 0,
	CHEST_A_OPEN,
	CHEST_BOX,
	CHEST_BOX_OPEN,
	CHEST_WEAPON,
	CHEST_WEAPON_OPEN,
	CHEST_END
};

enum class DROPITEM
{
	O2CAPSULE = 0,
	TRIPLEAXEL,
	PENTAAXEL,
	DROPITEM_END
};
//닫힘 다음 Open으로 순서 지킬 것.

class CDiveItemBox : public CGameObject
{
private:
	explicit CDiveItemBox(ITEMBOXTEX ItemBoxType, _float x, _float y, _float z, DROPITEM drop);
	explicit CDiveItemBox(const CDiveItemBox& rhs);
	virtual ~CDiveItemBox();


public:
	HRESULT		Ready_GameObject() override;
	_int		Update_GameObject(const _float& fTimeDelta) override;
	void		LateUpdate_GameObject(const _float& fTimeDelta) override;
	void		Render_GameObject() override;

public:
	void		Set_Open();
	_bool		Is_Open() const { return m_bIsOpen; }

private:
	HRESULT Ready_Component();
	void	Collision_With_DiveDave();

	// 충돌
	void	OnCollisionEnter(CCollider*);
	void	OnCollisionStay(CCollider*);
	void	OnCollisionExit(CCollider*);

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTransform* m_pTransformCom;
	CAABB* m_pAABB;

private:
	_vec3 m_vInitPos; 
	ITEMBOXTEX m_eCurBoxTex = ITEMBOXTEX::CHEST_A;
	_bool m_bIsOpen = false;
	_bool m_bIsCollWithMe = false;

	DROPITEM m_eDropItem = DROPITEM::O2CAPSULE;

public:
	static CDiveItemBox* Create(ITEMBOXTEX ItemBoxType, _float x = 0, _float y = 0, _float z = 0.1f, DROPITEM drop = DROPITEM::O2CAPSULE);

private:
	virtual void Free() override;
};

