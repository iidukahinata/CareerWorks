/**
* @file    Texture.h
* @brief
*
* @date	   2022/09/04 2022îNìxèâî≈
*/
#pragma once


#include "../IResource.h"
#include "SubSystem/Renderer/GraphicsAPI/D3D12/D3D12Texture.h"

class Texture : public IResource
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(Texture)
public:

	bool Load(StringView path) override;

	void Update() override;

	void* GetData() noexcept;
	void PSSet(UINT slot) noexcept;

private:

	D3D12Texture m_texture;
};