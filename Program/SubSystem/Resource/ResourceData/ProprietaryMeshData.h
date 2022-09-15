/**
* @file    ProprietaryMeshData.h
* @brief
*
* @date	   2022/09/06 2022年度初版
*/
#pragma once


#include "ProprietaryData.h"
#include "SubSystem/Renderer/Geometry/Vertex.h"

struct aiMesh;
struct PMDMesh;
struct PMDMaterial;

struct ProprietaryMeshData : public ProprietaryData
{
	COMPLETED_DEVELOPMENT()
public:

	// * PBR 用頂点データで作成
	Vector<VertexBump3D> m_vertices;
	Vector<uint32_t> m_indices;

	// * メッシュで使用するマテリアル名
	String m_materialPath;

public:

	void Serialized(FileStream* file) const override;
	void Deserialization(FileStream* file) override;

	/** assimp データから独自データに変換、テクスチャ生成で使用 */
	static ProprietaryMeshData ConvertProprietaryData(aiMesh* mesh) noexcept;
	/** pmd データから独自データに変換、テクスチャ生成で使用 */
	static ProprietaryMeshData ConvertProprietaryData(PMDMesh* mesh) noexcept;

	/** 独自データとして保存される出力先ファイルパスに変換 */
	static String ConvertProprietaryPath(StringView filePath) noexcept;
};