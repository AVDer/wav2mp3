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

#include <cassert>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <lame/lame.h>

#include "HandlerManager.h"

namespace wav2mp3 {

  /********************************************************************************/


  Encoder::CodecResult Encoder::encode(const std::string &wav_filename) {

    /* 1. Open WAV file
     * 2. Fill WAV file headers
     * 3. Create output MPS file
     * 4. Create Lame object and initialize it
     * 5. Convert data basing on bitrate
     */


    // Get WAV file handler
    HandlerManager<FILE*, int (*)(FILE *)> wav_file(fopen(wav_filename.c_str(), "rb"), fclose);
    if (!wav_file.handler_ok()) {
      return CodecResult::CR_IF_OPEN;
    }

    // Parse WAV file
    auto fill_result = fill_headers(wav_file.handler());
    if (fill_result != CodecResult::CR_OK) {
      return fill_result;
    }
    assert(data_offset); // Headers must be initialized

//    std::cout << "Sample rate: "<< fmt_header.sampleRate << std::endl;
//    std::cout << "Channels: "<< fmt_header.numChannels << std::endl;
//    std::cout << "Bits per sample: "<< fmt_header.bitsPerSample << std::endl;
//    std::cout << "Format: "<< fmt_header.audioFormat << std::endl;
//    std::cout << "Samples: "<< number_of_samples << std::endl;

    if (fmt_header.bitsPerSample != 8 &&
        fmt_header.bitsPerSample != 16 &&
        fmt_header.bitsPerSample != 32) {
      return CodecResult::CR_BITRATE;
    }

    auto number_of_samples = data_header.subchunk2Size / fmt_header.numChannels / (fmt_header.bitsPerSample / 8);

    // Create output MP3 file
    HandlerManager<FILE*, int (*)(FILE *)> mp3_file(
        fopen((wav_filename.substr(0, wav_filename.size() - 3) + "mp3").c_str(), "wb"), fclose);
    if (!mp3_file.handler_ok()) {
      return CodecResult::CR_OF_OPEN;
    }

    // Get Lame handler
    HandlerManager<lame_t, int (*)(lame_t)> lame(lame_init(), lame_close);
    if (!lame.handler_ok()) {
      return CodecResult::CR_LAME_INIT;
    }

    // Set up Lame
    lame_set_num_samples(lame.handler(), number_of_samples);
    lame_set_in_samplerate(lame.handler(), fmt_header.sampleRate);
    lame_set_num_channels(lame.handler(), fmt_header.numChannels);
    if (fmt_header.numChannels == 1) {
      lame_set_mode(lame.handler(), MPEG_mode::MONO);
    }
    lame_set_quality(lame.handler(), 5); // Good quality
    if (lame_init_params(lame.handler()) == -1) {
      return CodecResult::CR_LAME_PARAM;
    }

    fseek(wav_file.handler(), data_offset, SEEK_SET);

    int8_t wav_1b_buffer[BUFFER_SIZE];
    int16_t wav_2b_buffer[BUFFER_SIZE];
    int32_t wav_4b_buffer[BUFFER_SIZE];
    uint8_t mp3_buffer[BUFFER_SIZE];
    int samples_read {};
    int bytes_to_write {};

    // Perform the encoding
    do {
      if (fmt_header.bitsPerSample == 8) {
        samples_read = fread(wav_1b_buffer, 1, BUFFER_SIZE, wav_file.handler());
        for (size_t i = 0; i < BUFFER_SIZE; ++i) {
          wav_2b_buffer[i] = static_cast<int16_t>((static_cast<int16_t>(wav_1b_buffer[i]) - 0x80) *256);
        }
      }
      else if (fmt_header.bitsPerSample == 16) {
        samples_read = fread(wav_2b_buffer, 2, BUFFER_SIZE, wav_file.handler());
      }
      else if (fmt_header.bitsPerSample == 32) {
        samples_read = fread(wav_4b_buffer, 4, BUFFER_SIZE, wav_file.handler());
        for (size_t i = 0; i < BUFFER_SIZE; ++i) {
          wav_2b_buffer[i] = static_cast<int16_t>(wav_4b_buffer[i] / 65536);
        }
      }

      if (samples_read == 0) {
        bytes_to_write = lame_encode_flush(lame.handler(), mp3_buffer, BUFFER_SIZE);
      }
      else {
        if (fmt_header.numChannels == 1) {
          bytes_to_write = lame_encode_buffer(lame.handler(), wav_2b_buffer, nullptr, samples_read, mp3_buffer, BUFFER_SIZE);
        }
        else {
          bytes_to_write = lame_encode_buffer_interleaved(lame.handler(), wav_2b_buffer, samples_read / 2, mp3_buffer,
                                                          BUFFER_SIZE);
        }
      }

      fwrite(mp3_buffer, bytes_to_write, 1, mp3_file.handler());
    } while (samples_read != 0);

    return CodecResult::CR_OK;
  }


  /********************************************************************************/


  Encoder::CodecResult Encoder::fill_headers(FILE* file) {

    int bytes_read {};

    bytes_read = fread(&riff_header, 1, sizeof(RIFFHeader), file);
    if (bytes_read != sizeof(RIFFHeader)) {
      return  CodecResult::CR_NOT_WAVE;
    }
    if (strncmp(riff_header.chunkId, "RIFF", 4) != 0 || strncmp(riff_header.format, "WAVE", 4) != 0) {
      return  CodecResult::CR_NOT_WAVE;
    }


    bool fmt_chunk_found {false};
    while (!fmt_chunk_found && !feof(file)) {
      fread(&data_header, sizeof(DataHeader), 1, file);
      if (strncmp(data_header.subchunk2Id, "fmt ", 4) == 0) {
        fseek(file, ftell(file) - sizeof(DataHeader), SEEK_SET);
        fread(&fmt_header, sizeof(FMTHeader), 1, file);
        fmt_chunk_found = true;
        break;
      } else {
        fseek(file, data_header.subchunk2Size, SEEK_CUR);
      }
    }
    if (!fmt_chunk_found) {
      return  CodecResult::CR_NO_FMT;
    }
    fseek(file, ftell(file) - sizeof(FMTHeader) + fmt_header.subchunk1Size + 8, SEEK_SET);


    bool data_chunk_found {false};
    while (!data_chunk_found && !feof(file)) {
      fread(&data_header, sizeof(DataHeader), 1, file);
      if (strncmp(data_header.subchunk2Id, "data", 4) == 0) {
        data_chunk_found = true;
        data_offset = ftell(file);
        break;
      } else {
        fseek(file, data_header.subchunk2Size, SEEK_CUR);
      }
    }
    if (!data_chunk_found) {
      return  CodecResult::CR_NO_DATA;
    }
  }

}

