#include "ve_globals.hpp"
#include "ve_engine.hpp"
#include <iostream>
#include <cstdlib>
#include <stdexcept>

int main()
{
  ve::WIN_WIDTH = 800;
  ve::WIN_HEIGHT = 600;
  ve::NAME = "Little Vulkan";

	ve::ve_engine vulkanEngine;

	try {
		vulkanEngine.initEngine();

	} catch (const std::exception &e)
	{
		std::cerr << "Error in main: " << e.what() << '\n';
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}