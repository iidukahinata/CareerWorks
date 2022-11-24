/**
* @file    TickFunction.h
* @brief
*
* @date	   2022/10/03 2022年度初版
*/
#pragma once


class TickFunction
{
	COMPLETED_DEVELOPMENT()
	COPY_PROHIBITED(TickFunction)
public:

	TickFunction() = default;
	virtual ~TickFunction() = default;

	virtual void Tick(double deltaTime) = 0;

	/** 更新処理を行うためには、一度実行しなければならない。*/
	void RegisterToTickManager() noexcept;

	/** TickManager に登録した場合、終了時呼び出す必要があります。*/
	void UnRegisterFromTickManager() noexcept;

	/** 更新処理の待機、実行中なら true を返す。*/
	bool IsRegistered() const noexcept;

public:

	/** TickManager への更新は関数内で行うためは呼び出し側操作は不要 */
	void SetEnable(bool enable) noexcept;
	bool GetEnable() noexcept;

	/** この設定は前提条件に指定されている TickFunction の設定にも影響します。 */
	void SetPriority(uint32_t priority) noexcept;
	uint32_t GetPriority() const noexcept;

	/** true 指定された場合、AnyThread で実行されるようになります */
	void SetThreadSafe(bool isThreadSafe) noexcept;
	bool IsThreadSafe() const noexcept;

	/** 実行時使用される経過時間をセット。*/
	void SetDeletaTime(double deltaTime) noexcept;
	double GetDeletaTime() const noexcept;

	/** 実行前の前提条件設定 */
	void AddPrerequisite(TickFunction* tickFunction) noexcept;
	void RemovePrerequisite(TickFunction* tickFunction) noexcept;
	const Vector<TickFunction*>& GetPrerequisites() const noexcept;

private:

	// * 他スレッド動作時に使用
	double m_deltaTime = 0.f;

	// * 他スレッドで実行出来るか
	bool m_isThreadSafe = false;

	// * 更新順序優先度 0 に近い値が優先される。
	uint32_t m_priority = 128;

	// * 更新処理の有効性を保持
	bool m_isEnabled = true;

	// * TickManager に登録されているかを保持
	bool m_isRegistered = false;

	// * 設定された前提条件を使用し処理順を作成するため
	Vector<TickFunction*> m_prerequisites;
};

/**
* ゲームオブジェクト更新オブジェクト
*/
class TickGameObjectFunction : public TickFunction
{
public:

	class GameObject* m_gameObject;

public:

	void Tick(double deltaTime);
};

/**
* コンポーネント更新オブジェクト
*/
class TickComponentFunction : public TickFunction
{
public:

	class IComponent* m_component;

public:

	void Tick(double deltaTime);
};