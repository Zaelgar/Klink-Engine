#include <Graphics/Inc/Graphics.h>
#include <assimp/Importer.hpp>		// C++ importer interface
#include <assimp/scene.h>			// Output data structure
#include <assimp/postprocess.h>		// Post processing flags
#include <cstdio>
#include <fstream>

#include "Graphics/Inc/MeshIO.h"

using namespace Klink::Graphics;

const bool test = true;
//std::string input = "../../Assets/Models/aliensoldier.fbx";
//std::string output = "../../Assets/Models/Imported/aliensoldier.klinkmodel";

std::string input = "../../Assets/Models/Zombie_Idle_30.fbx";
std::string output = "../../Assets/Models/Imported/Zombie_Idle_30.klinkmodel";

using BoneVector = std::vector<Klink::Graphics::Bone>;
using BoneIndexLookup = std::map<std::string, int>;	// Used to lookup by name
std::vector<std::unique_ptr<AnimationClip>> animationClips;


struct Arguments
{
	const char* inputFileName = nullptr;
	const char* outputFileName = nullptr;
};

std::optional<Arguments> ParseArgs(int argc, char* argv[])
{
	if (test)
	{
		Arguments args;
		args.inputFileName = input.c_str();
		args.outputFileName = output.c_str();
		return args;
	}

	// We need at least 3 arguments.
	if (argc < 3)
		return std::nullopt;

	Arguments args;
	args.inputFileName = argv[argc - 2];
	args.outputFileName = argv[argc - 1];
	return args;
}

void PrintUsage()
{
	printf(
		"== ModelImporter Help ==\n"
		"\n"
		"Usage:\n"
		"    ModelImporter.exe [Options] <InputFile> <OutputFile>\n"
		"\n"
		"Options:\n"
		"    <none>\n"
		"\n"
	);
}

inline Klink::JMath::Vector3 Convert(const aiVector3D& v)
{
	return { v.x, v.y, v.z };
}

inline Klink::JMath::Quaternion Convert(const aiQuaternion& q)
{
	return { q.x, q.y, q.z, q.w };
}

inline Klink::JMath::Matrix4 Convert(const aiMatrix4x4& m)
{
	Klink::JMath::Matrix4 mat = *(reinterpret_cast<const Klink::JMath::Matrix4*> (&m));
	return Klink::JMath::Matrix4::Transpose(mat);
}

// Check if input bone already exists in skeleton. If it does, return the index. Otherwise, add it to the skeleton.
int TryAddBone(const aiBone* inputBone, Klink::Graphics::Skeleton& skeleton, BoneIndexLookup& boneIndexLookup)
{
	std::string name = inputBone->mName.C_Str();
	ASSERT(!name.empty(), "ERROR: [ModelImporter] input bone has no name!");

	auto iter = boneIndexLookup.find(name);
	if (iter != boneIndexLookup.end())
		return iter->second;

	// Add a new bone to skeleton
	auto& newBone = skeleton.bones.emplace_back(std::make_unique<Klink::Graphics::Bone>());
	newBone->name = std::move(name);
	newBone->index = static_cast<int>(skeleton.bones.size()) - 1;
	newBone->offsetTransform = Convert(inputBone->mOffsetMatrix);

	// Cache the bone index
	boneIndexLookup.emplace(newBone->name, newBone->index);
	return newBone->index;
}


// Recursively walk through the aiScene tree and add/link bones as we find them to our skeleton
Klink::Graphics::Bone* BuildSkeleton(const aiNode& sceneNode, Klink::Graphics::Bone* parent, Klink::Graphics::Skeleton& skeleton, BoneIndexLookup& boneIndexLookup)
{
	Klink::Graphics::Bone* bone = nullptr;

	std::string name = sceneNode.mName.C_Str();
	auto iter = boneIndexLookup.find(name);
	if (iter != boneIndexLookup.end())
	{
		bone = skeleton.bones[iter->second].get();
	}
	else
	{
		bone = skeleton.bones.emplace_back(std::make_unique<Klink::Graphics::Bone>()).get();
		bone->index = static_cast<int>(skeleton.bones.size()) - 1;
		bone->offsetTransform = Klink::JMath::Matrix4::Identity();
		if (name.empty())
			bone->name = "NoName" + std::to_string(bone->index);
		else
			bone->name = std::move(name);

		boneIndexLookup.emplace(bone->name, bone->index);
	}

	bone->parent = parent;
	
	if(parent)
		bone->parentIndex = parent->index;

	bone->toParentTransform = Convert(sceneNode.mTransformation);

	bone->children.reserve(sceneNode.mNumChildren);
	for (uint32_t i = 0; i < sceneNode.mNumChildren; ++i)
	{
		Klink::Graphics::Bone* child = BuildSkeleton(*sceneNode.mChildren[i], bone, skeleton, boneIndexLookup);
		bone->children.push_back(child);
		bone->childIndices.push_back(child->index);
	}

	return bone; // TODO - Is this right?
}

