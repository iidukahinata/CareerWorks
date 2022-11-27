/**
* @file    PostEffect.h
* @brief
*
* @date	   2022/10/29 2022îNìxèâî≈
*/
#pragma once


class IRenderer;
class PostProcessEffect;
class D3D12RenderTexture;

using PostEffectType = ClassTypeData;

class IPostEffect
{
	IN_DEVELOPMENT()
	SUPER_CLASS(IPostEffect)

	friend class PostEffectFactory;
public:

	virtual ~IPostEffect() = default;

	virtual void Serialized(FileStream* file) const = 0;
	virtual void Deserialized(FileStream* file) = 0;

	virtual void Initialize() = 0;

	virtual void Render() = 0;

	virtual D3D12RenderTexture& GetTexture() = 0;

public:

	IRenderer* GetRenderer() const noexcept;

protected:

	PostProcessEffect* m_postProcessEffect = nullptr;
};