#include "CFishGameObject.h"
#include "CFishSwimState.h"
void Fish::CFishSwimState::Enter()
{
}

void Fish::CFishSwimState::Update(const float& fTimeDelta)
{
    _vec3 vRight;
    m_pFish->m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
    D3DXVec3Normalize(&vRight, &vRight);
    m_pFish->m_pTransformCom->Move_Pos(&vRight, m_pFish->m_fSpeed, fTimeDelta);
    
    //m_pTransformCom->Get_Info(INFO_RIGHT, &vRight);
    //D3DXVec3Normalize(&vRight, &vRight);

    //m_pTransformCom->Move_Pos(&vRight, m_fSpeed, fTimeDelta);

}

void Fish::CFishSwimState::LateUpdate(const float& fTimeDelta)
{
}

void Fish::CFishSwimState::Exit()
{
}
