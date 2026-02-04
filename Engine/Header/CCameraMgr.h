#pragma once
#include "CBase.h"
#include "CCamera.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CCameraMgr : public CBase
{
	DECLARE_SINGLETON(CCameraMgr)

private:
	explicit CCameraMgr();
	virtual ~CCameraMgr();

public:
	CGameObject*		Get_Camera(CAMERAID ID)					 { return listCamera[ID]; }
	void				Set_Camera(CAMERAID ID, CGameObject* pCamera) { listCamera[ID] = pCamera; }

	HRESULT			Change_Camera(CAMERAID ID);


protected:
	CGameObject * listCamera[CAM_END];
	CAMERAID curCamera{ CAM_END };
	CAMERAID prevCamera{ CAM_END};
private:
	virtual void	Free();
};

END