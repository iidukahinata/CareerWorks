/**
* @file    D3D11Texture.cpp
* @brief
*
* @date	   2022/07/14 2022”N“x‰”Å
*/


#include "D3D11Texture.h"

bool D3D11Texture::Create(
	const D3D11_TEXTURE1D_DESC* desc,
	const D3D11_SUBRESOURCE_DATA* subresource /* = nullptr */
) noexcept
{
	ID3D11Texture1D* texture;
	auto hr = GetDevice()->CreateTexture1D(desc, nullptr, &texture);
	if (FAILED(hr)) {
		LOG_ERROR("ShaderResourceView ‚Ì¶¬‚ÉŽ¸”s‚µ‚Ü‚µ‚½B");
		return false;
	}

	hr = GetDevice()->CreateShaderResourceView(texture, nullptr, m_shaderResourceView.ReleaseAndGetAddressOf());
	if (FAILED(hr)) {
		LOG_ERROR("ShaderResourceView ‚Ì¶¬‚ÉŽ¸”s‚µ‚Ü‚µ‚½B");
		return false;
	}

	m_resource = texture;
	return true;
}

bool D3D11Texture::Create(
	const D3D11_TEXTURE2D_DESC* desc,
	const D3D11_SUBRESOURCE_DATA* subresource /* = nullptr */
) noexcept
{
	ID3D11Texture2D* texture;
	auto hr = GetDevice()->CreateTexture2D(desc, nullptr, &texture);
	if (FAILED(hr)) {
		LOG_ERROR("ShaderResourceView ‚Ì¶¬‚ÉŽ¸”s‚µ‚Ü‚µ‚½B");
		return false;
	}

	hr = GetDevice()->CreateShaderResourceView(texture, nullptr, m_shaderResourceView.ReleaseAndGetAddressOf());
	if (FAILED(hr)) {
		LOG_ERROR("ShaderResourceView ‚Ì¶¬‚ÉŽ¸”s‚µ‚Ü‚µ‚½B");
		return false;
	}

	m_resource = texture;
	return true;
}

bool D3D11Texture::Create(
	const D3D11_TEXTURE3D_DESC* desc,
	const D3D11_SUBRESOURCE_DATA* subresource /* = nullptr */
) noexcept
{
	ID3D11Texture3D* texture;
	auto hr = GetDevice()->CreateTexture3D(desc, nullptr, &texture);
	if (FAILED(hr)) {
		LOG_ERROR("ShaderResourceView ‚Ì¶¬‚ÉŽ¸”s‚µ‚Ü‚µ‚½B");
		return false;
	}

	hr = GetDevice()->CreateShaderResourceView(texture, nullptr, m_shaderResourceView.ReleaseAndGetAddressOf());
	if (FAILED(hr)) {
		LOG_ERROR("ShaderResourceView ‚Ì¶¬‚ÉŽ¸”s‚µ‚Ü‚µ‚½B");
		return false;
	}

	m_resource = texture;
	return true;
}

bool D3D11Texture::Create(const DirectX::Image* images, size_t imageSize, const DirectX::TexMetadata& meta)
{
	ASSERT(images, "ˆø”’l image‚É’l‚ð‚¢‚ê‚Ä‚­‚¾‚³‚¢B");

	auto hr = DirectX::CreateShaderResourceView(
		GetDevice(),
		images, imageSize, meta,
		m_shaderResourceView.ReleaseAndGetAddressOf());

	if (FAILED(hr)) {
		LOG_ERROR("ShaderResourceView ‚Ì¶¬‚ÉŽ¸”s‚µ‚Ü‚µ‚½B");
		return false;
	}
	return true;
}

void D3D11Texture::PSSet(UINT slot /* = 0 */) const noexcept
{
	GetContext()->PSSetShaderResources(slot, 1, m_shaderResourceView.GetAddressOf());
}

void D3D11Texture::SetShaderResourceView(ID3D11ShaderResourceView* shaderResourceView) noexcept
{
	m_shaderResourceView.Reset();
	m_shaderResourceView = shaderResourceView;
}

void D3D11Texture::SetResource(ID3D11Resource* resource) noexcept
{
	m_resource.Reset();
	m_resource = resource;
}

ID3D11Resource* D3D11Texture::GetResource() const noexcept
{
	return m_resource.Get();
}

ID3D11Resource** D3D11Texture::GetAddressOfResource() noexcept
{
	return m_resource.GetAddressOf();
}

ID3D11ShaderResourceView* D3D11Texture::GetShaderResourceView() const noexcept
{
	return m_shaderResourceView.Get();
}

ID3D11ShaderResourceView** D3D11Texture::GetAddressOfShaderResourceView() noexcept
{
	return m_shaderResourceView.GetAddressOf();
}