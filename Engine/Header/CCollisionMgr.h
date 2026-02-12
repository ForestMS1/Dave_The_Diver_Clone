#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "CAABB.h"

BEGIN(Engine)

class ENGINE_DLL CCollisionMgr :  public CBase
{
	DECLARE_SINGLETON(CCollisionMgr)
public:
	enum DIRECTION
	{
		DIR_NONE,
		DIR_LEFT,
		DIR_UP,
		DIR_RIGHT,
		DIR_DOWN,
		DIR_END
	};

	typedef struct tagCollRectExInfo
	{
		DIRECTION eDir;
		float fDistance;
	} COLL_RECT_EX_INFO;

private:
	explicit CCollisionMgr();
	virtual ~CCollisionMgr();

public:
	HRESULT Ready();
	
	bool Check_Rect(CAABB const* pSrcAABB, CAABB const* pDstAABB, float* pX, float* pY);
	// src가 dst에대해서 어떻게 충돌하고 있는가 pInfo 필수 z값 비교안함
	bool Collision_RectEx(CAABB const* pSrcAABB, CAABB const* pDstAABB, COLL_RECT_EX_INFO* pInfo);

private:
	virtual void		Free();
};


END
