/**
* @file    Model.h
* @brief
*
* @date	   2022/11/28 2022�N�x����
*/
#pragma once


#include "../IResource.h"
#include "SubSystem/Resource/ResourceData/ProprietaryModelData.h"

class Mesh;

class Model : public IResource
{
	COMPLETED_DEVELOPMENT()
	SUB_CLASS(Model, IResource)
public:

	bool Load(StringView path) override;

	/** ���Editer��Ŏg�p����� asset ��Ǝ��f�[�^�̍X�V���� */
	void Update() override;

	/** �p�����[�^�[ */
	void AddMesh(Mesh* mesh) noexcept;
	Mesh* GetMesh(uint32_t index) noexcept;
	const Vector<Mesh*>& GetAllMeshes() noexcept;

private:

	void UpdateProprietaryDataFile() noexcept;
	void UpdateResourceDataFile() noexcept;

private:

	ProprietaryModelData m_modelData;

	Vector<Mesh*> m_meshes;
};