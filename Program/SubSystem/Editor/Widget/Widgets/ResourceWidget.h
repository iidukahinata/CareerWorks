/**
* @file	   ResourceWidget.h
* @brief
*
* @date	   2022/09/07 2022年度初版
*/
#pragma once


#include "../Widget.h"

class Input;
class World;
class ResourceManager;

class ResourceWidget : public Widget
{
	IN_DEVELOPMENT("リソース消去")
public:

	void Initialize() override;
	void PostInitialize() override;
	void Draw() override;

private:

	/** リソース検索メニューバーの生成 */
	void ShowResourceMenu() noexcept;

	/** 全リソース列挙 / 選択リソース情報を保持など行う。*/
	void ShowResourceList() noexcept;

	/** リソース生成などのメソッドを行う。*/
	void ShowResourceHelper() noexcept;

	/** リソース生成時の設定などの指定を行う。*/
	void ShowResourceCreateWindow();

	/** 入力値から Window の切り替えなどを行うため。*/
	void ChackClickedCommand() noexcept;

private:

	/** 
	* リソースを生成した際に、初期では使用しないため加愛豊処理を用意。
	* コマンド処理に関するリソースポインタなどが消去される可能性があるが、
	* Flush 等処理を記述していないため呼び出し側で考慮する必要がある。
	*/
	void UnloadResource(IResource* resource) noexcept;

	/** リソースの選択を行いドラッグアンドドロップなどの機能に使用 */
	void ClickResource(uint32_t type, StringView name) noexcept;

	/** 選択中オブジェクトの読み込みを行い形に適した処理を行います。*/
	void DoubleClickResource(uint32_t type, StringView name) noexcept;

	/** リソース列挙時の色設定を登録 */
	void RegisterResourceColor() noexcept;

	/** Flags */
	bool IsTimeOut() noexcept;
	bool IsShowAllResourceType() noexcept;
	bool IsShowCreateWindow() noexcept;

private:

	enum TagType
	{
		TagType_ALL = -1
	};

	// * シーン切り替え用
	World* m_world;

	// * Resource 情報取得用
	ResourceManager* m_resourceManager;

	// * ダブルクリック時間計測用
	Stopwatch m_stopwatch;

	// * 現在選択中リソース
	ResourceData* m_selectResoruce = nullptr;

	// * tag 検索で簡略化を図るため (type id を使用)
	int m_tag = -1;

	// * filter 検索用
	ImGuiTextFilter m_filter;

	// * Image を使用するまでの間 Button Color
	Map<uint32_t, ImVec4> m_color;

	// * Create などを提供する Window の表示設定
	bool m_isShowHelperWindow = false;

	// * Helper Window 表示場所
	ImVec2 m_clickedMausePos;

	// * リソース生成メソッド
	std::function<IResource*(StringView)> m_resourceCreateFunc;
};