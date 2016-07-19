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

#include "WAVHeader.h"

namespace wav2mp3 {


  //!  Encoder class
  /*!
       Class provides functions for basic WAV file parsing and it's encoding to MP3 format with pre-defined quality
  */
  class Encoder {
    static const size_t BUFFER_SIZE{8192};
  public:

    //! Errors codes
    enum class CodecResult {
      CR_OK,            /*!< No errors in encoding process */
      CR_IF_OPEN,       /*!< Error while opening WAV file */
      CR_OF_OPEN,       /*!< MP3 file can't be created */
      CR_LAME_INIT,     /*!< Lame handler can't be got */
      CR_LAME_PARAM,    /*!< Lame can't be initialized */
      CR_NOT_WAVE,      /*!< WAV file headers are incorrect or absent */
      CR_NO_FMT,        /*!< FMT section not found */
      CR_NO_DATA        /*!< DATA section not found */
    };

    //! Single file encode function
    /*!
      \param wav_filename - WAV file name
      \return Encoding result
      \sa CodecResult
    */
    CodecResult encode(const std::string &wav_filename);

  private:
    RIFFHeader riff_header;
    FMTHeader fmt_header;
    DataHeader data_header;
    long data_offset;

    //! Fill RIFF, FMT and Data headers that are the class members
    /*!
      \param file - WAV file handler
      \return File handling result
      \sa CodecResult
    */
    CodecResult fill_headers(FILE *file);

  };

}


#endif //WAV2MP3_ENCODER_H
