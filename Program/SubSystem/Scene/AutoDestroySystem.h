/**
* @file    AutoDestroySystem.h
* @brief
*
* @date	   2022/11/04 2022îNìxèâî≈
*/
#pragma once


class GameObject;

class AutoDestroySystem
{
public:

	void AddAutoDestroy(GameObject* gameObject) noexcept;

	void RemoveAutoDestroy(GameObject* gameObject) noexcept;

public:

	void Initialize() noexcept;

	void Tick() noexcept;

	void Shutdown() noexcept;

private:

	bool AllComponentsCanErase(GameObject* gameObject) const noexcept;

private:

	Job m_job;

	Set<GameObject*> m_gameObjects;
};