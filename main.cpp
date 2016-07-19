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

#include "Filesystem.h"
#include "Processor.h"

int main(int argc, char **argv) {

  if (argc != 2) {
    std::cerr << "Usage: wav2mp3 wav_files_folder" << std::endl;
    return 1;
  }

  std::vector<std::string> filenames = filesystem::get_wav_files(argv[1]);
  for (const auto& s : filenames) {
    std::cout << s << std::endl;
  }

  Processor processor;
  processor.encode(std::move(filenames));

  return 0;
}
