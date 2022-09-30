/**
* @file	   AssetsWidget.h
* @brief
*
* @date	   2022/09/27 2022年度初版
*/
#pragma once


#include "../Widget.h"

class World;
class ResourceManager;

class AssetsWidget : public Widget
{
	IN_DEVELOPMENT("リソース消去")
public:

	void PostInitialize() override;
	void Draw() override;

private:

	/** リソース検索メニューバーの生成 */
	void ShowResourceMenu() noexcept;

	/** 全リソース列挙 / 選択リソース情報を保持など行う。*/
	void ShowResourceList() noexcept;

	/** 選択中 Directory 内リソースの列挙 */
	void ShowCurrentDirectoryResourceList() noexcept;

	/** フィルター検索時に指定名のリソースのみ列挙 */
	void ShowResourceListByName() noexcept;

	/** リソース生成などのメソッドを行う。*/
	void ShowResourceHelper() noexcept;

	/** リソース生成時の設定などの指定を行う。*/
	bool ShowCreateWindow(std::function<void(StringView)> createFunc) noexcept;

	/** 入力値から Window の切り替えなどを行うため。*/
	void ChackClickedCommand(IconType type, StringView name) noexcept;

	void DrawThumbnail(IconType type, StringView name) noexcept;

private:

	/** 
	* リソースを生成した際に、初期では使用しないため解放処理を用意。
	* コマンド処理に関するリソースポインタなどが消去される可能性があるが、
	* Flush 等処理を記述していないため呼び出し側で考慮する必要がある。
	*/
	void UnloadResource(IResource* resource) noexcept;

	/** リソースの選択を行いドラッグアンドドロップなどの機能に使用 */
	void ClickResource(IconType type, StringView name) noexcept;

	/** 選択中オブジェクトの読み込みを行い形に適した処理を行います。*/
	void DoubleClickResource(IconType type, StringView name) noexcept;

	/** 選択された Directory 内表示物の列挙 */
	void NavigateToDirectory(StringView path) noexcept;

	/** Currnet Directory から一つ上の Directory に戻す */
	void CurrentDirectoryToParent() noexcept;

	/** Flags */
	bool IsTimeOut() noexcept;

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

private:

	// * tag 検索で簡略化を図るため (type id を使用)
	int m_tag = -1;

	// * filter 検索用
	ImGuiTextFilter m_filter;

	bool m_isSelectDirectory;

	// * 現在表示 Directory 名
	String m_currentDirectory;

	// * Assets Directory からのファイル構成
	Vector<String> m_directoryTree;

	struct Thumbnail
	{
		String name;
		IconType type;
	};

	// * Current Directory に所属物のリスト
	Vector<Thumbnail> m_directoryEntries;

	// * 現在選択中リソース名
	String m_selectResourceName;

	// * 現在選択中リソース
	ResourceData* m_selectResoruce = nullptr;

private:

	// * Helper Window 表示場所
	ImVec2 m_clickedMausePos;

	// * リソース生成メソッド
	std::function<IResource*(StringView)> m_resourceCreateFunc;
};