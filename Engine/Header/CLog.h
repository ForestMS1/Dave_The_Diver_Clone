#pragma once
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CLog
{
public:
	static void Debug(const _tchar* format, ...) {
#ifdef _DEBUG
		_tchar buffer[1024];
		const _tchar* prefix = L"[DEBUG] ";

		// 1. 프리픽스를 먼저 버퍼에 씀 (쓰여진 글자 수를 반환받음)
		int offset = swprintf_s(buffer, _countof(buffer), L"%s", prefix);

		// 2. 가변 인자 처리
		va_list args;
		va_start(args, format);

		// buffer + offset 위치부터 쓰기 시작, 남은 크기는 전체 - offset
		vswprintf_s(buffer + offset, _countof(buffer) - offset, format, args);

		va_end(args);

		OutputDebugStringW(buffer); 
#endif
	}

	static void Info(const _tchar* format, ...) {
		_tchar buffer[1024];
		const _tchar* prefix = L"[INFO] ";

		// 1. 프리픽스를 먼저 버퍼에 씀 (쓰여진 글자 수를 반환받음)
		int offset = swprintf_s(buffer, _countof(buffer), L"%s", prefix);

		// 2. 가변 인자 처리
		va_list args;
		va_start(args, format);

		// buffer + offset 위치부터 쓰기 시작, 남은 크기는 전체 - offset
		vswprintf_s(buffer + offset, _countof(buffer) - offset, format, args);

		va_end(args);

		OutputDebugStringW(buffer);
	}
};

END