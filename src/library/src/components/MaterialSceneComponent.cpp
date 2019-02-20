#include "StdAfx.h"
#include "library/components/MaterialSceneComponent.h"

#include "library/Application.h"

#include "library/Model.h"
#include "library/Mesh.h"

#include "library/materials/Material.h"

#include "library/effect/EffectTechnique.h"
#include "library/effect/EffectPass.h"

namespace library
{
	void MaterialSceneComponent::SetModelName(const std::string& modelName)
	{
		if (m_modelName != modelName)
			m_modelName = modelName;
	}

	void MaterialSceneComponent::Initialize(const Application& app)
	{
		SceneComponent::Initialize(app);

		const auto& material = GetMaterial();
		assert(material.IsInitialized());

		// set default layout from material
		const auto& currentTechnique = material.GetCurrentTechnique();
		const auto& pass = currentTechnique.GetPass(0);
		m_input.layout = material.GetInputLayoutShared(pass);

		// load model from file
		if (!m_modelName.empty())
		{
			Model model(*m_app, m_modelName, true);
			const auto& mesh = model.GetMesh(0);

			m_input.vertices.buffer = material.CreateVertexBuffer(m_app->GetDevice(), mesh);
			m_input.vertices.count = mesh.GetVerticesCount();

			if (mesh.HasIndices())
			{
				m_input.indices = std::make_unique<BufferData>();
				m_input.indices->buffer = mesh.CreateIndexBuffer();
				m_input.indices->count = mesh.GetIndicesCount();
			}
		}
	}

	void MaterialSceneComponent::Draw_SetData()
	{
		auto& material = GetMaterial();
		auto& currentTechnique = material.GetCurrentTechnique();
		auto& pass = currentTechnique.GetPass(0);

		pass.Apply(0, m_app->GetDeviceContext());
	}
} // namespace library
