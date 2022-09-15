/**
 * @file	Timer.h
 * @brief   フレーム管理クラス
 *
 * @date	2022/09/06 2022年度初版
 */
#pragma once


#include "SubSystem/Core/ISubsystem.h"

class Timer : public ISubsystem
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(Timer)
public:

	bool Initialize() override;

	/**
	* 経過時間と設定された FPS からフレームの状態を返す。
	* 2022/04/24時点では制度が落ちるため、残りフレーム待ち時間での待ちを実装していない。
	*/
	bool ReachedNextFrame() noexcept;

	/** 中断されたアプリケーション再開時に時間調整のために使用。*/
	void ResetMeasurement() noexcept;

	/** FPS 値 変更時に使用 */
	void SetFPS(float fps) noexcept;

	/** 最後に ReachedNextFrame 関数で更新された時点の経過時間を返す */
	double GetDeltaTime() const noexcept;

private:

	// * 最新フレーム更新時間
	std::chrono::high_resolution_clock::time_point m_previousTime;

	// * フレーム経過時間(秒)
	double m_deltaTime;

	// * 設定中のFPS値を秒に変換した値
	double m_fpsTime;
};