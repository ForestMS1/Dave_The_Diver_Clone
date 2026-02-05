#pragma once

#include "Engine_Define.h"
#include <queue>

BEGIN(Engine)

class ENGINE_DLL CHelper
{
public:
	// 주의!! 동적할당된거 말고 스택에 할당된 주소로 해야함 sizeof때문에
	static void ConvertTCharToUTF8(const TCHAR* pSrc, char* pDest)
	{
		WideCharToMultiByte(CP_UTF8, 0, pSrc, -1, pDest, sizeof(pDest), NULL, NULL);
	}

	template<typename N, typename F>
	static void TreeLevelTraversal(N rootNode, F lamda)
	{
		queue<N> q;
		q.push(rootNode);

		while (!q.empty())
		{
			if (auto& node = q.front())
			{
				q.pop();

				{
					lamda(node);
				}

				// TODO: Get_Children() 인터페이스로 빼기
				//			현재는 GameObject만 가지고있긴함
				for (auto& pChild : *(node->Get_Children()))
				{
					q.push(pChild);
				}
			}
		}
	}
};

END