void SaveMeshes(const Arguments& args, const Klink::Graphics::Model& model)
{
	std::fstream* file = new std::fstream;
	file->open(args.outputFileName, ios::out);

	*file << "MeshCount: " << static_cast<uint32_t>(model.meshes.size()) << endl;

	for (const auto& meshIndex : model.meshes)
	{
		BoneMesh mesh = meshIndex.mesh;

		*file << meshIndex.materialIndex << endl;

		*file << "VertexCount: " << mesh.mVertices.size() << endl;
		for (int index = 0; index < mesh.mVertices.size(); ++index)
		{
			BoneVertex v = mesh.mVertices[index];

			*file << v.position.x << " " << v.position.y << " " << v.position.z << " "
				<< v.normal.x << " " << v.normal.y << " " << v.normal.z << " "
				<< v.tangent.x << " " << v.tangent.y << " " << v.tangent.z << " "
				<< v.UV.y << " " << v.UV.x << endl;
		}

		*file << "IndexCount: " << mesh.mIndices.size() << endl;
		for (int index = 0; index < mesh.mIndices.size();)
		{
			*file << mesh.mIndices[index++] << " " << mesh.mIndices[index++] << " " << mesh.mIndices[index++] << endl;
		}
	}

	file->close();
}

// HACK
void SaveMaterials(const Arguments& args, const Klink::Graphics::Model& model)
{
	std::filesystem::path path = args.outputFileName;
	path.replace_extension("materials");
	
	std::fstream* file = new std::fstream;
	file->open(path, ios::out);

	printf("\nOutputting %i Materials to file...", model.materials.size());

	*file << model.materials.size() << endl;
	for (auto mat : model.materials)
	{
		*file << mat.diffuseMapName << " " << mat.normalMapName << " " << mat.specularMapName << " " << mat.glossinessMapName << endl;
	}

	file->close();
}

void SaveSkeleton(const Arguments& args, const Klink::Graphics::Skeleton& skeleton)
{
	std::filesystem::path path = args.outputFileName;
	path.replace_extension("skeleton");

	std::fstream* file = new std::fstream;
	file->open(path, ios::out);

	printf("\nOutputting bones to file...");

	*file << skeleton.bones.size() << endl;

	for (auto& b : skeleton.bones)
	{
		Bone* bone = b.get();

		*file << bone->name << " " << bone->index << " " << bone->parentIndex << " " << bone->childIndices.size() << " ";
		for (auto child : bone->childIndices)
		{
			*file << child << " ";
		}

		*file
			<< bone->toParentTransform._11 << " " << bone->toParentTransform._12 << " " << bone->toParentTransform._13 << " " << bone->toParentTransform._14 << " "
			<< bone->toParentTransform._21 << " " << bone->toParentTransform._22 << " " << bone->toParentTransform._23 << " " << bone->toParentTransform._24 << " "
			<< bone->toParentTransform._31 << " " << bone->toParentTransform._32 << " " << bone->toParentTransform._33 << " " << bone->toParentTransform._34 << " "
			<< bone->toParentTransform._41 << " " << bone->toParentTransform._42 << " " << bone->toParentTransform._43 << " " << bone->toParentTransform._44 << " ";

		*file
			<< bone->offsetTransform._11 << " " << bone->offsetTransform._12 << " " << bone->offsetTransform._13 << " " << bone->offsetTransform._14 << " "
			<< bone->offsetTransform._21 << " " << bone->offsetTransform._22 << " " << bone->offsetTransform._23 << " " << bone->offsetTransform._24 << " "
			<< bone->offsetTransform._31 << " " << bone->offsetTransform._32 << " " << bone->offsetTransform._33 << " " << bone->offsetTransform._34 << " "
			<< bone->offsetTransform._41 << " " << bone->offsetTransform._42 << " " << bone->offsetTransform._43 << " " << bone->offsetTransform._44 << endl;
	}

	file->close();
}

