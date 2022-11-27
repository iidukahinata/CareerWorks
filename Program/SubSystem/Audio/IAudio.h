/**
* @file    Audio.h
* @brief
*
* @date	   2022/08/02 2022年度初版
*/
#pragma once


#include "SubSystem/Core/ISubsystem.h"

class AudioListener;

/**
* Audio抽象クラス
* このクラスではインターフェースのみを実装。
* 派生クラスで初期化等の関数を実装する。
*/
class IAudio : public ISubsystem
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(IAudio)
public:

	virtual ~IAudio() {}

	/** 3D サウンドを使用する場合は必ず値をセットする必要があります。*/
	void SetAudioListener(AudioListener* listener) noexcept;

protected:

	AudioListener* m_lisrener = nullptr;
};