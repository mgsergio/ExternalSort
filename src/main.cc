#include "io/file_stream.hh"
#include "io/string_wrapper.hh"
#include "sort/sort.hh"

#include <cassert>
#include <cstdio>
#include <cstring>

#include <unistd.h>

using namespace io;
using namespace sort;
using namespace std;

template <typename T1, typename T2>
void copy(T1 nameOrFile1, T2 nameOrFile2)
{
  FileStream inputStream(nameOrFile1, "r");
  FileStream outputStream(nameOrFile2, "w");

  StringWrapper in(inputStream);
  StringWrapper out(outputStream);

  string s;
  while (in.read(s))
    out.write(s);
}

int main(int argc, char * argv[])
{
  assert(argc == 2);

  char const fileNameTemplate[] = "/tmp/extsort.XXXX";

  char tmpInputFileName[sizeof(fileNameTemplate)]{};
  char tmpOutputFileName[sizeof(fileNameTemplate)]{};
  char tmpAuxFileName[sizeof(fileNameTemplate)]{};

  memmove(tmpInputFileName, fileNameTemplate, sizeof(fileNameTemplate));
  memmove(tmpOutputFileName, fileNameTemplate, sizeof(fileNameTemplate));
  memmove(tmpAuxFileName, fileNameTemplate, sizeof(fileNameTemplate));

  mktemp(tmpInputFileName);
  mktemp(tmpOutputFileName);
  mktemp(tmpAuxFileName);

  copy(argv[1], tmpInputFileName);

  {
    FileStream inputStream(tmpInputFileName, "r+");
    FileStream outputStream(tmpOutputFileName, "w+x");
    FileStream auxStream(tmpAuxFileName, "w+x");

    StringWrapper input(inputStream);
    StringWrapper output(outputStream);
    StringWrapper aux(auxStream);

    mergeSort(input, output, aux);
  }

  copy(tmpOutputFileName, stdout);

  // rename(tmpOutputFileName, inputFileName);
  remove(tmpInputFileName);
  remove(tmpOutputFileName);
  remove(tmpAuxFileName);

  return 0;
}
