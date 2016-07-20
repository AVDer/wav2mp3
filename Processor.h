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

#ifndef WAV2MP3_THREAD_H
#define WAV2MP3_THREAD_H

#include <pthread.h>
#include <string>
#include <vector>

//!  Processor class
/*!
     Provides an encode function that takes a vector of WAV file names and converts them to MP3.
     Encoder object is used for the conversion itself.
*/
class Processor {

public:

  //! Thread creating errors codes
  enum class ThreadResult {
    TR_OK,            /*!< No errors occured */
    TR_MUTEX_ERROR,   /*!< Mutex can't be initialized */
    TR_ATTR_ERROR,    /*!< Thread attributes can't be initialized */
    TR_THR_CREATE,    /*!< Thread create error */
    TR_THR_JOIN       /*!< Thread join error */
  };

  //! Encode multiple WAV files with multithreading support
  /*!
    \param wav_filenames - vector of WAV file names
    \param threads_number - number of threads to use; 0 - number of threads is equal to CPU cores amount
  */
  ThreadResult encode(std::vector<std::string> &&wav_filenames, uint32_t threads_number = 0);
};

//! Thread parameters structure. Is shared among the threads
struct ThreadParams {
  uint32_t files_processed;
  uint32_t success_number;
  std::vector<std::string> filenames;
};


#endif //WAV2MP3_THREAD_H
