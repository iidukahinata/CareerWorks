/**
* @file    D3D11DeviceChild.h
* @brief
*
* @date	   2022/06/25 2022年度初版
*/
#pragma once


#include "D3D11GrahicsDevice.h"

/**
* D3D11 各クラスの抽象クラス
* GraphicsDevive クラスと 各派先クラスを分離させるために作成。
*/
class D3D11DeviceChild
{
public:

	virtual ~D3D11DeviceChild() {}
	
	ID3D11Device* GetDevice() const
	{
		return D3D11GraphicsDevice::Get().GetDevice();
	}

	ID3D11DeviceContext* GetContext() const
	{
	 	return D3D11GraphicsDevice::Get().GetContext();
	}
};