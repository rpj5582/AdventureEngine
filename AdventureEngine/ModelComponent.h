#pragma once
#include "Component.h"

#include "Model.h"

namespace AdventureEngine
{
	class ModelComponent : public Component
	{
	public:
		ModelComponent();
		ModelComponent(Model* model);
		virtual ~ModelComponent();

		virtual void init() override;
		virtual void update() override;

		const Model* getModel() const;

	protected:
		Model* m_model;
	};
}
