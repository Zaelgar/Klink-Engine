#include "Precompiled.h"

#include "MeshIO.h"
#include "Graphics/Inc/Model.h"

#include <fstream>

using namespace Klink::Graphics;
using namespace Klink::JMath;

void MeshIO::Write(std::fstream* file, BoneMesh mesh)
{
	*file << "VertexCount: " << mesh.mVertices.size() << std::endl;
	for (int index = 0; index < mesh.mVertices.size(); ++index)
	{
		BoneVertex v = mesh.mVertices[index];

		*file << v.position.x << " " << v.position.y << " " << v.position.z << " "
			<< v.normal.x << " " << v.normal.y << " " << v.normal.z << " "
			<< v.tangent.x << " " << v.tangent.y << " " << v.tangent.z << " "
			<< v.UV.x << " " << v.UV.y << std::endl;
	}

	*file << "IndexCount: " << mesh.mIndices.size() << std::endl;
	for (int index = 0; index < mesh.mIndices.size();)
	{
		*file << mesh.mIndices[index++] << " " << mesh.mIndices[index++] << " " << mesh.mIndices[index++] << std::endl;
	}
}

void MeshIO::ReadInMeshes(std::filesystem::path filePath, Model& model)
{
	std::fstream file;
	std::string currentLine;
	std::string splitString;

	file.open(filePath, std::fstream::in);

	// Read in MeshCount;
	std::getline(file, currentLine);
	// Split the string at the 11th character, right where "MeshCount: " ends. Should just be an integer if Write is correct.
	splitString = currentLine.substr(11, currentLine.size() - 11);

	unsigned int meshCount = stoi(splitString);
	model.meshes.reserve(meshCount);

	for (unsigned int currentMesh = 0; currentMesh < model.meshes.capacity(); ++currentMesh)
	{
		model.meshes.emplace_back();

		file >> model.meshes[currentMesh].materialIndex;
		file.ignore(1);	// ignore last /n character for next getline() to work properly
		// Read in VertexCount for currentMesh
		std::getline(file, currentLine);

		// Split the string at the 13th character, right where "VertexCount: " ends. Should be an int
		splitString = currentLine.substr(13, currentLine.size() - 13);

		unsigned int vertexCount = stoi(splitString);
		model.meshes[currentMesh].mesh.mVertices.reserve(vertexCount);

		// Read in each number as a float
		float x, y, z;
		float nx, ny, nz;
		float tx, ty, tz;
		float u, v;
		for (unsigned int currentVertex = 0; currentVertex < vertexCount; ++currentVertex)
		{
			file >> x >> y >> z >> nx >> ny >> nz >> tx >> ty >> tz >> u >> v;
			model.meshes[currentMesh].mesh.mVertices.emplace_back(BoneVertex{ { x, y, z }, { nx, ny, nz }, { tx, ty, tz }, { u, v }, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f} });
		}

		file.ignore(1);	// ignore last /n character for next getline() to work properly
		std::getline(file, currentLine); // IndexCount: 85374

		// Split the string at the 12th character, right where "IndexCount: " ends. Should be an int
		splitString = currentLine.substr(12, currentLine.size() - 12);

		unsigned int indexCount = stoi(splitString);
		model.meshes[currentMesh].mesh.mIndices.reserve(indexCount);

		unsigned int index1, index2, index3;
		for (unsigned int currentIndex = 0; currentIndex < (indexCount / 3); ++currentIndex)
		{
			file >> index1 >> index2 >> index3;

			model.meshes[currentMesh].mesh.mIndices.emplace_back(index1);
			model.meshes[currentMesh].mesh.mIndices.emplace_back(index2);
			model.meshes[currentMesh].mesh.mIndices.emplace_back(index3);
		}
		std::getline(file, currentLine);

		model.meshes[currentMesh].meshBuffer.Initialize(model.meshes[currentMesh].mesh);
	}

	file.close();
}

void MeshIO::ReadInMaterials(std::filesystem::path filePath, Model& model)
{
	// For all textures and materials
	std::fstream file;

	file.open(filePath, std::fstream::in);

	unsigned int materialCount;
	file >> materialCount;

	model.materials.resize(materialCount);

	for (int materialIndex = 0; materialIndex < materialCount; ++materialIndex)
	{
		file >> model.materials[materialIndex].diffuseMapName >> model.materials[materialIndex].normalMapName >> model.materials[materialIndex].specularMapName >> model.materials[materialIndex].glossinessMapName;

		model.materials[materialIndex].diffuseID = TextureManager::Get()->LoadTexture(model.materials[materialIndex].diffuseMapName);
		model.materials[materialIndex].normalID = TextureManager::Get()->LoadTexture(model.materials[materialIndex].normalMapName);
		model.materials[materialIndex].specularID = TextureManager::Get()->LoadTexture(model.materials[materialIndex].specularMapName);
		model.materials[materialIndex].glossinessID = TextureManager::Get()->LoadTexture(model.materials[materialIndex].glossinessMapName);
	}
}

