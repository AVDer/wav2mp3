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

#ifndef WAV2MP3_WAVHEADER_H
#define WAV2MP3_WAVHEADER_H

#include <cstdint>
#include <string>

namespace wav2mp3 {

  struct WAVHeader {
    char chunkId[4]; // "RIFF" (0x52494646)
    uint32_t chunkSize;
    char format[4]; // "WAVE" (0x57415645)
  };

  struct FMTHeader {
    char subchunk1Id[4]; // "fmt " (0x666d7420)
    uint32_t subchunk1Size;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
  };

  struct DataHeader {
    char subchunk2Id[4]; // "data" (0x64617461)
    uint32_t subchunk2Size;
  };


}

#endif //WAV2MP3_WAVHEADER_H
