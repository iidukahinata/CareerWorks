#pragma once


struct PMDHeader
{
	float m_version;
	char m_model_name[20];
	char m_comment[256];
};

#pragma pack(1)
struct PMDVertex
{
	Math::Vector3 m_pos;
	Math::Vector3 m_normal;
	Math::Vector2 m_uv;
	uint16_t m_bone_num[2];
	uint8_t m_bone_weight;
	uint8_t m_edge_flag;
};
#pragma pack()

#pragma pack(1)
struct PMDMaterial
{
	Math::Vector3 m_diffuse;
	float m_alpha;
	float m_specularity;
	Math::Vector3 m_specular;
	Math::Vector3 m_ambient;
	uint8_t m_toonIdx;
	uint8_t m_edgeFlg;
	uint32_t m_indicesNum;
	char m_texFilePath[20];
};
#pragma pack()

#pragma pack(1)
struct PMDMesh
{
	Vector<PMDVertex> m_vertices;
	Vector<uint16_t> m_indices;
	PMDMaterial m_material;
};
#pragma pack()