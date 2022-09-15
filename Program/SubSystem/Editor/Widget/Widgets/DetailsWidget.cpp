/**
* @file	   DetailsWidget.cpp
* @brief
*
* @date	   2022/09/13 2022îNìxèâî≈
*/


#include "DetailsWidget.h"
#include "Factory/DetailsObjectFactory.h"
#include "SubSystem/Scene/World.h"
#include "SubSystem/Resource/ResourceManager.h"

DetailsWidget* DetailsWidget::m_detailsWidget = nullptr;
Vector<UniquePtr<DetailsObject>> DetailsWidget::m_detailsObjects;

void DetailsWidget::PostInitialize()
{
	m_detailsWidget = this;

	m_world = GetContext()->GetSubsystem<World>();
	ASSERT(m_world);

	m_resourceManager = GetContext()->GetSubsystem<ResourceManager>();
	ASSERT(m_resourceManager);
}

void DetailsWidget::Draw()
{
	ImGui::SetNextWindowPos(ImVec2(1100, 0), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(435, 630), ImGuiCond_Once);

	ImGui::Begin("Details");

	for (auto& detailsObject : m_detailsObjects)
	{
		detailsObject->Draw();
	}

	ImGui::End();
}

World* DetailsWidget::GetWorld() const noexcept
{
	return m_world;
}

ResourceManager* DetailsWidget::GetResourceManager() const noexcept
{
	return m_resourceManager;
}

void DetailsWidget::SelectGameObject(GameObject* gameObject) noexcept
{
	if (!gameObject)
	{
		return;
	}

	m_detailsObjects = DetailsObjectFactory::Create(m_detailsWidget, gameObject);
}

void DetailsWidget::SelectResource(ResourceData* resourceData) noexcept
{
	if (!resourceData)
	{
		return;
	}

	m_detailsObjects = DetailsObjectFactory::Create(m_detailsWidget, resourceData);
}