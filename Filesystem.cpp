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

namespace filesystem {

  std::vector<std::string> get_wav_files(const std::string &directory) {

    std::vector<std::string> result;

#ifdef WIN32
    HANDLE dir;
    WIN32_FIND_DATA file_data;

    if ((dir = FindFirstFile((directory + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE) {
      return result;
    }

    do {
      if (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        continue;
      }
      if (ends_with(file_data.cFileName, ".wav")) {
        result.emplace_back(directory + "/" + file_data.cFileName);
      }
    } while (FindNextFile(dir, &file_data));

    FindClose(dir);
#else


#endif

    return result;
  }

}