void MeshIO::ReadInSkeleton(std::filesystem::path filePath, Model& model)
{
	std::fstream file;

	file.open(filePath, std::fstream::in);

	unsigned int boneCount;
	file >> boneCount;
	model.skeleton.bones.reserve(boneCount);

	int rootIndex;

	for (int currentBone = 0; currentBone < boneCount; ++currentBone)
	{
		Bone b;
		int childCount;
		int childIndex;
		file
			>> b.name
			>> b.index
			>> b.parentIndex
			>> childCount;
		if (childCount)
		{
			b.children.reserve(childCount);
			b.childIndices.reserve(childCount);
			for (int i = 0; i < childCount; ++i)
			{
				file >> childIndex;
				b.childIndices.push_back(childIndex);
			}
		}

		// read in toParentTransform
		float _11, _12, _13, _14;
		float _21, _22, _23, _24;
		float _31, _32, _33, _34;
		float _41, _42, _43, _44;
		file
			>> _11 >> _12 >> _13 >> _14
			>> _21 >> _22 >> _23 >> _24
			>> _31 >> _32 >> _33 >> _34
			>> _41 >> _42 >> _43 >> _44;

		b.toParentTransform = Klink::JMath::Matrix4(
			_11, _12, _13, _14,
			_21, _22, _23, _24,
			_31, _32, _33, _34,
			_41, _42, _43, _44);
		if(b.parentIndex == -1)
		{
			rootIndex = b.index;
		}

		// read in offsetTransform
		file
			>> _11 >> _12 >> _13 >> _14
			>> _21 >> _22 >> _23 >> _24
			>> _31 >> _32 >> _33 >> _34
			>> _41 >> _42 >> _43 >> _44;

		b.offsetTransform = Klink::JMath::Matrix4(
			_11, _12, _13, _14,
			_21, _22, _23, _24,
			_31, _32, _33, _34,
			_41, _42, _43, _44);

		model.skeleton.bones.emplace_back(std::make_unique<Bone>(b));
	}

	model.skeleton.root = model.skeleton.bones[rootIndex].get();
	for (int boneIndex = 0; boneIndex < boneCount; boneIndex++)
	{
		for (int childIndex = 0; childIndex < model.skeleton.bones[boneIndex].get()->childIndices.size(); ++childIndex)
		{
			// This line links each bone's children to the bones in the skeleton bone array
			model.skeleton.bones[boneIndex].get()->children.push_back(model.skeleton.bones[model.skeleton.bones[boneIndex].get()->childIndices[childIndex]].get());

			// This line links each bone's parent to the bones in the skeleton bone array
			if (model.skeleton.bones[boneIndex].get()->parentIndex != -1)
				model.skeleton.bones[boneIndex].get()->parent = model.skeleton.bones[model.skeleton.bones[boneIndex].get()->parentIndex].get();
		}
	}

	file.close();
}

void MeshIO::ReadInAnimationClips(std::filesystem::path filePath, Klink::Graphics::AnimationClip& anim)
{
	// HACK - only assumes one animation Clip. Refactor for use with AnimationClip vector/ AnimationSet
	std::fstream file;
	file.open(filePath, std::fstream::in);

	int animClipCount;
	int animClipBoneSlotCount;

	file >> animClipCount;	// resize Animset animclip size once read in is complete
	file >> anim.name >> anim.duration >> anim.ticksPerSecond >> animClipBoneSlotCount;

	anim.boneAnimations.reserve(animClipBoneSlotCount);

	for (int bIndex = 0; bIndex < animClipBoneSlotCount; ++bIndex)
	{
		BoneAnimation boneAnim;
		AnimationBuilder animBuilder;
		std::string check;

		file >> check;
		// If 0, this is an empty slot.
		if (check == "0")
		{
			anim.boneAnimations.emplace_back();
			continue;
		}
		else
		{
			boneAnim.boneName = std::move(check);

			int posKeys;
			int rotKeys;
			int scaleKeys;

			file >> posKeys >> rotKeys >> scaleKeys;

			for (int posIndex = 0; posIndex < posKeys; ++posIndex) // read in each position key (if any, probably at least one)
			{
				float time;
				Vector3 posVector;

				file >> time >> posVector.x >> posVector.y >> posVector.z;

				animBuilder.AddPositionKey(posVector, time);
			}
			for (int rotIndex = 0; rotIndex < rotKeys; ++rotIndex) // read in each rotation key (if any, probably at least one)
			{
				float time;
				Quaternion rotQuat;

				file >> time >> rotQuat.x >> rotQuat.y >> rotQuat.z >> rotQuat.w;

				animBuilder.AddRotationKey(rotQuat, time);
			}
			for (int scaleIndex = 0; scaleIndex < scaleKeys; ++scaleIndex) // read in each position key (if any, probably at least one)
			{
				float time;
				Vector3 scaleVector;

				file >> time >> scaleVector.x >> scaleVector.y >> scaleVector.z;

				animBuilder.AddScalingKey(scaleVector, time);
			}

			boneAnim.animation = animBuilder.Get();

			anim.boneAnimations.emplace_back(std::make_unique<BoneAnimation>(boneAnim));
		}
	}

	file.close();
}