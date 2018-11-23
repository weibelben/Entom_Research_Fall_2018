
#include <CMakeSLAM/Catch/catch.hpp>
#include <CMakeSLAM/HeaderFiles/InputFile.h>

using namespace std;

const string file_name = "file_name";
const string file_path = "file_path";

TEST_CASE("InputFile_Constructor_FileName_Test")
{
    auto inputFile = InputFile(file_name, file_path);

    REQUIRE(inputFile.fileName == "file_name");
}

TEST_CASE("InputFile_Constructor_FilePath_Test")
{
    auto inputFile = InputFile(file_path, file_path);

    REQUIRE(inputFile.filePath == "file_path");
}