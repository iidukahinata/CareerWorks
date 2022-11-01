/**
* @file	   TransformDetails.cpp
* @brief
*
* @date	   2022/10/27 2022�N�x����
*/


#include "TransformDetails.h"
#include "SubSystem/Scene/Component/Transform.h"

TransformDetails::TransformDetails(DetailsWidget* detailsWidget, Transform* transform) :
	DetailsObject(detailsWidget), m_transform(transform)
{
	ASSERT(m_transform);
}

void TransformDetails::Draw()
{
	constexpr auto offsetPos = 130;

	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
	{
		auto transform = m_transform;
		auto position  = transform->GetPosition();
		auto rotation  = transform->GetRotation();
		auto scale	   = transform->GetScale();

		rotation.x = Math::ToDegree(rotation.x);
		rotation.y = Math::ToDegree(rotation.y);
		rotation.z = Math::ToDegree(rotation.z);

		ImGui::Text("Position"); ImGui::SameLine(offsetPos);
		auto inputPosition = ImGui::DragFloat3("##Position", &position.x, 0.1f);
		
		ImGui::Text("Rotation"); ImGui::SameLine(offsetPos);
		auto inputRotation = ImGui::DragFloat3("##Rotation", &rotation.x, 1.0f);

		ImGui::Text("Scale"); ImGui::SameLine(offsetPos);
		auto inputScale = ImGui::DragFloat3("##Scale", &scale.x, 0.05f);

		rotation.x = Math::ToRadian(rotation.x);
		rotation.y = Math::ToRadian(rotation.y);
		rotation.z = Math::ToRadian(rotation.z);

		if (inputPosition) RegisterEditorCommand([transform](auto data) { transform->SetPosition(data); }, position, transform->GetPosition());
		if (inputRotation) RegisterEditorCommand([transform](auto data) { transform->SetRotation(data); }, rotation, transform->GetRotation());
		if (inputScale)    RegisterEditorCommand([transform](auto data) { transform->SetScale(data);    }, scale   , transform->GetScale());
	}
}