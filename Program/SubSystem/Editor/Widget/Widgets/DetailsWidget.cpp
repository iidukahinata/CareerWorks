/**
* @file	   DetailsWidget.cpp
* @brief
*
* @date	   2022/10/21 2022年度初版
*/


#include "DetailsWidget.h"
#include "Factory/DetailsObjectFactory.h"
#include "SubSystem/Scene/World.h"
#include "SubSystem/Resource/ResourceManager.h"

bool							 DetailsWidget::m_requestUpdate		 = false;
GameObject*						 DetailsWidget::m_selectGameObject	 = nullptr;
ResourceData*					 DetailsWidget::m_selectResourceData = nullptr;
DetailsWidget*					 DetailsWidget::m_detailsWidget		 = nullptr;
Vector<UniquePtr<DetailsObject>> DetailsWidget::m_detailsObjects;

DetailsWidget::~DetailsWidget()
{
	ClearSelectObject();
}

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
	ImGui::SetNextWindowPos(ImVec2(1100, 20), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(440, 845), ImGuiCond_FirstUseEver);

	ImGui::Begin("Details", nullptr, ImGuiWindowFlags_NoCollapse);

	for (auto& detailsObject : m_detailsObjects)
	{
		detailsObject->Draw();
		ImGui::Separator();
	}

	// details 描画中での変更が行えないため安全な時間を作成
	if (m_requestUpdate) 
	{
		if (m_selectGameObject)	  SelectGameObject(m_selectGameObject);
		if (m_selectResourceData) SelectResource(m_selectResourceData);
		m_requestUpdate = false;
	}

	ImGui::End();
}

void DetailsWidget::RequestUpdate() noexcept
{
	m_requestUpdate = true;
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

	if (!m_requestUpdate)
	{
		if (m_selectGameObject == gameObject)
		{
			return;
		}
	}

	if (m_selectResourceData)
	{
		// 元に戻す処理時に DetailsObjects が生成させるため
		RegisterEditorCommand([](auto data) { SelectResourceInternal(data); }, (ResourceData*)(0), m_selectResourceData);
	}

	if (m_selectGameObject)
	{
		RegisterEditorCommand([](auto data) { SelectGameObjectInternal(data); }, gameObject, m_selectGameObject);
	}
	else
	{
		SelectGameObjectInternal(gameObject);
	}
}

void DetailsWidget::SelectResource(ResourceData* resourceData) noexcept
{
	if (!resourceData)
	{
		return;
	}

	if (!m_requestUpdate)
	{
		if (m_selectResourceData == resourceData)
		{
			return;
		}
	}

	if (m_selectGameObject)
	{
		// 元に戻す処理時に DetailsObjects が生成させるため
		RegisterEditorCommand([](auto data) { SelectGameObjectInternal(data); }, (GameObject*)(0), m_selectGameObject);
	}

	if (m_selectResourceData)
	{
		RegisterEditorCommand([](auto data) { SelectResourceInternal(data); }, resourceData, m_selectResourceData);
	}
	else
	{
		SelectResourceInternal(resourceData);
	}
}

void DetailsWidget::ClearSelectObject() noexcept
{
	m_selectGameObject = nullptr;
	m_selectResourceData = nullptr;

	m_detailsObjects.clear();
	m_detailsObjects.shrink_to_fit();
}

void DetailsWidget::SelectGameObjectInternal(GameObject* gameObject) noexcept
{
	if (gameObject)
	{
		ClearSelectObject();

		m_selectGameObject = gameObject;
		m_detailsObjects = DetailsObjectFactory::Create(m_detailsWidget, gameObject);
	}
}

void DetailsWidget::SelectResourceInternal(ResourceData* resourceData) noexcept
{
	if (resourceData)
	{
		ClearSelectObject();

		m_selectResourceData = resourceData;
		m_detailsObjects = DetailsObjectFactory::Create(m_detailsWidget, resourceData);
	}
}