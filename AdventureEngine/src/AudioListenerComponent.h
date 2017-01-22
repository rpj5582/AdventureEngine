#pragma once

#include "Component.h"

#include <al.h>
#include <alc.h>

namespace AdventureEngine
{
	class AudioListenerComponent : public Component
	{
	public:
		AudioListenerComponent();
		~AudioListenerComponent();

	private:
		virtual void init() override;
		virtual void update(float deltaTime) override;
	};
}