#include "ve_engine.hpp"

#include <iostream>
#include <cstdlib>
#include <stdexcept>

int main()
{
	ve::ve_engine vulkanEngine{ 800, 600, "Vulkan Window" };

	try {
		vulkanEngine.initEngine();
	} catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}