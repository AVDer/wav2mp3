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
    static bool encode(const std::string& wav_filename);
    static void encode(const std::vector<std::string>& wav_filenames);
  };

}


#endif //WAV2MP3_ENCODER_H
