#pragma once


#include <string>
#include <vector>


class TestUtil
{
public:


  static void ToFile(std::string const &file, std::string const& content);

  static std::vector<std::string> get_all_files_names_within_folder(std::string const &folder, std::string const &pattern);

  static std::string ExePath();

};

