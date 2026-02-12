#include "CCollisionMgr.h"
IMPLEMENT_SINGLETON(CCollisionMgr)


CCollisionMgr::CCollisionMgr()
{
}

CCollisionMgr::~CCollisionMgr()
{
    Free();
}

void CCollisionMgr::Free()
{

}
HRESULT CCollisionMgr::Ready()
{

    return S_OK;
}

bool CCollisionMgr::Check_Rect(CAABB const * pSrcAABB, CAABB const * pDstAABB, float* pX, float* pY)
{
	
	float		fWidth = fabsf(pSrcAABB->Get_Center().x - pDstAABB->Get_Center().x);
	float		fHeight = fabsf(pSrcAABB->Get_Center().y - pDstAABB->Get_Center().y);
	;
	float		fRadiusX = pSrcAABB->Get_Extents().x + pDstAABB->Get_Extents().x;
	float		fRadiusY = pSrcAABB->Get_Extents().y + pDstAABB->Get_Extents().y;

	if (fRadiusX >= fWidth && fRadiusY >= fHeight)
	{
		*pX = fRadiusX - fWidth;
		*pY = fRadiusY - fHeight;

		return true;
	}

	return false;
}

bool CCollisionMgr::Collision_RectEx(CAABB const* pSrcAABB, CAABB const* pDstAABB, COLL_RECT_EX_INFO* pInfo)
{
	float	fWidth(0.f), fHeight(0.f);
	if (Check_Rect(pSrcAABB, pDstAABB, &fWidth, &fHeight))
	{
		// 상 하 충돌
		if (fWidth > fHeight)
		{
			// 상 충돌
			if (pSrcAABB->Get_Center().y < pDstAABB->Get_Center().y)
			{
				pInfo->eDir = DIR_UP;
				pInfo->fDistance = fHeight;
				return true;
			}
			// 하 충돌
			else
			{
				pInfo->eDir = DIR_DOWN;
				pInfo->fDistance = fHeight;
				return true;
			}
		}

		// 좌 우 충돌
		else
		{
			// 좌 충돌
			if (pSrcAABB->Get_Center().x < pDstAABB->Get_Center().x)
			{
				pInfo->eDir = DIR_LEFT;
				pInfo->fDistance = fWidth;
				return true;
			}
			// 우 충돌
			else
			{
				pInfo->eDir = DIR_RIGHT;
				pInfo->fDistance = fWidth;
				return true;
			}
		}
	}
	pInfo->eDir = DIR_END;
	return false;
}
