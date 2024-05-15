#include "ve_window.hpp"

#include <iostream>
#include <cstdlib>
#include <stdexcept>

int main()
{
	ve::ve_window newWindow{800, 600, "Vulkan Window"};

	try {
	newWindow.mainLoop();
	} catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;

}