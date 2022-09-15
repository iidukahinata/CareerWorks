/**
* @file    MyGui.cpp
* @brief   gui管理クラス
*
* @date	   2022/09/15 2022年度初版
*/


#include "EditorSystem.h"
#include "DragDrop.h"
#include "SubSystem/Window/Window.h"
#include "Widget/Widgets/LogWidget.h"
#include "Widget/Widgets/SceneWidget.h"
#include "Widget/Widgets/DetailsWidget.h"
#include "Widget/Widgets/ProfilerWidget.h"
#include "Widget/Widgets/ResourceWidget.h"
#include "SubSystem/Renderer/GraphicsAPI/D3D12/D3D12GrahicsDevice.h"

bool EditorSystem::Initialize() noexcept
{
	m_job.SetFunction([this](double) { Render(); }, FunctionType::Render);
	m_job.RegisterToJobSystem();

	RegisterWidgetsToContainer();

	for (const auto& widget : m_widgets)
	{
		widget->Initialize();
	}

	return true;
}

bool EditorSystem::PostInitialize() noexcept
{
	if (!SetUpImguiObjects())
	{
		return false;
	}

	for (const auto& widget : m_widgets)
	{
		widget->PostInitialize();
	}

	return true;
}

void EditorSystem::Shutdown() noexcept
{
	m_job.UnRegisterFromJobSystem();

	m_widgets.clear();

	ImGui_ImplDX12_Shutdown();
	ImGui::DestroyContext();
}

void EditorSystem::Render() noexcept
{
	// start the dear ImGui frame
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	EditorHelper::Get().AddTexture(&m_descriptHeap);

	// draw user gui
	for (const auto& widget : m_widgets)
	{
		widget->Draw();
	}

	ChackClickedCommand();

	// rendering
	EditorHelper::Get().BegineRenderer();
	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), D3D12GrahicsDevice::Get().GetCommandContext().GetCommandList());

	EditorHelper::Get().Reset();
}

bool EditorSystem::SetUpImguiObjects() noexcept
{
	auto ret = m_descriptHeap.Create(1, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	if (!ret) {
		return false;
	}

	if (!ImGui::CreateContext()) {
		return false;
	}

	ret = ImGui_ImplWin32_Init(Window::Get().GetHandle());
	if (!ret) {
		return false;
	}

	ret = ImGui_ImplDX12_Init(
		D3D12GrahicsDevice::Get().GetDevice(),
		1, DXGI_FORMAT_R8G8B8A8_UNORM,
		m_descriptHeap.GetHeap(),
		m_descriptHeap.GetCPUHandle(),
		m_descriptHeap.GetGPUHandle());

	if (!ret) {
		return false;
	}

	ImGui::StyleColorsClassic();

    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;
	
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.40f, 0.80f, 0.90f);

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("Data/Resource/Font/UDDigiKyokashoN-R.ttc", 14.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());

	return true;
}

void EditorSystem::RegisterWidgetsToContainer() noexcept
{
	m_widgets.emplace_back(std::make_unique<LogWidget>());
	m_widgets.emplace_back(std::make_unique<SceneWidget>());
	m_widgets.emplace_back(std::make_unique<DetailsWidget>());
	m_widgets.emplace_back(std::make_unique<ProfilerWidget>());
	m_widgets.emplace_back(std::make_unique<ResourceWidget>());
}

void EditorSystem::ChackClickedCommand() noexcept
{
	// Draw 中で使用される可能性があるため
	if (ImGui::IsMouseReleased(0))
	{
		DragDrop::Get().EndDrag();
	}

	// Undo
	if (ImGui::IsKeyDown(VK_CONTROL) && ImGui::IsKeyReleased(0x5A /* = Z */))
	{
		EditorHelper::Get().UndoCommand();
	}
	// Redo
	if (ImGui::IsKeyDown(VK_CONTROL) && ImGui::IsKeyReleased(0x59 /* = Y */))
	{
		EditorHelper::Get().RedoCommand();
	}
}