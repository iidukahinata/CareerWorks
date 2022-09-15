/**
* @file		ModelImporter.h
* @brief
*
* @date		2022/08/09 2022�N�x����
*/
#pragma once


struct aiNode;
struct aiMesh;
struct aiMaterial;
struct aiScene;
struct PMDMesh;
struct PMDMaterial;

class ResourceManager;

class ModelImporter
{
	COMPLETED_DEVELOPMENT()
	COPY_PROHIBITED(ModelImporter)
public:

	ModelImporter(ResourceManager* resourceManager);

	/**
	* �w��t�@�C���p�X�̃f�[�^��Ǝ��e�N�X�`���f�[�^�ɕϊ����쐬
	* �����ɐ�������΁A�w�胊�\�[�X�t�@�C�����ɒǉ������B
	*/
	bool CreateModelData(StringView filePath) noexcept;

private:

	bool ImportFile(StringView filePath) noexcept;

	/** PMD�p���f���ǂݍ��݊֐� */
	bool LoadPMDFile(StringView filePath) noexcept;

	void CreateResourceData(StringView filePath) noexcept;

	void Release() noexcept;

	void ProcessNode(aiNode* node);

	String LoadMesh(aiMesh* mesh);
	String LoadMaterial(aiMaterial* material);
	Vector<String> LoadTextures(aiMaterial* material, bool isCreate);

	String LoadMesh(PMDMesh* mesh, StringView path);
	String LoadMaterial(PMDMaterial* material, StringView path);
	String LoadTexture(PMDMaterial* material, bool isCreate);

private:

	const aiScene* m_aiScene = nullptr;

	Vector<String> m_meshPaths;
	Unordered_Map<String, String> m_materialPaths;
	Unordered_Map<String, Vector<String>> m_texturePaths;

	ResourceManager* m_resourceManager;
};