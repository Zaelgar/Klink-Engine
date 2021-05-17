#include "Precompiled.h"

#include "Model.h"

using namespace std;
using namespace Klink::Graphics;

void Model::Terminate()
{
	for (auto mesh : meshes)
	{
		mesh.meshBuffer.Terminate();
	}
}

void Model::Render() const
{
	auto tm = TextureManager::Get();
	auto ss = SamplerManager::Get();

	// bind each and render each
	for (auto mesh : meshes)
	{
		const auto& material = materials[mesh.materialIndex];

		// Bind diffuse
		tm->GetTexture(material.diffuseID)->BindPS(0);

		// Bind normal
		tm->GetTexture(material.normalID)->BindPS(1);

		// Bind specular
		tm->GetTexture(material.specularID)->BindPS(2);

		// Bind Sampler
		ss->GetSampler("LinearWrap")->BindPS();

		mesh.meshBuffer.Render();
	}
}