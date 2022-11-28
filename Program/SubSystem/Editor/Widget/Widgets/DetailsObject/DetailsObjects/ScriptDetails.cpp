/**
* @file	   ScriptDetails.cpp
* @brief
*
* @date	   2022/11/28 2022年度初版
*/

#include "ScriptDetails.h"
#include "SubSystem/Scene/World.h"
#include "SubSystem/Editor/DragDrop.h"
#include "SubSystem/Resource/ResourceManager.h"
#include "SubSystem/Scene/Component/IScript.h"
#include "SubSystem/Resource/Resources/Script/ScriptInstance.h"

ScriptDetails::ScriptDetails(DetailsWidget* detailsWidget, IComponent* component) :
	ComponentDetails(detailsWidget)
{
	m_script = dynamic_cast<IScript*>(component);
	ASSERT(m_script);
}

void ScriptDetails::Draw()
{
	constexpr auto offsetPos = 130;

	if (ShowComponentHeader(m_script))
	{
		auto script = m_script;
		auto scriptPath = script->GetScript() ? script->GetScript()->GetFilePath() : "";

		ImGui::Text("Script"); ImGui::SameLine(offsetPos);

		constexpr auto itemWidth = 250;
		ImGui::PushItemWidth(itemWidth);
		ImGui::InputText("##", scriptPath.data(), scriptPath.size());
		ImGui::PopItemWidth();

		// ドラッグアンドドロップでの Script 切り替え
		const auto hoverd = ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);
		if (ImGui::IsMouseReleased(0) && hoverd)
		{
			if (auto scriptInstance = CatchDragObject<ScriptInstance>())
			{
				RegisterEditorCommand([script](auto data) { script->SetScript(data); }, scriptInstance, script->GetScript());
			}
		}

		ShowDragDropHelper<ScriptInstance>(hoverd, 34, 196, 255);

		ImGui::SameLine(offsetPos + itemWidth + 5);
		
		OpenResourceHelper("Use Script");
		if (auto resourceData = ShowSearchResourceHelper<ScriptInstance>())
		{
			auto scriptInstance = LoadResource<ScriptInstance>(resourceData);
			RegisterEditorCommand([script](auto data) { script->SetScript(data); }, scriptInstance, script->GetScript());
		}
	}
}