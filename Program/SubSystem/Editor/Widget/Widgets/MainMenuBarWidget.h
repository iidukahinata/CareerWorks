/**
* @file	   MainMenuBarWidget.h
* @brief
*
* @date	   2022/10/27 2022年度初版
*/
#pragma once


#include "../Widget.h"

class World;
class Renderer;
class ResourceManager;
class Material;

class MainMenuBarWidget : public Widget
{
	WAIT_FOR_DEVELOPMENT("View && Assets など未作成")
public:

	void PostInitialize() override;
	void Draw() override;

	void Serialized(FileStream* file) const override;
	void Deserialized(FileStream* file) override;

private:

	/** 各設定を行える Window の表示を行う。*/
	void ShowSettingsWindow() noexcept;

	/** 新しい Scene の生成と名前指定 */
	void ShowNewSceneModal() noexcept;

	/** セーブ先 Scene 名の取得を行う。*/
	void ShowSaveAsModal() noexcept;

	/** プログラムの再起動確認 Window */
	void ShowRestartSystemModal() noexcept;

	/** フルスクリーン時に ImGui で WindowMenu を表示 */
	void ShowWindowMenu() noexcept;

private:

	void ShowMaterial(StringView label, Material* material, std::function<void(Material*)> collBack) noexcept;

private:

	/** ドラッグアンドドロップ時の UI 関係の表示 */
	template<class T>
	void ShowDragDropHelper(bool hovered, uint32_t r, uint32_t g, uint32_t b) const noexcept;
	void ShowDragDropHelper(uint32_t selctType, bool hovered, uint32_t r, uint32_t g, uint32_t b) const noexcept;

	/** Drag Drop されたオブジェクト取得の Helper 関数 */
	template<class T>
	T* CatchDragObject() const noexcept;
	IResource* CatchDragResourceObject(uint32_t selectType) const noexcept;

	/** リソース検索用 Helper 関数 */
	template<class T>
	ResourceData* ShowSearchResourceHelper() noexcept;
	ResourceData* ShowSearchResourceHelper(uint32_t selectType) noexcept;
	bool OpenResourceHelper(StringView lavel) noexcept;

	template<class T>
	T* LoadResource(ResourceData* resourceData) const noexcept;
	IResource* LoadResource(ResourceData* resourceData) const noexcept;

private:

	World* m_world = nullptr;

	Renderer* m_renderer = nullptr;

	ResourceManager* m_resourceManager = nullptr;

	bool m_openSettginsWindow = false;

	// * 検索 helper 関数用 filter 検索用
	ImGuiTextFilter m_searchResourceFilter;

	String m_searchResourceLavel;
};

template<class T>
FORCEINLINE void MainMenuBarWidget::ShowDragDropHelper(bool hovered, uint32_t r, uint32_t g, uint32_t b) const noexcept
{
	ShowDragDropHelper(T::TypeData.Hash, hovered, r, g, b);
}

template<class T>
FORCEINLINE T* MainMenuBarWidget::CatchDragObject() const noexcept
{
	return dynamic_cast<T*>(CatchDragResourceObject(T::TypeData.Hash));
}

template<class T>
FORCEINLINE ResourceData* MainMenuBarWidget::ShowSearchResourceHelper() noexcept
{
	return ShowSearchResourceHelper(T::TypeData.Hash);
}

template<class T>
FORCEINLINE T* MainMenuBarWidget::LoadResource(ResourceData* resourceData) const noexcept
{
	return dynamic_cast<T*>(LoadResource(resourceData));
}