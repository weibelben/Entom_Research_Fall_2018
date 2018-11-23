#include <list>
#include <string>

#include "../HeaderFiles/InstructionPackage.h"
#include "../HeaderFiles/CrashAndBurn.h"


using namespace std;

InstructionPackage::InstructionPackage(list<string> raw_instructions)
{
    fileMapping = FileMapping();
    instructions = raw_instructions;
    insIterator = instructions.begin();
}

void InstructionPackage::populate_members()
{
    if (instructions.empty())
    {
        throw CrashAndBurn("Instruction package empty.\n");
    }

    set_instruction_title();
    set_input_files();
    set_output_files();
    set_transformations();
}

/*
 * Parsed Instruction Title Format:
 * <Title of ins file>
 * as the first line with content.
 */
void InstructionPackage::set_instruction_title()
{
    string ins_line = continue_iterator("Instruction Title");

    if (ins_line == "Input Files:\n")
    {
        instructionTitle = "Auto-Generated Instruction Title";
        --insIterator;
    }
    else
    {
        instructionTitle = ins_line;
    }
}

/*
 * Parsed Input File Format:
 * Input Files:
 *      fileA = path/to/file/fileA.fasta
 */
void InstructionPackage::set_input_files()
{
    string ins_line = continue_iterator("Input Files");
    
    if (ins_line.find("Input Files:") != 0)
    {
        const string error;
        CrashAndBurn(error
            + "The instruction following the instruction title did not "
            + "specify input files.\nUsage:\nInput Files:\n\tfileN = <path>\n");
    }
    
    // at this point, the iterator contains the line "Input Files:"
    while (!(*insIterator).empty() && (*insIterator).find("Output Files:") != 0)
    {
        const string identifier = split_identifier(*insIterator);
        const string file_path = split_path(*insIterator);
        inputFiles.emplace_back(identifier, file_path);
        fileMapping.add_file(identifier, InputFile(identifier, file_path));

        ++insIterator;
        if (insIterator == instructions.end())
        {
            CrashAndBurn("There are no instructions following the input files\n");
        }
    }

    // no input files were parsed
    if (fileMapping.empty())
    {
        CrashAndBurn("No input files were specified.\n");
    }
    num_input_files_ = fileMapping.num_mappings;
}
/*
 * Parsed Output File Format:
 * Output Files:
 *      fileA = path/to/file/fileA.fasta
 */
void InstructionPackage::set_output_files()
{
    string ins_line = continue_iterator("Output Files");
    
    if (ins_line.find("Output Files:") != 0)
    {
        const string error;
        CrashAndBurn(error
            + "The instruction following the input files did not specify output files.\n"
            + "Usage:\nOutput Files:\n\tfileN = <path>\n");
    }

    // at this point, the iterator contains the line "Output Files:"
    while (!(*insIterator).empty() && (*insIterator).find("Transformations:") != 0)
    {
        const string identifier = split_identifier(*insIterator);
        const string file_path = split_path(*insIterator);
        outputFiles.emplace_back(identifier, file_path);
        fileMapping.add_file(identifier, InputFile(identifier, file_path));

        ++insIterator;
        if (insIterator == instructions.end())
        {
            CrashAndBurn("No instructions following the output files\n");
        }
    }

    // no input files were parsed
    if (fileMapping.num_mappings == num_input_files_)
    {
        // No output files were added to the file_mapping
        CrashAndBurn("No output files were found.\n");
    }
}

/*
 * Parsed Transformation Format:
 * Transformations:
 *      <transformation>(fileA.fasta -> fileB.out)
 */
void InstructionPackage::set_transformations()
{
    string ins_line = continue_iterator("Transformations");
    
    if (!ins_line.empty() && ins_line.find("Transformations:") != 0)
    {
        const string error;
        CrashAndBurn(error
            + "The instruction following the output files did not specify transformations.\n"
            + "Usage:\nTransformations:\n\t<transformation>(fileA.ext -> fileB.ext)\n");
    }

    // at this point, the iterator contains the line "Transformations:"
    while (!(*insIterator).empty() && insIterator != instructions.end())
    {
        auto transformation = Transformation(fileMapping, *insIterator);
        transformation.parse_enum();
        transformations.push_back(transformation);
        ++insIterator;        
    }

    // no transformations were parsed
    if (transformations.empty())
    {
        CrashAndBurn("No transformations were found.\n");
    }
}

/*
 * Parses the file identifier from the input or output file line:
 *      fileA = path/to/file/fileA.fasta
 */
string InstructionPackage::split_identifier(string line)
{
    const size_t index = line.find('=');

    if (index == string::npos)
    {
        CrashAndBurn("Input File Format:\n\tidentifier=path/to/file.ext\n");
    }

    return remove_whitespace(line.substr(0, line.find('=')));
}

/*
 * Parses the file path from the input or output file line:
 *      fileA = path/to/file/fileA.fasta
 */
string InstructionPackage::split_path(string line)
{
    const size_t index = line.find('=');
    return remove_whitespace(line.substr(index + 1, line.length() - index));
}

/*
 * This method removes whitespace (space or tab chars) from the beginning and end of str
 */
string InstructionPackage::remove_whitespace(string str)
{
    int start = 0;
    size_t end = str.length() - 1;

    // ascii for space and tab char
    while (str.at(start) == 32 || str.at(start) == 9) { start++; }
    while (str.at(end) == 32 || str.at(end) == 9) { end--; }
    return str.substr(start, end - start + 1);
}

/*
 * Increments the instruction pointer until a line is found with content.
 * Parameters:  instruction is the method that calls the continuation.
 *              argument is the line that was first parsed before continuation call.
 * Returns the first line with content.
 */
string InstructionPackage::continue_iterator(string instruction)
{
    string argument = *insIterator;

    while (argument.empty())
    {
        ++insIterator;
        argument = *insIterator;

        if (insIterator == instructions.end())
        {
            CrashAndBurn("The instruction file did not contain any "
                + instruction + ".\n");
        }
    }

    ++insIterator;
    return argument;
}