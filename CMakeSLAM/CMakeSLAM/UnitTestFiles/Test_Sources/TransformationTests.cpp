#include "../TestMain.h"
#include <stdio.h>

#include <CMakeSLAM/Catch/catch.hpp>
#include <CMakeSLAM/HeaderFiles/FileMapping.h>
#include <CMakeSLAM/HeaderFiles/Transformation.h>

using namespace std;

FileMapping create_file_mapping()
{
    auto file_mapping = FileMapping();
    file_mapping.add_file("file1", InputFile("file1", "file1.fasta"));
    file_mapping.add_file("file2", InputFile("file2", "file2.fasta"));

    return file_mapping;
}

TEST_CASE("Transformation_Constructor_Test")
{
    string test_string = "test";

    auto transformation = Transformation(create_file_mapping(), test_string);

    REQUIRE(transformation.input_transformation_string_ == test_string);
}

TEST_CASE("Transformation_Empty_Transformation_Test")
{
    string empty_string = "";

    CHECK_THROWS(Transformation(create_file_mapping(), empty_string));
}

TEST_CASE("Transformation_Parse_Unrecognized_Enum_Test")
{
    string unrecognized_string = "unknown";

    auto transformation = Transformation(create_file_mapping(), unrecognized_string);

    CHECK_THROWS(transformation.parse_enum());
}

TEST_CASE("Transformation_Parse_concatQualQuery_Test")
{
    string trans_string = "concatQualQuery";

    auto transformation = Transformation(create_file_mapping(), trans_string);

    // no error thrown : valid input
    transformation.parse_enum();
}

TEST_CASE("Transformation_Parse_alignFromList_Test")
{
    string trans_string = "alignFromList";

    auto transformation = Transformation(create_file_mapping(), trans_string);

    // no error thrown : valid input
    transformation.parse_enum();
}

