/**
* @file    ILightMap.h
* @brief
*
* @date	   2022/11/28 2022年度初版
*/
#pragma once


class ILight;
class ICamera;

/**
* 他クラスと分離させるために各インターフェイス関数を持つ Ligth 管理抽象クラスとして実装変更した。
*/
class ILightMap
{
	COMPLETED_DEVELOPMENT()
	SUPER_CLASS(ILightMap)
public:

	virtual ~ILightMap();

	/** 各 Light 定数バッファ等を作成等行う。*/
	virtual void Initialize() = 0;

	/**
	* Light定数バッファをGPUにセットする処理を書き込む。
	* 基本的には Renderer の描画前に呼び出される。
	*/
	virtual void Update(ICamera* mainCamera) = 0;

public:

	void Clear() noexcept;

	void AddLight(ILight* light) noexcept;
	void RemoveLight(ILight* light) noexcept;

	void SetAmbient(const Math::Vector4& color) noexcept;

protected:

	// * Sceneにセットされている全てのLightオブジェクト
	Vector<ILight*> m_lights;

	// * 環境光カラー
	Math::Vector4 m_ambientLight;
};