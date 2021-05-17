#ifndef INCLUDED_KLINK_GRAPHICS_MESHIO_H
#define INCLUDED_KLINK_GRAPHICS_MESHIO_H

#include "Mesh.h"
#include "Model.h"
#include "TextureManager.h"
#include "AnimationClip.h"
#include "AnimationBuilder.h"
#include <fstream>

namespace Klink::Graphics
{

class MeshIO
{
public:

	static void Write(std::fstream*, BoneMesh);

	static void ReadInMeshes(std::filesystem::path filePath, Klink::Graphics::Model& model);
	static void ReadInMaterials(std::filesystem::path filePath, Klink::Graphics::Model& model);
	static void ReadInSkeleton(std::filesystem::path filePath, Klink::Graphics::Model& model);
	static void ReadInAnimationClips(std::filesystem::path filePath, Klink::Graphics::AnimationClip& anim);

};

};

#endif // INCLUDED_KLINK_GRAPHICS_MESHIO_H