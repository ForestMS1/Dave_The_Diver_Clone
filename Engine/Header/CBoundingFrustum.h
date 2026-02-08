#pragma once
#include "CCollider.h"
#include "Engine_Define.h"
#include <DirectXCollision.h>
class ENGINE_DLL CBoundingFrustum : public CCollider
{
private:
	explicit CBoundingFrustum(COLLIDERID m_eColliderID);
	virtual ~CBoundingFrustum();

public:
	BoundingFrustum  Get_BoundingFrustum() const { return m_BoundingFrustum; }
	_matrix Get_RefProj() const { return m_RefProjMat ; }
	void  Set_BoundingFrustum(BoundingFrustum boundingFrustum) { m_BoundingFrustum = boundingFrustum; }
	void  Set_BoundingFrustum(_matrix const * matProj)
	{
		XMMATRIX matXM = XMLoadFloat4x4(reinterpret_cast<const XMFLOAT4X4*>(matProj));
		m_BoundingFrustum = BoundingFrustum(matXM);
	}

public:
	HRESULT Ready_Frustum(_matrix const* matProj);
	void Render() override;

public:
	static CBoundingFrustum* Create(_matrix const* matProj);
	static CBoundingFrustum* Create(_matrix const* matProj, std::wstring_view svTag, void* pVoid = nullptr);

protected:
	HRESULT Ready_Buffer() override;

private:
	void Free() override;

protected:
	_matrix m_RefProjMat;
	BoundingFrustum m_BoundingFrustum;
};

