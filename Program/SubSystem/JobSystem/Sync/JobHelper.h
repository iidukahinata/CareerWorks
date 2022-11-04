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
	PreUpdate,

	Update,

	PostUpdate,

	Render,

	PostRender,

	None,
};