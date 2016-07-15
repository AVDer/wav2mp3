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

#ifndef WAV2MP3_RESOURCEMANAGER_H
#define WAV2MP3_RESOURCEMANAGER_H

#include <functional>

template <typename H, class F>
class HandlerManager {
public:
  HandlerManager(H handler, F close_function):
      handler_(handler),
      close_function_(close_function) {

  }

  H& handler() {
    return handler_;
  }

  bool handler_ok() const {
    return handler_ != nullptr;
  }

  ~HandlerManager() {
    std::bind(close_function_, handler_);
  }

private:
  H handler_;
  F close_function_;
};


#endif //WAV2MP3_RESOURCEMANAGER_H
