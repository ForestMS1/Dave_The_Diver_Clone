#pragma once
#include "CNode.h"

class CActionNode : public CNode
{
public:
	explicit CActionNode(std::function<NodeStatus(const _float& fTimeDelta)> func) : action(func) {}
	virtual ~CActionNode() {}
public:
	virtual void OnStart() = 0;

	NodeStatus Tick(const _float& fTimeDelta) override
	{
		OnStart();

		NodeStatus result = action(fTimeDelta);

		if (result != NodeStatus::RUNNING)
		{
			OnEnd();
			return result;
		}
	}

	virtual void OnEnd() = 0;

protected:
	std::function<NodeStatus(const _float& fTimeDelta)> action;
	_bool OnStartComplete = false;
private:
	virtual void Free() override {};
};