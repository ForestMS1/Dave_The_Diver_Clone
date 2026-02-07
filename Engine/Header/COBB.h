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
	void  Set_BoundingOrientedBox(BoundingOrientedBox boundingOrientedBox) { m_BoundingOrientedBox = boundingOrientedBox; }
	void  Set_BoundingOrientedBox(_vec3* vCenter, _vec3* vExtents, _vec3* vOrientation)
	{
		XMFLOAT3 center = { vCenter->x, vCenter->y, vCenter->z };
		XMFLOAT3 extent = { vExtents->x, vExtents->y, vExtents->z };
		XMVECTOR orientation = XMVectorSet(vOrientation->x, vOrientation->y, vOrientation->z, 0.0f);
		XMVECTOR quat = XMQuaternionRotationRollPitchYawFromVector(orientation);
		XMFLOAT4 rot;
		XMStoreFloat4(&rot, quat);
		m_BoundingOrientedBox = BoundingOrientedBox(center, extent, rot);
	}

public:
	HRESULT Ready_OBB(_vec3* vCenter, _vec3* vExtents, _vec3* vOrientation);
	void Render() override;

	static COBB* Create(_vec3* vCenter, _vec3* vExtents, _vec3* vOrientation);
	static COBB* Create(_vec3* vCenter, _vec3* vExtents, _vec3* vOrientation, std::wstring_view svTag, void* pVoid = nullptr);

protected:
	HRESULT Ready_Buffer() override;

private:
	void Free() override;

protected:
	BoundingOrientedBox m_BoundingOrientedBox;
};