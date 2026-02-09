#include "CBoundingSphere.h"
#include "CGraphicDev.h"

#include "CColliderMgr.h"

CBoundingSphere::CBoundingSphere(COLLIDERID eColliderID)
	: CCollider(eColliderID)
	, m_pSphereMesh(nullptr)
	, m_fRefRadius(0.f)
{

}

CBoundingSphere::~CBoundingSphere()
{
}

HRESULT CBoundingSphere::Ready_BoundingSphere(_vec3 const* vCenter, float& fRadius)
{
	Set_BoundingSphere(vCenter, fRadius);
	m_fRefRadius = fRadius;

	m_dwOriginalColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
	m_dwIntersectColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	return S_OK;
}

CBoundingSphere* CBoundingSphere::Create(_vec3 const* vCenter, float& fRadius)
{
	return CBoundingSphere::Create(vCenter, fRadius, {});
}

CBoundingSphere* CBoundingSphere::Create(_vec3 const* vCenter, float& fRadius, std::wstring_view svTag, void* pVoid)
{
	CBoundingSphere* pSphere = new CBoundingSphere(COLL_SPHERE);
	pSphere->Set_Tag(svTag);
	pSphere->Set_VoidPtr(pVoid);

	if (FAILED(pSphere->Ready_BoundingSphere(vCenter, fRadius)))
	{
		Safe_Release(pSphere);
		MSG_BOX("pSphere Create Failed");
		return nullptr;
	}

	return pSphere;
}

HRESULT CBoundingSphere::Ready_Buffer()
{
	m_dwCurrentColor = m_dwOriginalColor;

	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
	if (FAILED(D3DXCreateSphere(pGraphicDev, 1.f, 10, 10, &m_pSphereMesh, nullptr)))
	{
		return E_FAIL;
	}

	m_bRenderInitialized = true;
	return S_OK;
}

void CBoundingSphere::Render()
{
	if (!m_bRenderInitialized)
	{
		if (FAILED(Ready_Buffer()))
		{
			return;
		}
	}

	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
	pGraphicDev->SetTexture(0, nullptr);

	pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); 
	pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);


	float fScale = m_BoundingSphere.Radius ;

	D3DXMATRIX matTrans, matScale, matWorld;
	D3DXMatrixTranslation(&matTrans, m_BoundingSphere.Center.x, m_BoundingSphere.Center.y, m_BoundingSphere.Center.z);
	D3DXMatrixScaling(&matScale, fScale, fScale, fScale);
	matWorld = matScale * matTrans;
	pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

	pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, (DWORD)m_dwCurrentColor);

	pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
	pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

	m_pSphereMesh->DrawSubset(0);

	pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);


	pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
}

void CBoundingSphere::Free()
{
	CCollider::Free();
	Safe_Release(m_pSphereMesh);
}
