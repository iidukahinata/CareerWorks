/**
* @file	   ColliderDetails.cpp
* @brief
*
* @date	   2022/11/28 2022”N“x‰”Å
*/


#include "ColliderDetails.h"
#include "SubSystem/Scene/Component/ICollider.h"

ColliderDetails::ColliderDetails(DetailsWidget* detailsWidget, IComponent* component) :
	ComponentDetails(detailsWidget)
{
	m_collider = dynamic_cast<ICollider*>(component);
	ASSERT(m_collider);
}

void ColliderDetails::Draw()
{
	constexpr auto offsetPos = 130;
	constexpr auto shapeTypeCombo = "Box\0Sphere\0Capsule\0\0";

	if (ShowComponentHeader(m_collider))
	{
		auto collider		 = m_collider;
		auto shapeType		 = collider->GetShapeType();
		auto isTrigger		 = collider->GetTrigger();
		auto scale			 = collider->GetScale();
		auto contactOffset	 = collider->GetContactOffset();
		auto resetOffset	 = collider->GetRestOffset();
		auto staticFriction  = collider->GetStaticFriction();
		auto dynamicFriction = collider->GetDynamicFriction();
		auto restitution	 = collider->GetRestitution();

		ImGui::Text("ShapeType"); ImGui::SameLine(offsetPos);
		auto inputShapeType = ImGui::Combo("##ShapeType", (int*)(&shapeType), shapeTypeCombo);

		ImGui::Text("IsTrigger"); ImGui::SameLine(offsetPos);
		auto inputIsTrigger = ImGui::Checkbox("##IsTrigger", &isTrigger);

		auto inputScale = false;
		if (shapeType == ShapeType::Box)
		{
			ImGui::Text("Scale"); ImGui::SameLine(offsetPos);
			inputScale = ImGui::DragFloat3("##Scale", &scale.x, 0.1f);
		}
		else if (shapeType == ShapeType::Sphere)
		{
			ImGui::Text("Radius"); ImGui::SameLine(offsetPos);
			inputScale = ImGui::DragFloat("##Radius", &scale.x, 0.1f);
		}
		else if (shapeType == ShapeType::Capsule)
		{
			ImGui::Text("Radius"); ImGui::SameLine(offsetPos);
			inputScale |= ImGui::DragFloat("##Radius", &scale.x, 0.1f);

			ImGui::Text("Hight"); ImGui::SameLine(offsetPos);
			inputScale |= ImGui::DragFloat("##Hight", &scale.y, 0.1f);
		}

		ImGui::Text("ResetOffset"); ImGui::SameLine(offsetPos);
		auto inputResetOffset = ImGui::DragFloat("##ResetOffset", &resetOffset, 0.1f);

		ImGui::Text("ContactOffset"); ImGui::SameLine(offsetPos);
		auto inputContactOffset = ImGui::DragFloat("##ContactOffset", &contactOffset, 0.1f);

		ImGui::Text("");
		ImGui::Text("Material");

		ImGui::Text("StaticFriction"); ImGui::SameLine(offsetPos);
		auto inputStaticFriction = ImGui::DragFloat("##StaticFriction", &staticFriction, 0.1f);

		ImGui::Text("DynamicFriction"); ImGui::SameLine(offsetPos);
		auto inputDynamicFriction = ImGui::DragFloat("##DynamicFriction", &dynamicFriction, 0.1f);

		ImGui::Text("Restitution"); ImGui::SameLine(offsetPos);
		auto inputRestitution = ImGui::DragFloat("##Restitution", &restitution, 0.1f);

		if (inputShapeType)		  RegisterEditorCommand([collider](auto data) { collider->SetShapeType(data);	    }, shapeType	  , collider->GetShapeType());
		if (inputIsTrigger)		  RegisterEditorCommand([collider](auto data) { collider->SetTrigger(data);			}, isTrigger	  , collider->GetTrigger());
		if (inputScale)			  RegisterEditorCommand([collider](auto data) { collider->SetScale(data);			}, scale		  , collider->GetScale());
		if (inputResetOffset)	  RegisterEditorCommand([collider](auto data) { collider->SetRestOffset(data);	    }, resetOffset	  , collider->GetRestOffset());
		if (inputContactOffset)   RegisterEditorCommand([collider](auto data) { collider->SetContactOffset(data);	}, contactOffset  , collider->GetContactOffset());
		if (inputStaticFriction)  RegisterEditorCommand([collider](auto data) { collider->SetStaticFriction(data);	}, staticFriction , collider->GetStaticFriction());
		if (inputDynamicFriction) RegisterEditorCommand([collider](auto data) { collider->SetDynamicFriction(data); }, dynamicFriction, collider->GetDynamicFriction());
		if (inputRestitution)	  RegisterEditorCommand([collider](auto data) { collider->SetRestitution(data);	    }, restitution	  , collider->GetRestitution());
	}
}