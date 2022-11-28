/**
* @file    IAudio.h
* @brief
*
* @date	   2022/11/28 2022年度初版
*/
#pragma once


#include "SubSystem/Core/ISubsystem.h"

class IAudioListener;

/**
* Audio抽象クラス
* このクラスではインターフェースのみを実装。
* 派生クラスで初期化等の関数を実装する。
*/
class IAudio : public ISubsystem
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(IAudio, ISubsystem)
public:

	virtual ~IAudio() {}

	/** 3D サウンドを使用する場合は必ず値をセットする必要があります。*/
	void SetAudioListener(IAudioListener* listener) noexcept;

protected:

	IAudioListener* m_lisrener = nullptr;
};