/**
* @file		JobHelper.h
* @brief
*
* @date		2022/09/07 2022年度初版
*/
#pragma once


/**
* Job 実行タイミング制御用 enum
*/
enum FunctionType
{
	// フレーム開始時に実行。主に各 Thread の立ち上げを行う。
	PreUpdate,

	// 物理計算前に実行。World 更新を行う。
	PrePhysics,

	// 物理計算の開始時に実行。
	StartPhysics,

	// 物理計算後に実行。
	EndPhysics,

	// 全ての更新が終了した時に実行。
	PostUpdate,

	// 描画開始時に実行。
	Render,

	// 全ての描画処理が終了した時に実行中。Preset などを行う。
	PostRender,

	None,
};