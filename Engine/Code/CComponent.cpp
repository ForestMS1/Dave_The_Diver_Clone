#include "CComponent.h"

CComponent::CComponent()
    : m_bClone(false)
    , m_pGameObject(nullptr)
{
}

CComponent::CComponent(const CComponent& rhs)
    : m_bClone(true)
    , m_pGameObject(nullptr) // 클론시 오브젝트는 널처리 따로 세터 하는게 좋을듯
{
}

CComponent::~CComponent()
{
}

void CComponent::Free()
{
}
