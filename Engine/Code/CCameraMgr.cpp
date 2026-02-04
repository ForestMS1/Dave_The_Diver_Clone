#include "CCameraMgr.h"

IMPLEMENT_SINGLETON(CCameraMgr)

CCameraMgr::CCameraMgr()
{
}

CCameraMgr::~CCameraMgr()
{
}

HRESULT CCameraMgr::Change_Camera(CAMERAID ID)
{
	if (ID != curCamera) {

		// 기존 카메라 끄기
		if(CAM_END != curCamera)
			dynamic_cast<CCamera*>(listCamera[curCamera])->Set_SwitchCamera(false);

		// 새 카메라 교체
		dynamic_cast<CCamera*>(listCamera[ID])->Reset_Camera();
	

		prevCamera = curCamera;
	}

	return S_OK;
}


void CCameraMgr::Free()
{
}
