#include "stdafx.h"
#include "TestUtil.h"
#include <fstream>
#include <Windows.h>


void TestUtil::ToFile(std::string const &file, std::string const& content)
{
  std::ofstream o(file);

  o << content;

  o.close();
}


std::vector<std::string> TestUtil::get_all_files_names_within_folder(std::string const &folder, std::string const &pattern)
{
  std::vector<std::string> names;
  std::string search_path = folder + "/" + pattern;
  WIN32_FIND_DATA fd;
  HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
  if (hFind != INVALID_HANDLE_VALUE) {
    do {
      // read all (real) files in current folder
      // , delete '!' read other 2 default folder . and ..
      if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
        names.push_back(fd.cFileName);
      }
    } while (::FindNextFile(hFind, &fd));
    ::FindClose(hFind);
  }
  return names;
}

std::string TestUtil::ExePath() {
  char buffer[MAX_PATH];
  GetModuleFileName(NULL, buffer, MAX_PATH);
  std::string::size_type pos = std::string(buffer).find_last_of("\\/");
  return std::string(buffer).substr(0, pos);
}
