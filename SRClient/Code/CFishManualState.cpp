#include "CFishManualState.h"

#include "CDInputMgr.h"
#include "CHelper.h"

void Fish::CFishManualState::Enter()
{
}

void Fish::CFishManualState::Update(const float& fTimeDelta)
{
	if (CDInputMgr::GetInstance()->Key_Down(DIK_M))
	{
		//_long mouseX = CDInputMgr::GetInstance()->Get_DIMouseMove(DIMS_X);
		//_long mouseY = CDInputMgr::GetInstance()->Get_DIMouseMove(DIMS_Y);

		_vec3 vMouseWorld;
		CHelper::GetMousePointInWorld(&vMouseWorld);

		int x = 0;
		
	}
}

void Fish::CFishManualState::LateUpdate(const float& fTimeDelta)
{
}

void Fish::CFishManualState::Exit()
{
}
