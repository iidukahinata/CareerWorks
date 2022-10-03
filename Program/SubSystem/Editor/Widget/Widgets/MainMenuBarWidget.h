/**
* @file	   MainMenuBarWidget.h
* @brief
*
* @date	   2022/09/30 2022îNìxèâî≈
*/
#pragma once


#include "../Widget.h"

class World;
class ResourceManager;

class MainMenuBarWidget : public Widget
{
	COMPLETED_DEVELOPMENT()
public:

	void PostInitialize() override;
	void Draw() override;

private:

	void ShowSaveAsModal() noexcept;

private:

	World* m_world;

	ResourceManager* m_resourceManager;
};