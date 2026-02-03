#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
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

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// 윈도우즈 기본 폰트 추가
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesKorean());
	
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
	//style.FontScaleDpi = main_scale;        // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)
	//style.ScaleAllSizes

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(pGraphicDev);

	return S_OK;
}

void CImguiMgr::Update_Imgui()
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

}

void CImguiMgr::Render_Imgui(LPDIRECT3DDEVICE9 pGraphicDev)
{
	pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
}

bool CImguiMgr::WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	return false;
}

void CImguiMgr::Before_DeviceReset()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
}

void CImguiMgr::After_DeviceReset()
{
	ImGui_ImplDX9_CreateDeviceObjects();
}

void CImguiMgr::Free()
{
	// Cleanup
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}