/**
* @file	   ViewPortWidget.cpp
* @brief
*
* @date	   2022/10/25 2022年度初版
*/


#include "ViewPortWidget.h"
#include "DetailsWidget.h"
#include "SubSystem/Scene/World.h"

void ViewPortWidget::PostInitialize()
{
	m_world = GetContext()->GetSubsystem<World>();
	ASSERT(m_world);
}

void ViewPortWidget::Draw()
{
	ImGui::SetNextWindowPos(ImVec2(220, 20), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(880, 530), ImGuiCond_FirstUseEver);

	ImGui::Begin("View Port", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	ShowToolBar();
	ShowViewPort();

	ImGui::End();
}

void ViewPortWidget::ShowToolBar() noexcept
{
	constexpr auto offset = 100;
	auto centorPos = ImGui::GetWindowWidth() / 2.f;
	
	// Editor State
	auto mode = !m_isPlay ? "Editor" : "Game";
	ImGui::Text(mode);

	// State Button
	ImGui::SameLine(ImGui::GetWindowWidth() / 2.f);

	auto isPlay = ShowStateButton(m_isPlay, []() { return ImGui::ArrowButton("##left", ImGuiDir_Right); }); ImGui::SameLine();
	auto isStop = ShowStateButton(m_isPouse, []() { return ImGui::Button("||"); });

	// View State
	ImGui::SameLine(ImGui::GetWindowWidth() - offset);
	ImGui::Text("Aspect 16 : 9");

	ImGui::SetCursorPos(ImVec2(0.0f, 50.0f));
	ImGui::Separator();

	if (!m_world->GetCurrentScene() && (isPlay || isStop))
	{
		LOG_ERROR("Sceneをセットしてください。");
		return;
	}

	if (isPlay)
	{
		m_isPlay = !m_isPlay;
		auto state = m_isPlay ? EditorState::Run : EditorState::Stop;
		NotifyEvent<ChangeEditorStateEvent>(state);

		if (!m_isPlay)
		{
			DetailsWidget::ClearSelectObject();
			m_isPouse = false;
		}
	}

	if (m_isPlay && isStop)
	{
		m_isPouse = !m_isPouse;
		auto state = m_isPouse ? EditorState::Pause : EditorState::Unpause;
		NotifyEvent<ChangeEditorStateEvent>(state);
	}
}

void ViewPortWidget::ShowViewPort() noexcept
{
	auto texture = EditorHelper::Get().GetFinalFrameTexture();
	auto width	 = (int)ImGui::GetWindowWidth();
	auto height  = (int)ImGui::GetWindowHeight() - 52;

	const auto wRate = (width / 16);
	const auto hRate = (height / 9);
	const auto rate = wRate < hRate ? wRate : hRate;

	width = rate * 16;
	height = rate * 9;

	ImGui::SetCursorPos(ImVec2(0.0f, 55.0f));
	EditorHelper::Get().AddImage(texture, ImVec2(width, height));
}

bool ViewPortWidget::ShowStateButton(bool state, std::function<bool()> func) noexcept
{
	bool result = false;
	
	if (state)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.2f, 1.00f));
		result = func();
		ImGui::PopStyleColor();
	}
	else
	{
		result = func();
	}

	return result;
}