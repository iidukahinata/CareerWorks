/**
* @file    ComponentFactory.h
* @brief
*
* @date	   2022/11/27 2022年度初版
*/
#pragma once


#include "../Component/IComponent.h"

/**
* コンポーネントの生成をシステムから分離させるために作成。
* 新しいコンポーネントを作成した場合、下記関数内に追加していく。
*/
struct ComponentFactory
{
public:

	/** 指定名のコンポーネントの生成を行う。生成されない場合は null オブジェクトを返す。*/
	static UniquePtr<IComponent> Create(GameObject* gameObject, StringView name) noexcept;

	/** 呼び出し時に登録されているコンポーネントタイプ（Key）を返します。*/
	static Vector<StringView> GetAllComponentType();

public:

	/** system 起動時に system に対応する各コンポーネントの登録を system から行う。*/
	template<class Key, class T = Key>
	static void Register();

	/** system 終了時に対応するコンポーネントの登録解除を行うようにする。*/
	template<class Key>
	static void UnRegister();

private:

	// * 使用コンポーネントはシステムに依存するため生成関数を登録する形にする。
	static Unordered_Map<String, std::function<UniquePtr<IComponent>()>> m_componentFactorys;
};

template<class Key, class T /* = Key */>
FORCEINLINE void ComponentFactory::Register()
{
	auto name = Key::TypeData.Name;
	if (!m_componentFactorys.contains(name.data()))
	{
		m_componentFactorys[name.data()] = []() { return std::make_unique<T>(); };
	}
}

template<class Key>
FORCEINLINE void ComponentFactory::UnRegister()
{
	auto name = Key::TypeData.Name;
	if (m_componentFactorys.contains(name))
	{
		m_componentFactorys.erase(name);
	}
}