void SaveAnimationClips(const Arguments& args, const std::vector<std::unique_ptr<AnimationClip>>& animationClips)
{
	std::filesystem::path path = args.outputFileName;
	path.replace_extension("animationclips");

	std::fstream*file = new std::fstream;
	file->open(path, ios::out);

	*file << animationClips.size() << endl;

	for (int clipIndex = 0; clipIndex < animationClips.size(); ++clipIndex)
	{
		auto animClip = animationClips[clipIndex].get();

		*file << animClip->name << " " << animClip->duration << " " << animClip->ticksPerSecond << " " << animClip->boneAnimations.size() << endl;

		for (int boneAnimIndex = 0; boneAnimIndex < animClip->boneAnimations.size(); ++boneAnimIndex)
		{
			if (animClip->boneAnimations[boneAnimIndex])
			{
				auto boneAnim = animClip->boneAnimations[boneAnimIndex].get();

				*file << boneAnim->boneName << " " << boneAnim->animation.mPositionKeyframes.size() << " " << boneAnim->animation.mRotationKeyframes.size() << " " << boneAnim->animation.mScaleKeyframes.size() << endl;

				for (auto posKey : boneAnim->animation.mPositionKeyframes)
				{
					*file << posKey.time << " " << posKey.value.x << " " << posKey.value.y << " " << posKey.value.z << endl;
				}
				for (auto rotKey : boneAnim->animation.mRotationKeyframes)
				{
					*file << rotKey.time << " " << rotKey.value.x << " " << rotKey.value.y << " " << rotKey.value.z << " " << rotKey.value.w << endl;
				}
				for (auto scaleKey : boneAnim->animation.mScaleKeyframes)
				{
					*file << scaleKey.time << " " << scaleKey.value.x << " " << scaleKey.value.y << " " << scaleKey.value.z << endl;
				}
			}
			else
			{
				*file << "0" << endl;
			}
		}
	}

	file->close();
}

