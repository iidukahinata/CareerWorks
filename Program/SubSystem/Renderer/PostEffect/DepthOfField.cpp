#include "Program/Subsystem/Core/Common/pch.h"
#include "DepthOfField.h"

void DepthOfField::Serialized(FileStream* file) const
{
}

void DepthOfField::Deserialized(FileStream* file)
{
}

void DepthOfField::Initialize()
{

}

void DepthOfField::Render()
{

}

D3D12RenderTexture& DepthOfField::GetTexture()
{
    return m_renderTexture;
}