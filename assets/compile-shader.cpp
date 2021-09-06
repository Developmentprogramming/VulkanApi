#include <fstream>
#include <filesystem>
#include <iostream>
#include <stdlib.h>

namespace fs = std::filesystem;

void CompileShader(const fs::path& source, const fs::path& output)
{
	std::string glslc_command = "glslc " + source.string() + " -o " + output.string() + "/" + source.filename().string() + ".spv";
	std::system(glslc_command.c_str());
}

int main(int argc, char* argv[])
{
	fs::path shader("shaders");
	fs::path shaderSpv("shaders-spv");

	if (!fs::exists(shader) && !fs::exists(shaderSpv))
		return 1;

	for (auto dir : fs::recursive_directory_iterator(shader))
	{
		std::cout << dir.path().filename() << std::endl;
		CompileShader(dir.path(), shaderSpv);
	}

	return 0;
}
