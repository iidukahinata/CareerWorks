/**
* @file	   RigidBodyDetails.cpp
* @brief
*
* @date	   2022/11/28 2022”N“x‰”Å
*/


#include "RigidBodyDetails.h"
#include "SubSystem/Scene/Component/IRigidBody.h"

RigidBodyDetails::RigidBodyDetails(DetailsWidget* detailsWidget, IComponent* component) :
	ComponentDetails(detailsWidget)
{
	m_rigidBody = dynamic_cast<IRigidBody*>(component);
	ASSERT(m_rigidBody);
}

void RigidBodyDetails::Draw()
{
	constexpr auto offsetPos = 130;
	constexpr auto bodyTypeCombo = "Dynamic\0Static\0\0";

	if (ShowComponentHeader(m_rigidBody))
	{
		auto rigidBody	 = m_rigidBody;
		auto bodyType	 = rigidBody->GetBodyType();
		auto mass		 = rigidBody->GetMass();
		auto isKinematic = rigidBody->IsKinematic();
		auto useGraviry  = rigidBody->UseGravity();
		auto posLock	 = rigidBody->GetPositionLock();
		auto rotLock	 = rigidBody->GetRotationLock();

		ImGui::Text("BodyType"); ImGui::SameLine(offsetPos);
		auto inputBodyType = ImGui::Combo("##BodyType", (int*)(&bodyType), bodyTypeCombo);

		ImGui::Text("Mass"); ImGui::SameLine(offsetPos);
		auto inputMass = ImGui::DragFloat("##Mass", &mass, 0.1f);

		ImGui::Text("IsKinematic"); ImGui::SameLine(offsetPos);
		auto inputIsKinematic = ImGui::Checkbox("##IsKinematic", &isKinematic);

		ImGui::Text("UseGraviry"); ImGui::SameLine(offsetPos);
		auto inputUseGraviry = ImGui::Checkbox("##UseGraviry", &useGraviry);

		ImGui::Text("Position Lock"); ImGui::SameLine(offsetPos);
		auto inputPosLock = ImGui::Checkbox("##PosLockX", (bool*)&posLock.x); ImGui::SameLine();
		inputPosLock |= ImGui::Checkbox("##PosLockY", (bool*)&posLock.y); ImGui::SameLine();
		inputPosLock |= ImGui::Checkbox("##PosLockZ", (bool*)&posLock.z);

		ImGui::Text("Rotation Lock"); ImGui::SameLine(offsetPos);
		auto inputRotLock = ImGui::Checkbox("##RotLockX", (bool*)&rotLock.x); ImGui::SameLine();
		inputRotLock |= ImGui::Checkbox("##RotLockY", (bool*)&rotLock.y); ImGui::SameLine();
		inputRotLock |= ImGui::Checkbox("##RotLockZ", (bool*)&rotLock.z);

		if (inputBodyType)    RegisterEditorCommand([rigidBody](auto data) { rigidBody->SetBodyType(data);	   }, bodyType	  , rigidBody->GetBodyType());
		if (inputMass)		  RegisterEditorCommand([rigidBody](auto data) { rigidBody->SetMass(data);		   }, mass		  , rigidBody->GetMass());
		if (inputIsKinematic) RegisterEditorCommand([rigidBody](auto data) { rigidBody->SetKinematic(data);	   }, isKinematic , rigidBody->IsKinematic());
		if (inputUseGraviry)  RegisterEditorCommand([rigidBody](auto data) { rigidBody->SetUseGravity(data);   }, useGraviry  , rigidBody->UseGravity());
		if (inputPosLock)	  RegisterEditorCommand([rigidBody](auto data) { rigidBody->SetPositionLock(data); }, posLock	  , rigidBody->GetPositionLock());
		if (inputRotLock)	  RegisterEditorCommand([rigidBody](auto data) { rigidBody->SetRotationLock(data); }, rotLock	  , rigidBody->GetRotationLock());
	}
}