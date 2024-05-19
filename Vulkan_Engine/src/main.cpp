#include "ve_configuration.hpp"
#include "ve_engine.hpp"

#include <iostream>
#include <cstdlib>
#include <stdexcept>

int main()
{
	ve::ve_configuration config{800, 600, "Lil Vulky"};

	ve::ve_engine vulkanEngine{ config };

	try {
		vulkanEngine.initEngine(&vulkanEngine);
	} catch (const std::exception &e)
	{
		std::cerr << "Error in main: " << e.what() << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}