#include <iostream>
#include <boost/filesystem.hpp>
#include "framework/assets.h"



int main(int argc, char** argv)
{
	std::cout << file::getResourceDirectory() << "\n";
	
	return 0;
}
