#include "StdAfx.h"
#include "library/components/DrawableInputMaterialComponent.h"

#include "library/Application.h"
#include "library/Model.h"
#include "library/Mesh.h"

#include "library/effect/EffectTechnique.h"
#include "library/effect/EffectPass.h"

#include "library/materials/Material.h"

namespace library
{
	void DrawableInputMaterialComponent::SetDefaultModelName(const std::string& modelName)
	{
		if (m_defaultModelName != modelName)
			m_defaultModelName = modelName;
	}

	void DrawableInputMaterialComponent::Initialize(const Application& app)
	{
		DrawableInputComponent::Initialize(app);

		// set default input layout
		const auto& material = GetMaterial();

		const auto& currentTechnique = material.GetCurrentTechnique();
		const auto& pass = currentTechnique.GetPass(0);
		m_inputLayout = material.GetInputLayoutShared(pass);

		// load default model
		if (!m_defaultModelName.empty())
		{
			const auto& material = GetMaterial();
			assert(material.IsInitialized());

			Model model(*m_app, m_defaultModelName, true);
			const auto& mesh = model.GetMesh(0);

			m_vertexBufferData.buffer = material.CreateVertexBuffer(m_app->GetDevice(), mesh);
			m_vertexBufferData.count = mesh.GetVerticesCount();

			m_indexBufferData = std::make_unique<BufferData>();
			m_indexBufferData->buffer = mesh.CreateIndexBuffer();
			m_indexBufferData->count = mesh.GetIndicesCount();
		}
	}

	unsigned DrawableInputMaterialComponent::GetVertexSize() const
	{
		return GetMaterial().GetVertexSize();
	}

	void DrawableInputMaterialComponent::Draw_SetData()
	{
		auto& currentTechnique = GetMaterial().GetCurrentTechnique();
		auto& pass = currentTechnique.GetPass(0);

		pass.Apply(0, m_app->GetDeviceContext());
	}
} // namespace library
