#pragma once
#include "CGameObject.h"
#include "CAABB.h"
class CFishAABBCollider : public CGameObject
{
public:
	explicit CFishAABBCollider(CAABB* pAABB, std::wstring_view svCollGroup);
	explicit CFishAABBCollider(const CFishAABBCollider& rhs) = delete;
	virtual ~CFishAABBCollider();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject() {};

public:
	void Set_ApplyParentAngle(bool b) { m_bApplyParentAngle = b; };
	void Set_PosOffset(_vec3 const* pOffset) { m_vPosOffset = *pOffset; }

public:
	CAABB* Get_AABB() { return m_pAABB; }
private:
	HRESULT			Ready_Component();

private:
	Engine::CTransform* m_pTransformCom;
	CAABB* m_pAABB;

	//const _vec3 m_vPos;
	//const _vec3 m_vScale;
	_vec3 m_vPosOffset;

	std::wstring m_sCollGroup;

	bool m_bApplyParentAngle;

public:
	static CFishAABBCollider* Create(CAABB* pAABB, std::wstring_view svCollGroup);

public:
	void Free() override;
};

