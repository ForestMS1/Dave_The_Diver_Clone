#pragma once
#include "CNode.h"
class CSelector : public CNode
{
public:
	explicit CSelector() {}
	virtual ~CSelector() {}

public:
	void Add_Child(CNode* pNode){ m_vecChild.push_back(pNode); }

public:
	NodeStatus Tick(const _float& fTimeDelta)
	{
		for (auto& childNode : m_vecChild)
		{
			NodeStatus status = childNode->Tick(fTimeDelta);

			if (status != NodeStatus::FAILURE)
				return status;
		}
		return NodeStatus::FAILURE;
	}


private:
	std::vector<CNode*> m_vecChild;
private:
	virtual void Free() override {}
};

