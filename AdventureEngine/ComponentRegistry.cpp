#include "ComponentRegistry.h"



ComponentRegistry::ComponentRegistry()
{
	Object::registerComponent<RenderComponent>("RenderComponent");
	Object::registerComponent<CameraComponent>("CameraComponent");
	Object::registerComponent<TestComponent>("TestComponent");
}


ComponentRegistry::~ComponentRegistry()
{
}
