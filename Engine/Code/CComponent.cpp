#include "CComponent.h"

CComponent::CComponent()
    : m_bClone(false)
{
}

CComponent::CComponent(const CComponent& rhs)
    : m_bClone(true)
{
}

CComponent::~CComponent()
{
}

void CComponent::Free()
{
}
