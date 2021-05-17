#ifndef INCLUDED_KLINK_GRAPHICS_MODEL_H
#define INCLUDED_KLINK_GRAPHICS_MODEL_H

#include "Mesh.h"
#include "MeshBuffer.h"
#include "Skeleton.h"
#include "TextureManager.h"
#include "SamplerManager.h"
#include "RasterizerStateManager.h"

namespace Klink::Graphics
{

class Model
{
public:

	void Terminate();

	void Render() const;

public:

	struct MeshData
	{
		BoneMesh mesh;
		uint32_t materialIndex;
		MeshBuffer meshBuffer;
	};

	struct MaterialData
	{
		std::string diffuseMapName;
		std::string normalMapName;
		std::string specularMapName;
		std::string glossinessMapName;
		TextureID diffuseID;
		TextureID normalID;
		TextureID specularID;
		TextureID glossinessID;
	};

	std::vector<MeshData> meshes;
	std::vector<MaterialData> materials;
	Skeleton skeleton;
};

} //  namespace Klink::Graphics

#endif // INCLUDED_KLINK_GRAPHICS_MODEL_H