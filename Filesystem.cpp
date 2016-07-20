/********************************************************************************
Simple C/C++ command line application that encodes a set of WAV files to MP3
Copyright (C) 2016  Andrey Derevyanko

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
********************************************************************************/

#include "Filesystem.h"

#ifdef WIN32
#include <windef.h>
#include <windows.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#endif

#include "HandlerManager.h"

namespace filesystem {

  std::vector<std::string> get_wav_files(const std::string &directory) {

    std::vector<std::string> result;

#ifdef WIN32

    WIN32_FIND_DATA file_data;
    HandlerManager<HANDLE, WINBOOL(*)(HANDLE)> dir(FindFirstFile((directory + "/*").c_str(), &file_data), FindClose);

    if (dir.handler() == INVALID_HANDLE_VALUE) {
      return result;
    }

    do {
      if (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        continue;
      }
      if (ends_with(file_data.cFileName, ".wav") || ends_with(file_data.cFileName, ".WAV")) {
        result.emplace_back(directory + "/" + file_data.cFileName);
      }
    } while (FindNextFile(dir.handler(), &file_data));

#else

    dirent *ent;
    struct stat file_status;

    HandlerManager<DIR*, int(*)(DIR*)> dir(opendir(directory.c_str()), closedir);

    while ((ent = readdir(dir.handler())) != NULL) {
      const std::string filename = ent->d_name;
      const std::string full_filename = directory + "/" + filename;

      if (filename[0] == '.' || stat(full_filename.c_str(), &file_status) == -1 || file_status.st_mode & S_IFDIR) {
        continue;
      }

      if (ends_with(filename, ".wav") || ends_with(filename, ".WAV")) {
        result.emplace_back(full_filename);
      }
    }

#endif

    return result;
  }

}