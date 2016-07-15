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

#ifndef WAV2MP3_ENCODER_H
#define WAV2MP3_ENCODER_H

#include <string>
#include <vector>

namespace wav2mp3 {

  class Encoder {
    static const size_t BUFFER_SIZE {8192};
  public:
    enum class CodecResult {
      CR_OK,
      CR_IF_OPEN,
      CR_OF_OPEN,
      CR_LAME_INIT,
      CR_FILE_NAME,
      CR_NOT_WAVE
    };

    static CodecResult encode(const std::string& wav_filename);
    static void encode(const std::vector<std::string>& wav_filenames);

  private:
    inline static bool ends_with(const std::string& str, const std::string& ending) {
      if (ending.size() > str.size()) return false;
      return std::equal(ending.rbegin(), ending.rend(), str.rbegin());
    }
  };

}


#endif //WAV2MP3_ENCODER_H