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

#include "Processor.h"

#include <iomanip>
#include <iostream>
#include <thread>

#include "Encoder.h"

/********************************************************************************/
static pthread_mutex_t processor_mutex;
/********************************************************************************/

//! Thread function for files encoding. Files are taken one by one from the vector of filenames
/*!
  \param params - pointer to ThreadParams structure containing files list and shared counter of processed ones
*/
void *encoding_thread(void *params) {
  using wav2mp3::Encoder;

  ThreadParams *parameter = reinterpret_cast<ThreadParams *>(params);
  const auto files_number = parameter->filenames.size();
  Encoder encoder;
  while (parameter->files_processed < files_number) {

    pthread_mutex_lock(&processor_mutex);
    const auto file_number = parameter->files_processed++;
    pthread_mutex_unlock(&processor_mutex);

    if (file_number >= files_number) {
      break;
    }
    auto status = encoder.encode(parameter->filenames.at(file_number));

    pthread_mutex_lock(&processor_mutex);
    std::cout << std::setw(70) << std::left << parameter->filenames.at(file_number);
    std::cout << std::right << (status == Encoder::CodecResult::CR_OK ? "OK" : "FAULT: ");
    switch (status) {
      case Encoder::CodecResult::CR_OK:
        break;
      case Encoder::CodecResult::CR_IF_OPEN:
        std::cout << "File can't be opened";
        break;
      case Encoder::CodecResult::CR_OF_OPEN:
        std::cout << "Can't create output file";
        break;
      case Encoder::CodecResult::CR_LAME_INIT:
      case Encoder::CodecResult::CR_LAME_PARAM:
        std::cout << "Lame initialization error";
        break;
      case Encoder::CodecResult::CR_NOT_WAVE:
      case Encoder::CodecResult::CR_NO_FMT:
      case Encoder::CodecResult::CR_NO_DATA:
        std::cout << "WAV file format error";
        break;
      default:
        std::cout << "Unknown error";
        break;
    }
    std::cout << std::endl;
    pthread_mutex_unlock(&processor_mutex);
  }
  pthread_exit(nullptr);
}

/********************************************************************************/

void Processor::encode(std::vector<std::string> &&wav_filenames, uint32_t threads_number/* = 0*/) {
  if (threads_number == 0) {
    threads_number = std::thread::hardware_concurrency();
  }
  if (threads_number == 0) {
    threads_number = 1;
  }
  pthread_t threads[threads_number];
  ThreadParams parameters;
  parameters.filenames = std::move(wav_filenames);
  parameters.files_processed = 0;

  pthread_mutex_init(&processor_mutex, nullptr);

  for (uint32_t thr_index = 0; thr_index < threads_number; ++thr_index) {
    pthread_create(&threads[thr_index], nullptr, encoding_thread, &parameters);
  }
  for (uint32_t thr_index = 0; thr_index < threads_number; ++thr_index) {
    pthread_join(threads[thr_index], nullptr);
  }

  pthread_mutex_destroy(&processor_mutex);
}
