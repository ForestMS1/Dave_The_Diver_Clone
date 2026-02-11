#pragma once
#include "CCollider.h"
#include "Engine_Define.h"
#include <DirectXCollision.h>
class ENGINE_DLL CBoundingSphere : public CCollider
{
private:
	explicit CBoundingSphere(COLLIDERID m_eColliderID);
	virtual ~CBoundingSphere();

public:
	BoundingSphere  Get_BoundingSphere() const { return m_BoundingSphere; }
	float Get_RefRadius() const { return m_fRefRadius; }
	void  Set_BoundingSphere(BoundingSphere boundingSphere) { m_BoundingSphere = boundingSphere; }
	void  Set_BoundingSphere(_vec3 const* vCenter, float& fRadius)
	{
		XMFLOAT3 center = { vCenter->x, vCenter->y, vCenter->z };
		m_BoundingSphere = BoundingSphere(center, fRadius);
	}
	void Set_BoundingSphereCenter(_vec3 const* vCenter)
	{
		XMFLOAT3 center = { vCenter->x, vCenter->y, vCenter->z };
		m_BoundingSphere.Center = center;
	}
	_vec3 Get_Center()
	{
		return { m_BoundingSphere.Center.x, m_BoundingSphere.Center.y ,m_BoundingSphere.Center.z };
	}

public:
	HRESULT Ready_BoundingSphere(_vec3 const* vCenter, float& fRadius);
	void Render() override;

public:
	static CBoundingSphere* Create(_vec3 const* vCenter, float& fRadius);
	static CBoundingSphere* Create(_vec3 const* vCenter, float& fRadius, std::wstring_view svTag, void* pVoid = nullptr);

protected:
	HRESULT Ready_Buffer() override;
	LPD3DXMESH m_pSphereMesh;

private:
	void Free() override;

protected:
	float m_fRefRadius;
	BoundingSphere m_BoundingSphere;
};

