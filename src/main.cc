#include "io/buffered_string_wrapper.hh"
#include "io/file_stream.hh"
#include "io/string_wrapper.hh"
#include "sort/sort.hh"

#include "cxxopts/include/cxxopts.hpp"

#include <cassert>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

#include <unistd.h>

using namespace io;
using namespace sort;
using namespace std;

size_t bufferSize = 256;

template <typename Wrapper>
void copyImpl(FileStream & inpupt, FileStream & ouput);

template <>
void copyImpl<StringWrapper>(FileStream & input, FileStream & output)
{
  StringWrapper in(input);
  StringWrapper out(output);

  StringWrapper::value_type s;
  while (in.read(s))
    out.write(s);
}

template <>
void copyImpl<BufferedStringWrapper>(FileStream & input, FileStream & output)
{
  BufferedStringWrapper in(input, bufferSize);
  BufferedStringWrapper out(output, bufferSize);

  BufferedStringWrapper::value_type s;
  while (in.read(s))
    out.write(s);
}

template <typename Wrapper, typename T1, typename T2>
void copy(T1 nameOrFile1, T2 nameOrFile2)
{
  FileStream input(nameOrFile1, "r");
  FileStream output(nameOrFile2, "w");

  copyImpl<Wrapper>(input, output);
}

template <typename Wrapper>
void sortFile(char const *inputFileName, char const *outputFileName,
          char const *auxFileName);

template <>
void sortFile<BufferedStringWrapper>(char const *inputFileName,
                                 char const *outputFileName,
                                 char const *auxFileName)
{
  FileStream inputStream(inputFileName, "r+");
  FileStream outputStream(outputFileName, "w+x");
  FileStream auxStream(auxFileName, "w+x");

  BufferedStringWrapper input(inputStream, bufferSize);
  BufferedStringWrapper output(outputStream, bufferSize);
  BufferedStringWrapper aux(auxStream, bufferSize);

  mergeSort(input, output, aux);
}

template <>
void sortFile<StringWrapper>(char const *inputFileName, char const *outputFileName,
                         char const *auxFileName)
{
  FileStream inputStream(inputFileName, "r+");
  FileStream outputStream(outputFileName, "w+x");
  FileStream auxStream(auxFileName, "w+x");

  StringWrapper input(inputStream);
  StringWrapper output(outputStream);
  StringWrapper aux(auxStream);

  mergeSort(input, output, aux);
}

// TODO(mgsergio): Add in-memory sort stage.
template <typename Wrapper>
void doWork(string const & fileName)
{
  char const fileNameTemplate[] = "/tmp/extsortFile.XXXX";

  char tmpInputFileName[sizeof(fileNameTemplate)]{};
  char tmpOutputFileName[sizeof(fileNameTemplate)]{};
  char tmpAuxFileName[sizeof(fileNameTemplate)]{};

  memmove(tmpInputFileName, fileNameTemplate, sizeof(fileNameTemplate));
  memmove(tmpOutputFileName, fileNameTemplate, sizeof(fileNameTemplate));
  memmove(tmpAuxFileName, fileNameTemplate, sizeof(fileNameTemplate));

  mktemp(tmpInputFileName);
  mktemp(tmpOutputFileName);
  mktemp(tmpAuxFileName);

  copy<Wrapper>(fileName, tmpInputFileName);
  sortFile<Wrapper>(tmpInputFileName, tmpOutputFileName, tmpAuxFileName);
  copy<Wrapper>(tmpOutputFileName, stdout);

  remove(tmpInputFileName);
  remove(tmpOutputFileName);
  remove(tmpAuxFileName);
}

int main(int argc, char * argv[])
{
  cxxopts::Options options(argv[0], "Sort large files of strings");
  options.positional_help("filename");
  options.add_options()
      ("help", "Print help")
      ("filename", "Path to file to sort", cxxopts::value<string>())
      ("b,buffered", "Buffered mode", cxxopts::value<size_t>())
      ;
  options.parse_positional("filename");
  options.parse(argc, argv);

  try
  {
    if (options.count("help"))
    {
      cout << options.help();
      exit(0);
    }
    if (!options.count("filename"))
    {
      cout << options.help();
      exit(1);
    }
    auto const fileName = options["filename"].as<string>();
    if (options.count("buffered"))
    {
      bufferSize = std::min(options["buffered"].as<size_t>(), bufferSize);
      doWork<BufferedStringWrapper>(fileName);
    }
    else
    {
      doWork<StringWrapper>(fileName);
    }
  }
  catch (cxxopts::OptionException const & e)
  {
    cout << "error parsing options: " << e.what() << endl;
    exit(1);
  }

  return 0;
}
