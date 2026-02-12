#pragma once

#include "Engine_Define.h"
#include <queue>
#include "CGraphicDev.h"
#include "CInfoMgr.h"

BEGIN(Engine)

class ENGINE_DLL CHelper
{
public:
	static std::string WStringToString(std::wstring_view sv)
	{
		std::string s;
		s.assign(sv.begin(), sv.end());
		return s;
	}
	static std::wstring StringToWString(std::string_view sv)
	{
		std::wstring s;
		s.assign(sv.begin(), sv.end());
		return s;
	}
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

	static void GetMousePointRay(_vec3* pRayPos, _vec3* pRayDir)
	{
		LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

		POINT ptMouse{};
		GetCursorPos(&ptMouse);

		ScreenToClient(CInfoMgr::GetInstance()->Get_HWND(), &ptMouse);

		_vec3		vMousePos;

		_D3DVIEWPORT9 ViewPort;
		ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));

		pGraphicDev->GetViewport(&ViewPort);

		// 뷰포트 -> 투영
		vMousePos.x = ptMouse.x / (ViewPort.Width * 0.5f) - 1.f;
		vMousePos.y = ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f;
		vMousePos.z = 0.f;

		// 투영 -> 뷰 스페이스
		_matrix matProj;
		pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
		D3DXMatrixInverse(&matProj, 0, &matProj);
		D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

		// 뷰 스페이스 -> 월드
		_matrix matView;
		pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
		D3DXMatrixInverse(&matView, 0, &matView);

		_vec3 vRayPos{ 0.f, 0.f, 0.f };
		_vec3 vRayDir = vMousePos - vRayPos;

		D3DXVec3TransformCoord(pRayPos, &vRayPos, &matView);
		D3DXVec3TransformNormal(pRayDir, &vRayDir, &matView);
	}

	static void GetMousePointInWorld(_vec3* pOut)
	{
		LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(CInfoMgr::GetInstance()->Get_HWND(), &pt);

		D3DVIEWPORT9 vp;
		pGraphicDev->GetViewport(&vp);

		_matrix matProj, matView;
		pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
		pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

		_matrix matInvView, matInvProj;
		D3DXMatrixInverse(&matInvView, 0, &matView);
		D3DXMatrixInverse(&matInvProj, 0, &matProj);

		// NDC
		float ndcX = (2.f * pt.x / vp.Width) - 1.f;
		float ndcY = 1.f - (2.f * pt.y / vp.Height);

		// Near
		_vec3 rayStart(ndcX, ndcY, 0.f);
		D3DXVec3TransformCoord(&rayStart, &rayStart, &matInvProj);
		D3DXVec3TransformCoord(&rayStart, &rayStart, &matInvView);

		// Far
		_vec3 rayEnd(ndcX, ndcY, 1.f);
		D3DXVec3TransformCoord(&rayEnd, &rayEnd, &matInvProj);
		D3DXVec3TransformCoord(&rayEnd, &rayEnd, &matInvView);

		_vec3 rayDir = rayEnd - rayStart;
		D3DXVec3Normalize(&rayDir, &rayDir);

		// Z=0 평면과 교차
		float t = -rayStart.z / rayDir.z;

		*pOut = rayStart + rayDir * t;
	}
};

END