int main(int argc, char* argv[])
{
	const auto argsOpt = ParseArgs(argc, argv);
	if (!argsOpt.has_value())
	{
		PrintUsage();
		return -1;
	}

	const auto& args = argsOpt.value();

	// Create an instance of the importer class to do the parsing for us.
	Assimp::Importer importer;

	// Try to import the model into a scene.
	const aiScene* scene = importer.ReadFile(args.inputFileName,
		aiProcessPreset_TargetRealtime_Quality |
		aiProcess_ConvertToLeftHanded);
	if (scene == nullptr)
	{
		printf("Error: %s\n", importer.GetErrorString());
		return -1;
	}

	// scene
	// +- mesh[_][_][_][_][_][_]...
	// +- material[_][_][_][_][_]...
	// +- animation[_][_][_][_][_]...
	// +- mRootNode
	//     +-Node
	//       +- Node
	//       +- Node
	//          +- Node ...
	//     +-Node
	//       +- Node ...
	//     +-Node
	//     ...

	Klink::Graphics::Model model;
	BoneIndexLookup boneIndexLookup;

	// Look for mesh data.
	if (scene->HasMeshes())
	{
		printf("Reading mesh data...\n");

		const uint32_t numMeshes = scene->mNumMeshes;
		const uint32_t numMaterials = scene->mNumMaterials;
		model.meshes.reserve(numMeshes);
		model.materials.reserve(numMaterials);

		for (uint32_t meshIndex = 0; meshIndex < numMeshes; ++meshIndex)
		{
			Model::MeshData currentMeshData;

			const aiMesh* inputMesh = scene->mMeshes[meshIndex];
			const uint32_t numVertices = inputMesh->mNumVertices;
			const uint32_t numFaces = inputMesh->mNumFaces;
			const uint32_t numIndices = numFaces * 3;

			printf("Reading material index...\n");

			currentMeshData.materialIndex = inputMesh->mMaterialIndex;

			printf("Reading vertices...\n");

			currentMeshData.mesh.mVertices.reserve(numVertices);

			const aiVector3D* positions = inputMesh->mVertices;
			const aiVector3D* normals = inputMesh->mNormals;
			const aiVector3D* tangents = inputMesh->mTangents;
			const aiVector3D* texCoords = inputMesh->HasTextureCoords(0) ? inputMesh->mTextureCoords[0] : nullptr;
			for (uint32_t i = 0; i < numVertices; ++i)
			{
				Klink::Graphics::BoneVertex vertex;
				vertex.position = Convert(positions[i]);
				vertex.normal = Convert(normals[i]);
				vertex.tangent = Convert(tangents[i]);
				vertex.UV = texCoords ? Klink::JMath::Vector2{ texCoords[i].y, texCoords[i].x } : 0.0f;

				currentMeshData.mesh.mVertices.push_back(vertex);
			}

			printf("Reading indices...\n");

			currentMeshData.mesh.mIndices.reserve(numIndices);

			const aiFace* faces = inputMesh->mFaces;
			for (uint32_t i = 0; i < numFaces; ++i)
			{
				currentMeshData.mesh.mIndices.push_back(faces[i].mIndices[0]);
				currentMeshData.mesh.mIndices.push_back(faces[i].mIndices[1]);
				currentMeshData.mesh.mIndices.push_back(faces[i].mIndices[2]);
			}

			// Added for Bones
			if (inputMesh->HasBones())
			{
				printf("Reading bones...\n");

				// Track how many weights have we added to each vertex so far
				std::vector<int> numWeights(currentMeshData.mesh.mVertices.size(), 0);

				for (uint32_t meshBoneIndex = 0; meshBoneIndex < inputMesh->mNumBones; ++meshBoneIndex)
				{
					aiBone* inputBone = inputMesh->mBones[meshBoneIndex];
					int boneIndex = TryAddBone(inputBone, model.skeleton, boneIndexLookup);

					for (int32_t weightIndex = 0; weightIndex < inputBone->mNumWeights; ++weightIndex)
					{
						const aiVertexWeight& weight = inputBone->mWeights[weightIndex];
						auto& vertex = currentMeshData.mesh.mVertices[weight.mVertexId];
						auto& count = numWeights[weight.mVertexId];

						// For performance, our vertices can only hold 4 weights per vertex
						if (count < 4)
						{
							vertex.boneIndices[count] = meshBoneIndex;
							vertex.boneWeights[count] = weight.mWeight;
							++count;
						}
					}
				}
			}

			

			//currentMeshData.meshBuffer.Initialize(currentMeshData.mesh);

			model.meshes.emplace_back(currentMeshData);
		}
	}

	if (!model.skeleton.bones.empty())
	{
		printf("Building Skeleton...\n");
		BuildSkeleton(*scene->mRootNode, nullptr, model.skeleton, boneIndexLookup);
	}

	// Look for material data
	if (scene->HasMaterials())
	{
		printf("Reading materials... \n");

		for (uint32_t i = 0; i < scene->mNumMaterials; ++i)
		{
			Model::MaterialData currentMaterialData;

			const aiMaterial* inputMaterial = scene->mMaterials[i];

			// For now we are only interested in diffuse maps
			const uint32_t diffuseTextureCount = inputMaterial->GetTextureCount(aiTextureType_DIFFUSE);
			if (diffuseTextureCount > 0)
			{
				aiString texturePath;
				inputMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texturePath);
				if (inputMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == aiReturn_SUCCESS)
				{
					const aiTexture* texture;
					texture = scene->GetEmbeddedTexture(texturePath.C_Str());

					if (texture)
					{
						std::string fileName = args.outputFileName;
						fileName.erase(fileName.length() - 11);							// remove the ".klinkmodel" extension
						fileName += ("_mat_" + std::to_string(i));						// add _mat_# for material
						fileName += "_diffuse_" + std::to_string(diffuseTextureCount - 1);		// add _diffuse_# for diffuse per material

						ASSERT(scene->HasTextures(), "ERROR: [ModelImporter] No embedded texture found!");

						ASSERT(diffuseTextureCount < (int)scene->mNumTextures, "ERROR: [ModelImporter] Invalid texture index!");

						ASSERT(texture->mHeight == 0, "ERROR: [ModelImporter] Uncompressed texture found!");

						if (texture->CheckFormat("jpg"))
							fileName += ".jpg";
						else if (texture->CheckFormat("png"))
							fileName += ".png";
						else
							ASSERT(0, "ERROR: Unrecognized texture format!\n");

						printf("Extracting embedded texture %s \n", texturePath.C_Str());

						std::string fullFileName = args.outputFileName;
						fullFileName = fullFileName.substr(0, fullFileName.rfind('/') -8);
						fullFileName += fileName.substr(fileName.rfind("Models/") + 7, fileName.size());

						fileName = fileName.substr(fileName.rfind("Models/"), fileName.size());

						currentMaterialData.diffuseMapName = fileName;

						FILE* file = nullptr;
						fopen_s(&file, fullFileName.c_str(), "wb");
						size_t written = fwrite(texture->pcData, 1, texture->mWidth, file);
						ASSERT(written == texture->mWidth, "ERROR: Failed to extract embedded texture!");
						fclose(file);
					}
					else
					{
						std::filesystem::path filePath = texturePath.C_Str();
						std::string fileName = filePath.filename().u8string();

						currentMaterialData.diffuseMapName = fileName;

						printf("Adding texture %s\n", fileName.c_str());
					}
				}
			}

			const uint32_t normalTextureCount = inputMaterial->GetTextureCount(aiTextureType_NORMALS);
			if (normalTextureCount > 0)
			{
				aiString normalTexturePath;
				inputMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_NORMALS, 0), normalTexturePath);
				if (inputMaterial->GetTexture(aiTextureType_NORMALS, 0, &normalTexturePath) == aiReturn_SUCCESS)
				{
					const aiTexture* texture;
					texture = scene->GetEmbeddedTexture(normalTexturePath.C_Str());

					if (texture)
					{
						std::string fileName = args.inputFileName;
						//fileName.erase(fileName.length() - 4);							// remove the ".fbx" extension
						fileName.erase(fileName.length() - 11);							// remove the ".klinkmodel" extension
						fileName += ("_mat_" + std::to_string(i));						// add _mat_# for material
						fileName += "_normal_" + std::to_string(normalTextureCount - 1);		// add _normal_# for normal per material

						ASSERT(scene->HasTextures(), "ERROR: [ModelImporter] No embedded texture found!");

						ASSERT(normalTextureCount < (int)scene->mNumTextures, "ERROR: [ModelImporter] Invalid texture index!");

						ASSERT(texture->mHeight == 0, "ERROR: [ModelImporter] Uncompressed texture found!");

						if (texture->CheckFormat("jpg"))
							fileName += ".jpg";
						else if (texture->CheckFormat("png"))
							fileName += ".png";
						else
							ASSERT(0, "ERROR: Unrecognized texture format!\n");

						printf("Extracting embedded texture %s \n", normalTexturePath.C_Str());

						std::string fullFileName = args.outputFileName;
						//fullFileName = fullFileName.substr(0, fullFileName.rfind('/') + 1);
						fullFileName = fullFileName.substr(0, fullFileName.rfind('/') - 8);
						fullFileName += fileName.substr(fileName.rfind("Models/") + 7, fileName.size());

						fileName = fileName.substr(fileName.rfind("Models/"), fileName.size());
						currentMaterialData.normalMapName = fileName;

						FILE* file = nullptr;
						fopen_s(&file, fullFileName.c_str(), "wb");
						size_t written = fwrite(texture->pcData, 1, texture->mWidth, file);
						ASSERT(written == texture->mWidth, "ERROR: Failed to extract embedded texture!");
						fclose(file);
					}
					else
					{
						std::filesystem::path filePath = normalTexturePath.C_Str();
						std::string fileName = filePath.filename().u8string();

						currentMaterialData.normalMapName = fileName;

						printf("Adding texture %s\n", fileName.c_str());
					}
				}
			}

			const uint32_t specularTextureCount = inputMaterial->GetTextureCount(aiTextureType_SPECULAR);
			if (specularTextureCount > 0)
			{
				aiString specularTexturePath;
				inputMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_SPECULAR, 0), specularTexturePath);
				if (inputMaterial->GetTexture(aiTextureType_SPECULAR, 0, &specularTexturePath) == aiReturn_SUCCESS)
				{
					const aiTexture* texture;
					texture = scene->GetEmbeddedTexture(specularTexturePath.C_Str());

					if (texture)
					{
						std::string fileName = args.inputFileName;
						//fileName.erase(fileName.length() - 4);							// remove the ".fbx" extension
						fileName.erase(fileName.length() - 11);							// remove the ".klinkmodel" extension
						fileName += ("_mat_" + std::to_string(i));						// add _mat_# for material
						fileName += "_specular_" + std::to_string(specularTextureCount - 1);		// add _normal_# for normal per material

						ASSERT(scene->HasTextures(), "ERROR: [ModelImporter] No embedded texture found!");

						ASSERT(specularTextureCount < (int)scene->mNumTextures, "ERROR: [ModelImporter] Invalid texture index!");

						ASSERT(texture->mHeight == 0, "ERROR: [ModelImporter] Uncompressed texture found!");

						if (texture->CheckFormat("jpg"))
							fileName += ".jpg";
						else if (texture->CheckFormat("png"))
							fileName += ".png";
						else
							ASSERT(0, "ERROR: Unrecognized texture format!\n");

						printf("Extracting embedded texture %s \n", specularTexturePath.C_Str());

						std::string fullFileName = args.outputFileName;
						//fullFileName = fullFileName.substr(0, fullFileName.rfind('/') + 1);
						fullFileName = fullFileName.substr(0, fullFileName.rfind('/') - 8);
						fullFileName += fileName.substr(fileName.rfind("Models/") + 7, fileName.size());

						fileName = fileName.substr(fileName.rfind("Models/"), fileName.size());

						currentMaterialData.specularMapName = fileName;

						FILE* file = nullptr;
						fopen_s(&file, fullFileName.c_str(), "wb");
						size_t written = fwrite(texture->pcData, 1, texture->mWidth, file);
						ASSERT(written == texture->mWidth, "ERROR: Failed to extract embedded texture!");
						fclose(file);
					}
					else
					{
						std::filesystem::path filePath = specularTexturePath.C_Str();
						std::string fileName = filePath.filename().u8string();

						currentMaterialData.specularMapName = fileName;

						printf("Adding texture %s\n", fileName.c_str());
					}
				}
			}

			const uint32_t glossinessTextureCount = inputMaterial->GetTextureCount(aiTextureType_SHININESS);
			if (glossinessTextureCount > 0)
			{
				aiString glossinessTexturePath;
				inputMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_SHININESS, 0), glossinessTexturePath);
				if (inputMaterial->GetTexture(aiTextureType_SHININESS, 0, &glossinessTexturePath) == aiReturn_SUCCESS)
				{
					const aiTexture* texture;
					texture = scene->GetEmbeddedTexture(glossinessTexturePath.C_Str());

					if (texture)
					{
						std::string fileName = args.inputFileName;
						//fileName.erase(fileName.length() - 4);							// remove the ".fbx" extension
						fileName.erase(fileName.length() - 11);							// remove the ".klinkmodel" extension
						fileName += ("_mat_" + std::to_string(i));						// add _mat_# for material
						fileName += "_glossiness_" + std::to_string(glossinessTextureCount - 1);		// add _normal_# for normal per material

						ASSERT(scene->HasTextures(), "ERROR: [ModelImporter] No embedded texture found!");

						ASSERT(glossinessTextureCount < (int)scene->mNumTextures, "ERROR: [ModelImporter] Invalid texture index!");

						ASSERT(texture->mHeight == 0, "ERROR: [ModelImporter] Uncompressed texture found!");

						if (texture->CheckFormat("jpg"))
							fileName += ".jpg";
						else if (texture->CheckFormat("png"))
							fileName += ".png";
						else
							ASSERT(0, "ERROR: Unrecognized texture format!\n");

						printf("Extracting embedded texture %s \n", glossinessTexturePath.C_Str());

						std::string fullFileName = args.outputFileName;
						//fullFileName = fullFileName.substr(0, fullFileName.rfind('/') + 1);
						fullFileName = fullFileName.substr(0, fullFileName.rfind('/') - 8);
						fullFileName += fileName.substr(fileName.rfind("Models/") + 7, fileName.size());

						fileName = fileName.substr(fileName.rfind("Models/"), fileName.size());

						currentMaterialData.glossinessMapName = fileName;

						FILE* file = nullptr;
						fopen_s(&file, fullFileName.c_str(), "wb");
						size_t written = fwrite(texture->pcData, 1, texture->mWidth, file);
						ASSERT(written == texture->mWidth, "ERROR: Failed to extract embedded texture!");
						fclose(file);
					}
					else
					{
						std::filesystem::path filePath = glossinessTexturePath.C_Str();
						std::string fileName = filePath.filename().u8string();

						currentMaterialData.glossinessMapName = fileName;

						printf("Adding texture %s\n", fileName.c_str());
					}
				}
			}

			// emplace back once all data collected.
			model.materials.emplace_back(currentMaterialData);
		}
	}

	if (scene->HasAnimations())
	{
		printf("Reading Animations...\n");

		for (uint32_t animIndex = 0; animIndex < scene->mNumAnimations; ++animIndex)
		{
			const aiAnimation* inputAnim = scene->mAnimations[animIndex];
			auto& animClip = animationClips.emplace_back(std::make_unique<AnimationClip>());

			if (inputAnim->mName.length > 0)
			{
				animClip->name = inputAnim->mName.C_Str();
			}
			else
			{
				animClip->name = "Anim" + std::to_string(animIndex);
			}

			animClip->duration = static_cast<float>(inputAnim->mDuration);
			animClip->ticksPerSecond = static_cast<float>(inputAnim->mTicksPerSecond);

			printf("Reading bone animations for %s...", animClip->name.c_str());

			// Reserve space so we have one animation slot per bone, not that not all bones will have animations so some slots will stay empty.
			// However, keeping them the same size means we can use bone index directly to look up animations.
			animClip->boneAnimations.resize(model.skeleton.bones.size());
			for (uint32_t boneAnimIndex = 0; boneAnimIndex < inputAnim->mNumChannels; ++boneAnimIndex)
			{
				const aiNodeAnim* inputBoneAnim = inputAnim->mChannels[boneAnimIndex];
				int slotIndex = boneIndexLookup[inputBoneAnim->mNodeName.C_Str()];
				auto& boneAnim = animClip->boneAnimations[slotIndex];
				boneAnim = std::make_unique<BoneAnimation>();

				AnimationBuilder builder;
				for (uint32_t keyIndex = 0; keyIndex < inputBoneAnim->mNumPositionKeys; ++keyIndex)
				{
					auto& key = inputBoneAnim->mPositionKeys[keyIndex];
					builder.AddPositionKey(Convert(key.mValue), static_cast<float>(key.mTime));
				}
				for (uint32_t keyIndex = 0; keyIndex < inputBoneAnim->mNumRotationKeys; ++keyIndex)
				{
					auto& key = inputBoneAnim->mRotationKeys[keyIndex];
					builder.AddRotationKey(Convert(key.mValue), static_cast<float>(key.mTime));
				}
				for (uint32_t keyIndex = 0; keyIndex < inputBoneAnim->mNumScalingKeys; ++keyIndex)
				{
					auto& key = inputBoneAnim->mScalingKeys[keyIndex];
					builder.AddScalingKey(Convert(key.mValue), static_cast<float>(key.mTime));
				}

				boneAnim->animation = builder.Get();
				boneAnim->boneName = model.skeleton.bones[slotIndex].get()->name;
				animClip->boneAnimations[slotIndex] = std::move(boneAnim);
			}
		}
	}

	SaveMeshes(args, model);
	SaveMaterials(args, model);
	SaveSkeleton(args, model.skeleton);
	SaveAnimationClips(args, animationClips);

	printf("All done!\n");
	return 0;
}