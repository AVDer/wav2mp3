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

#ifndef WAV2MP3_FILESYSTEM_H
#define WAV2MP3_FILESYSTEM_H

#include <string>
#include <vector>

namespace filesystem {

  //! Get vector of WAV files in the specified folder
  /*!
    \param directory - Directory to search files in
    \return Vector of found WAV files
  */
  std::vector<std::string> get_wav_files(const std::string &directory);

  inline static bool ends_with(const std::string &str, const std::string &ending) {
    if (ending.size() > str.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), str.rbegin());
  }

}


#endif //WAV2MP3_FILESYSTEM_H
