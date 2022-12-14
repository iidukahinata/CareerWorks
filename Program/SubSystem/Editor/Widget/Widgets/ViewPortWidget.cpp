/**
* @file	   ViewPortWidget.cpp
* @brief
*
* @date	   2022/10/31 2022年度初版
*/


#include "ViewPortWidget.h"
#include "DetailsWidget.h"
#include "SubSystem/Scene/World.h"
#include "SubSystem/Renderer/IRenderer.h"
#include "SubSystem/Script/ScriptEngine.h"
#include "SubSystem/Scene/Component/Components/Camera.h"
#include "ThirdParty/ImGuizmo/ImGuizmo.h"

void ViewPortWidget::PostInitialize()
{
	m_world = GetContext()->GetSubsystem<World>();
	ASSERT(m_world);

	m_renderer = GetContext()->GetSubsystem<IRenderer>();
	ASSERT(m_renderer);

	m_scriptEngine = GetContext()->GetSubsystem<ScriptEngine>();
	ASSERT(m_scriptEngine);
}

void ViewPortWidget::Draw()
{
	ImGui::SetNextWindowPos(ImVec2(220, 20), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(880, 530), ImGuiCond_FirstUseEver);

	ImGui::Begin("View Port", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	ShowToolBar();
	ShowViewPort();
	InputHandle();

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
	ImGui::SameLine(centorPos);

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
		EditorHelper::Get().SetIsChangeScene(true);

		m_isPlay = !m_isPlay;
		auto state = m_isPlay ? EditorState::Run : EditorState::Stop;
		NotifyEvent<ChangeEditorStateEvent>(state);

		CancelEvents();

		if (!m_isPlay)
		{
			m_scriptEngine->UnRegisterEventLisneterList();
			DetailsWidget::ClearSelectObject();
			m_isPouse = false;
		}
		else 
		{
			m_scriptEngine->RegisterEventLisneterList();
		}
	}
	else
	{
		EditorHelper::Get().SetIsChangeScene(false);
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
	constexpr auto cursorPos = ImVec2(0.0f, 55.0f);
	auto texture = EditorHelper::Get().GetFinalFrameTexture();
	auto width	 = (int)ImGui::GetWindowWidth();
	auto height  = (int)ImGui::GetWindowHeight() - 52;

	// view のアスペクトを合わせる
	{
		const auto wRate = (width / 16);
		const auto hRate = (height / 9);
		const auto rate = wRate < hRate ? wRate : hRate;

		width = rate * 16;
		height = rate * 9;
	}

	ImGui::SetCursorPos(cursorPos);
	EditorHelper::Get().AddImage(texture, ImVec2(width, height));

	// editor mode 時はギズモ表示
	if (!m_isPlay)
	{
		Show3DGuizmo(ImGui::GetCursorScreenPos(), width, height);
	}
}

void ViewPortWidget::InputHandle() noexcept
{
	if (m_isPlay)
	{
		return;
	}

	if (ImGui::IsPopupOpen("", ImGuiPopupFlags_AnyPopupId))
	{
		return;
	}

	if (!ImGui::IsMouseDown(1))
	{
		return;
	}

	auto camera = m_renderer->GetMainCamera();
	if (!camera)
	{
		return;
	}

	auto& transform = camera->GetTransform();
	auto& forward = transform.GetForward();
	auto& right = transform.GetRight();
	auto& up = transform.GetUp();

	if (ImGui::IsKeyDown(ImGuiKey_W))
	{
		transform.SetPosition(transform.GetPosition() + forward * 0.5f);
	}
	if (ImGui::IsKeyDown(ImGuiKey_A))
	{
		transform.SetPosition(transform.GetPosition() - right * 0.5f);
	}
	if (ImGui::IsKeyDown(ImGuiKey_S))
	{
		transform.SetPosition(transform.GetPosition() - forward * 0.5f);
	}
	if (ImGui::IsKeyDown(ImGuiKey_D))
	{
		transform.SetPosition(transform.GetPosition() + right * 0.5f);
	}
	if (ImGui::IsKeyDown(ImGuiKey_Q))
	{
		transform.SetPosition(transform.GetPosition() - up * 0.5f);
	}
	if (ImGui::IsKeyDown(ImGuiKey_E))
	{
		transform.SetPosition(transform.GetPosition() + up * 0.5f);
	}

	auto delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
	auto eular = transform.GetRotation().GetEuler();
	eular.x += delta.y / 10.f; eular.y += delta.x / 10.f;
	eular.x = std::clamp(eular.x, -80.0f, 85.0f);
	transform.SetRotation(Math::Quaternion::FromEuler(eular));
	ImGui::ResetMouseDragDelta(ImGuiMouseButton_Right);
}

void ViewPortWidget::Show3DGuizmo(const ImVec2& cursorPos, float imageWidth, float imageHeight) noexcept
{
	ImGui::SameLine(10);
	if (ImGui::RadioButton("Translate", m_guizmoMode == GuizmoMode::Translate))
	{
		m_guizmoMode = GuizmoMode::Translate;
	}

	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate", m_guizmoMode == GuizmoMode::Rotate))
	{
		m_guizmoMode = GuizmoMode::Rotate;
	}

	ImGui::SameLine();
	if (ImGui::RadioButton("Scale", m_guizmoMode == GuizmoMode::Scale))
	{
		m_guizmoMode = GuizmoMode::Scale;
	}

	ImGuizmo::SetOrthographic(false);
	ImGuizmo::SetDrawlist();
	ImGuizmo::SetRect(cursorPos.x, 55, imageWidth, imageHeight);

	const auto camera = m_renderer->GetMainCamera();
	if (!camera)
	{
		return;
	}

	const auto view = camera->GetViewMatrix();
	const auto projection = camera->GetProjectionMatrix();

	ImGuizmo::OPERATION operation;
	switch (m_guizmoMode) {
	case GuizmoMode::Translate: operation = ImGuizmo::OPERATION::TRANSLATE; break;
	case GuizmoMode::Rotate: operation = ImGuizmo::OPERATION::ROTATE; break;
	case GuizmoMode::Scale: operation = ImGuizmo::OPERATION::SCALE; break;
	}

	if (auto gameObject = DetailsWidget::GetSelectGameObject())
	{
		auto& transform = gameObject->GetTransform();
		auto matrix = transform.GetLocalMatrix();
		auto delta = Math::Vector3::Zero;
		
		auto isMove = ImGuizmo::Manipulate(&view.m[0][0], &projection.m[0][0], operation, ImGuizmo::LOCAL, &matrix.m[0][0], &delta.x);
		if (ImGuizmo::IsUsing() && isMove)
		{
			Math::Vector3 pos, angle, scale;
			ImGuizmo::DecomposeMatrixToComponents(&matrix.m[0][0], &pos.x, &angle.x, &scale.x);

			auto rot = transform.GetRotation() * Math::Quaternion::FromEuler(delta);

			transform.SetPosition(pos);
			transform.SetRotation(rot);
			transform.SetScale(scale);
		}
	}
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

void ViewPortWidget::CancelEvents() noexcept
{
	CancelEvent<KeyPressedEvent>(true);
	CancelEvent<KeyReleasedEvent>(true);

	if (!m_isPlay)
	{
		CancelEvent<CollisionEnterEvent>(true);
		CancelEvent<CollisionStayEvent>(true);
		CancelEvent<CollisionExitEvent>(true);
		CancelEvent<TriggerEnterEvent>(true);
		CancelEvent<TriggerStayEvent>(true);
		CancelEvent<TriggerExitEvent>(true);
	}
}