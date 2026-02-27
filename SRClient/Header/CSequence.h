#pragma once
#include "CNode.h"
class CSequence : public CNode
{
public:
	explicit CSequence() {}
	virtual ~CSequence() {}
public:
	void Add_Child(CNode* pNode) { m_vecChild.push_back(pNode); }

	NodeStatus Tick(const _float& fTimeDelta) override
	{
		for (auto& childNode : m_vecChild)
		{
			NodeStatus status = childNode->Tick(fTimeDelta);

			if (status != NodeStatus::SUCCESS)
				return NodeStatus::FAILURE;
		}
		return NodeStatus::SUCCESS;
	}

private:
	std::vector<CNode*> m_vecChild;

private:
	virtual void Free() override {}
};

