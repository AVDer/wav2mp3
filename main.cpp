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
//  for (const auto& s : filenames) {
//    std::cout << s << std::endl;
//  }

  Processor processor;
  auto result = processor.encode(std::move(filenames));
  if (result != Processor::ThreadResult::TR_OK) {
    std::cerr << "Error: ";
    switch (result) {
      case Processor::ThreadResult::TR_MUTEX_ERROR:
        std::cerr << "Mutex initialization error";
        break;
      case Processor::ThreadResult::TR_ATTR_ERROR:
        std::cerr << "Thread attributes initialization error";
        break;
      case Processor::ThreadResult::TR_THR_CREATE:
        std::cerr << "Conversion thread can't be created";
        break;
      case Processor::ThreadResult::TR_THR_JOIN:
        std::cerr << "Conversion thread join error";
        break;
      default:
        std::cerr << "Unknown error";
        break;
    }
    std::cerr << std::endl;
    return 1;
  }

  return 0;
}
