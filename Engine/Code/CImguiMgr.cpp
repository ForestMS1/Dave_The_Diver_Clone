#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "ImGuizmo.h"
#include "CImguiMgr.h"

LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

IMPLEMENT_SINGLETON(CImguiMgr)

CImguiMgr::CImguiMgr()
{
}

CImguiMgr::~CImguiMgr()
{
	Free();
}

HRESULT CImguiMgr::Ready_Imgui(HWND hWnd, LPDIRECT3DDEVICE9 pGraphicDev)
{
	ImGui_ImplWin32_EnableDpiAwareness();
	float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));
	
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();


	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
	//style.FontScaleDpi = main_scale;        // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)
	//style.ScaleAllSizes

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// TODO: 한글폰트 로딩하면 메모리많이쓰던데 정상인지확인 필요
	//	- 26.2.4 한글폰트 로딩 하면 176 메가점유, 로딩안하면 47 메가 점유
	// io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesKorean());
	
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();


	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(pGraphicDev);

	return S_OK;
}

void CImguiMgr::Update_Imgui()
{
	if (!ImGui::GetCurrentContext()) return;

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

}

void CImguiMgr::Render_Imgui(LPDIRECT3DDEVICE9 pGraphicDev)
{
	if (!ImGui::GetCurrentContext()) return;

	pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
}

bool CImguiMgr::WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (!ImGui::GetCurrentContext()) return false;

	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	return false;
}

void CImguiMgr::Before_DeviceReset()
{
	if (!ImGui::GetCurrentContext()) return;

	ImGui_ImplDX9_InvalidateDeviceObjects();
}

void CImguiMgr::After_DeviceReset()
{
	if (!ImGui::GetCurrentContext()) return;

	ImGui_ImplDX9_CreateDeviceObjects();
}

void CImguiMgr::Free()
{
	if (!ImGui::GetCurrentContext()) return;

	// Cleanup
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}