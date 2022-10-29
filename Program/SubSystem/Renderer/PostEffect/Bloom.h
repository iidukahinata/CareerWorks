/**
* @file    Bloom.h
* @brief
*
* @date	   2022/10/29 2022îNìxèâî≈
*/
#pragma once


#include "PostEffect.h"

class Bloom : public PostEffect
{
	IN_DEVELOPMENT()
	SUB_CLASS(Bloom)
public:

	void Serialized(FileStream* file) const override;
	void Deserialized(FileStream* file) override;

	void Initialize() override;

	void Render() override;

private:
};