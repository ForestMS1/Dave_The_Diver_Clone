#pragma once
#include "CCollider.h"
#include "Engine_Define.h"
#include <DirectXCollision.h>

class ENGINE_DLL COBB : public CCollider
{
private:
	explicit COBB(COLLIDERID m_eColliderID);
	virtual ~COBB();

public:
	BoundingOrientedBox  Get_BoundingOrientedBox() const { return m_BoundingOrientedBox; }
	_vec3 Get_RefExtents() const { return m_vRefExtents; }
	_vec3 Get_RefOri() const { return m_vRefOri; }
	void  Set_BoundingOrientedBox(BoundingOrientedBox boundingOrientedBox) { m_BoundingOrientedBox = boundingOrientedBox; }
	void  Set_BoundingOrientedBox(_vec3 const* vCenter, _vec3 const* vExtents, _vec3 const* vOrientation)
	{
		XMFLOAT3 center = { vCenter->x, vCenter->y, vCenter->z };
		XMFLOAT3 extent = { vExtents->x, vExtents->y, vExtents->z };
		XMVECTOR orientation = XMVectorSet(vOrientation->x, vOrientation->y, vOrientation->z, 0.0f);
		XMVECTOR quat = XMQuaternionRotationRollPitchYawFromVector(orientation);
		XMFLOAT4 rot;
		XMStoreFloat4(&rot, quat);
		m_BoundingOrientedBox = BoundingOrientedBox(center, extent, rot);
	}
	void Set_OrientedBoxCenter(_vec3 const* vCenter)
	{
		XMFLOAT3 center = { vCenter->x, vCenter->y, vCenter->z };
		m_BoundingOrientedBox.Center = center;
	}
	_vec3 Get_Center() const
	{
		return { m_BoundingOrientedBox.Center.x,m_BoundingOrientedBox.Center.y,m_BoundingOrientedBox.Center.z };
	}
	_vec3 Get_Extents() const
	{
		return { m_BoundingOrientedBox.Extents.x,m_BoundingOrientedBox.Extents.y,m_BoundingOrientedBox.Extents.z };
	}

public:
	HRESULT Ready_OBB(_vec3 const* vCenter, _vec3 const* vExtents, _vec3 const* vOrientation);
	void Render() override;

public:
	static COBB* Create(_vec3 const* vCenter, _vec3 const* vExtents, _vec3 const* vOrientation);
	static COBB* Create(_vec3 const* vCenter, _vec3 const* vExtents, _vec3 const* vOrientation, std::wstring_view svTag, void* pVoid = nullptr);

protected:
	HRESULT Ready_Buffer() override;

private:
	void Free() override;

protected:
	_vec3 m_vRefExtents;
	_vec3 m_vRefOri;
	BoundingOrientedBox m_BoundingOrientedBox;
};