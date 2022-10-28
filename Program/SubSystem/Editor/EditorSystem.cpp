/**
* @file    EditorSystem.cpp
* @brief
*
* @date	   2022/10/27 2022年度初版
*/


#include "EditorSystem.h"
#include "DragDrop.h"
#include "SubSystem/Window/Window.h"
#include "Widget/Widgets/SceneWidget.h"
#include "Widget/Widgets/AssetsWidget.h"
#include "Widget/Widgets/DetailsWidget.h"
#include "Widget/Widgets/ConsoleWidget.h"
#include "Widget/Widgets/ProfilerWidget.h"
#include "Widget/Widgets/ViewPortWidget.h"
#include "Widget/Widgets/MainMenuBarWidget.h"
#include "SubSystem/Renderer/GraphicsAPI/D3D12/D3D12GraphicsDevice.h"

bool EditorSystem::Initialize() noexcept
{
	m_job.SetFunction([this](double) { Render(); }, FunctionType::Render);
	m_job.RegisterToJobSystem();

	RegisterWidgetsToContainer();

	return true;
}

bool EditorSystem::PostInitialize(void* shaderResourceView) noexcept
{
	if (!SetUpImGuiObjects(shaderResourceView))
	{
		return false;
	}

	SetUpGuiStyle();
	AddFonts();

	for (const auto& widget : m_widgets)
	{
		widget->PostInitialize();
	}

	// load editor setting
	if (FileSystem::Exists(EDITOR_SETTINGS_PATH))
	{
		FileStream file(EDITOR_SETTINGS_PATH, OpenMode::Read_Mode);
		ASSERT(file.IsOpen());

		// 過去設定データから Editor 情報を更新
		for (const auto& widget : m_widgets)
		{
			widget->Deserialized(&file);
		}
	}

	return true;
}

void EditorSystem::Shutdown() noexcept
{
	// save editor setting
	{
		FileStream file(EDITOR_SETTINGS_PATH, OpenMode::Write_Mode);
		ASSERT(file.IsOpen());

		for (const auto& widget : m_widgets)
		{
			widget->Serialized(&file);
		}
	}

	EditorHelper::Get().Shutdown();

	m_job.UnRegisterFromJobSystem();

	m_widgets.clear();
	m_widgets.shrink_to_fit();

	m_descriptHeap.Release();
	ImGui_ImplDX12_Shutdown();
	ImGui::DestroyContext();
}

void EditorSystem::Render() noexcept
{
	// gui 表示用 ドローコール
	D3D12GraphicsDevice::Get().SetRenderTarget();
	D3D12GraphicsDevice::Get().GetCommandContext().DrawIndexedInstanced(4, 1, 0, 0, 0);

	// start the dear ImGui frame
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ShowDockingWindow();

	// draw user gui
	for (const auto& widget : m_widgets)
	{
		widget->Draw();
	}

	ChackClickedCommand();

	// rendering
	EditorHelper::Get().BegineRenderer();
	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), D3D12GraphicsDevice::Get().GetCommandContext().GetCommandList());
}

bool EditorSystem::SetUpImGuiObjects(void* finalFrameSRV) noexcept
{
	constexpr auto maxTextureCount = 128;
	auto ret = m_descriptHeap.Create(maxTextureCount, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	if (!ret) {
		return false;
	}

	EditorHelper::Get().Initialize(&m_descriptHeap, finalFrameSRV);

	if (!ImGui::CreateContext()) {
		return false;
	}

	ret = ImGui_ImplWin32_Init(Window::Get().GetHandle());
	if (!ret) {
		return false;
	}

	ret = ImGui_ImplDX12_Init(
		D3D12GraphicsDevice::Get().GetDevice(),
		1, DXGI_FORMAT_R8G8B8A8_UNORM,
		m_descriptHeap.GetHeap(),
		m_descriptHeap.GetCPUHandle(),
		m_descriptHeap.GetGPUHandle());

	if (!ret) {
		return false;
	}

	return true;
}

void EditorSystem::SetUpGuiStyle() noexcept
{
	auto& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::StyleColorsClassic();

	auto style = &ImGui::GetStyle();
	auto colors = style->Colors;

    colors[ImGuiCol_WindowBg]             = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_FrameBg]              = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]       = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
    colors[ImGuiCol_FrameBgActive]        = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
    colors[ImGuiCol_TitleBg]              = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
    colors[ImGuiCol_TitleBgActive]        = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]     = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
	colors[ImGuiCol_MenuBarBg]		      = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_Header]               = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_HeaderHovered]        = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_HeaderActive]         = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_Button]               = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
    colors[ImGuiCol_ButtonHovered]        = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_ButtonActive]         = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    colors[ImGuiCol_CheckMark]            = ImVec4(0.90f, 0.90f, 0.90f, 0.50f);
    colors[ImGuiCol_SliderGrab]           = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
    colors[ImGuiCol_ScrollbarBg]          = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab]        = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]  = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_Tab]				  = colors[ImGuiCol_HeaderActive];
    colors[ImGuiCol_TabHovered]           = colors[ImGuiCol_HeaderHovered];
    colors[ImGuiCol_TabActive]            = colors[ImGuiCol_Header];
    colors[ImGuiCol_TabUnfocused]         = colors[ImGuiCol_HeaderActive];
    colors[ImGuiCol_TabUnfocusedActive]   = colors[ImGuiCol_HeaderActive];
    colors[ImGuiCol_DockingPreview]       = ImVec4(0.5f, 0.5f, 0.5f, 0.80f);
    colors[ImGuiCol_DockingEmptyBg]       = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
}

void EditorSystem::AddFonts() noexcept
{
	auto& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("Data/Resource/Font/UDDigiKyokashoN-R.ttc", 14.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
}

void EditorSystem::RegisterWidgetsToContainer() noexcept
{
	m_widgets.emplace_back(std::make_unique<DetailsWidget>());
	m_widgets.emplace_back(std::make_unique<ConsoleWidget>());
	m_widgets.emplace_back(std::make_unique<AssetsWidget>());
	m_widgets.emplace_back(std::make_unique<SceneWidget>());
	m_widgets.emplace_back(std::make_unique<ViewPortWidget>());
	m_widgets.emplace_back(std::make_unique<ProfilerWidget>());
	m_widgets.emplace_back(std::make_unique<MainMenuBarWidget>());
}

void EditorSystem::ShowDockingWindow() const noexcept
{
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const auto viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", 0, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	auto dockspace_id = ImGui::GetID("MyDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	ImGui::End();
}

void EditorSystem::ChackClickedCommand() noexcept
{
	// Draw 中で使用される可能性があるため
	if (ImGui::IsMouseReleased(0))
	{
		DragDrop::Get().EndDrag();
	}

	// 長押し入力時にユーザーが認識出来る速度に調整
	if (m_stopwatch.GetRap(Milli) > 120)
	{
		// Undo
		if (ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsKeyDown(ImGuiKey_Z))
		{
			EditorHelper::Get().UndoCommand();
			m_stopwatch.Start();
		}
		// Redo
		if (ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsKeyDown(ImGuiKey_Y))
		{
			EditorHelper::Get().RedoCommand();
			m_stopwatch.Start();
		}
	}
}