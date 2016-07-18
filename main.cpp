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

#include <iostream>
#include <vector>

#include "Encoder.h"

int main(int argc, char **argv) {

  if (argc < 2) {
    std::cerr << "Usage: wav2mp3 file [files]\n,where file and each of files are wav ones)" << std::endl;
    return 1;
  }

  std::vector<std::string> filenames;
  for (uint32_t i = 1; i < argc; ++i) {
    filenames.emplace_back(argv[i]);
  }

  wav2mp3::Encoder encoder;
  encoder.encode(filenames);

  return 0;
}
