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
	void  Set_BoundingBox(BoundingBox boundingBox) { m_BoundingBox = boundingBox; }
	void  Set_BoundingBox(_vec3* vCenter, _vec3* vExtents)
	{
		XMFLOAT3 center = { vCenter->x, vCenter->y, vCenter->z };
		XMFLOAT3 extent = { vExtents->x, vExtents->y, vExtents->z };
		m_BoundingBox = BoundingBox(center, extent);
	}

public:
	HRESULT Ready_AABB(_vec3* vCenter, _vec3* vExtents);
	void Render() override;

	static CAABB* Create(_vec3* vCenter, _vec3* vExtents);
	static CAABB* Create(_vec3* vCenter, _vec3* vExtents, std::wstring_view svTag, void* pVoid = nullptr);
	
protected:
	HRESULT Ready_Buffer() override;

private:
	void Free() override;

protected:
	BoundingBox m_BoundingBox;
};