/**
* @file    LightMap.h
* @brief
*
* @date	   2022/08/02 2022�N�x����
*/
#pragma once


class Light;
class Camera;

/**
* ���N���X�ƕ��������邽�߂Ɋe�C���^�[�t�F�C�X�֐������� Ligth �Ǘ����ۃN���X�Ƃ��Ď����ύX�����B
*/
class LightMap
{
	COMPLETED_DEVELOPMENT()
	SUPER_CLASS(LightMap)
public:

	virtual ~LightMap();

	/** �e Light �萔�o�b�t�@�����쐬���s���B*/
	virtual void Initialize() {}

	/**
	* Light�萔�o�b�t�@��GPU�ɃZ�b�g���鏈�����������ށB
	* ��{�I�ɂ� Renderer �̕`��O�ɌĂяo�����B
	*/
	virtual void Update(Camera* mainCamera) {}

	void Clear() noexcept;

	void AddLight(Light* light) noexcept;
	void RemoveLight(Light* light) noexcept;

	void SetAmbient(const Math::Vector4& color) noexcept;

protected:

	// * Scene�ɃZ�b�g����Ă���S�Ă�Light�I�u�W�F�N�g
	Vector<Light*> m_lights;

	// * �����J���[
	Math::Vector4 m_ambientLight;
};