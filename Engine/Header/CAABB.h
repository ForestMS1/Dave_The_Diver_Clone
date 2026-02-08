#pragma once
#include "CCollider.h"
#include "Engine_Define.h"
#include <DirectXCollision.h>

class ENGINE_DLL CAABB : public CCollider
{
private:
	explicit CAABB(COLLIDERID m_eColliderID);
	virtual ~CAABB();

public:
	BoundingBox  Get_BoundingBox() const { return m_BoundingBox; }
	_vec3 Get_RefExtents() const { return m_vRefExtents; }
	void  Set_BoundingBox(BoundingBox boundingBox) { m_BoundingBox = boundingBox; }
	void  Set_BoundingBox(_vec3 const* vCenter, _vec3 const* vExtents)
	{
		XMFLOAT3 center = { vCenter->x, vCenter->y, vCenter->z };
		XMFLOAT3 extent = { vExtents->x, vExtents->y, vExtents->z };
		m_BoundingBox = BoundingBox(center, extent);
	}
	void Set_BoundingBoxCenter(_vec3 const* vCenter)
	{
		XMFLOAT3 center = { vCenter->x, vCenter->y, vCenter->z };
		m_BoundingBox.Center = center;
	}

public:
	HRESULT Ready_AABB(_vec3 const* vCenter, _vec3 const* vExtents);
	void Render() override;

public:
	static CAABB* Create(_vec3 const* vCenter, _vec3 const* vExtents);
	static CAABB* Create(_vec3 const* vCenter, _vec3 const* vExtents, std::wstring_view svTag, void* pVoid = nullptr);
	
protected:
	HRESULT Ready_Buffer() override;

private:
	void Free() override;

protected:
	_vec3 m_vRefExtents;
	BoundingBox m_BoundingBox;
};