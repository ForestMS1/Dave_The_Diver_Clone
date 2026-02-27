#pragma once
#include "CBase.h"
#include "Engine_Define.h"

enum class NodeStatus
{
    SUCCESS = 0,
    RUNNING,
    FAILURE,
    STATUS_END,
};
class CNode :
    public CBase
{
public:
    explicit CNode() {}
    virtual ~CNode() {}

public:
    virtual NodeStatus Tick(const _float& fTimeDelta) = 0;

protected:
    void Free() override {}
};