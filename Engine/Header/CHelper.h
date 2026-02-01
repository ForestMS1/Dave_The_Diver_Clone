#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CHelper
{
public:
	// 주의!! 동적할당된거 말고 스택에 할당된 주소로 해야함 sizeof때문에
	static void ConvertTCharToUTF8(const TCHAR* pSrc, char* pDest)
	{
		WideCharToMultiByte(CP_UTF8, 0, pSrc, -1, pDest, sizeof(pDest), NULL, NULL);
	}
};

END