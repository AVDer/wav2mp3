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

#include "Encoder.h"

#include <iomanip>
#include <iostream>
#include <lame/lame.h>

#include "HandlerManager.h"

namespace wav2mp3 {

  void Encoder::encode(const std::vector<std::string> &wav_filenames) {
    for (const auto &f : wav_filenames) {
      std::cout << f;
      auto status = encode(f);
      std::cout << std::setw(30) << (status == CodecResult::CR_OK ? "OK" : "FAULT") << std::endl;
    }
  }


  Encoder::CodecResult Encoder::encode(const std::string &wav_filename) {

    if (!ends_with(wav_filename, ".wav")) {
      return CodecResult::CR_FILE_NAME;
    }

    int bytes_read {};
    int bytes_to_write {};

    HandlerManager<FILE*, int (*)(FILE *)> wav_file(fopen(wav_filename.c_str(), "rb"), fclose);
    if (!wav_file.handler_ok()) {
      return CodecResult::CR_IF_OPEN;
    }

    HandlerManager<FILE*, int (*)(FILE *)> mp3_file(
        fopen((wav_filename.substr(0, wav_filename.size() - 3) + "mp3").c_str(), "wb"), fclose);
    if (!mp3_file.handler_ok()) {
      return CodecResult::CR_OF_OPEN;
    }

    HandlerManager<lame_t, int (*)(lame_t)> lame(lame_init(), lame_close);
    if (!lame.handler_ok()) {
      return CodecResult::CR_LAME_INIT;
    }

    int16_t wav_buffer[BUFFER_SIZE * 2];
    uint8_t mp3_buffer[BUFFER_SIZE];

    lame_init_params(lame.handler());

    do {
      bytes_read = fread(wav_buffer, 2 * sizeof(int16_t), BUFFER_SIZE, wav_file.handler());
      if (bytes_read == 0) {
        bytes_to_write = lame_encode_flush(lame.handler(), mp3_buffer, BUFFER_SIZE);
      }
      else {
        bytes_to_write = lame_encode_buffer_interleaved(lame.handler(), wav_buffer, bytes_read, mp3_buffer,
                                                        BUFFER_SIZE);
      }
      fwrite(mp3_buffer, bytes_to_write, 1, mp3_file.handler());
    } while (bytes_read != 0);

    return CodecResult::CR_OK;
  }
}
