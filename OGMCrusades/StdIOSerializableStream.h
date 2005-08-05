// $Id$

// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#ifndef _STD_IO_SERIALIZABLE_STREAM_H_
#define _STD_IO_SERIALIZABLE_STREAM_H_

#include <stdio.h>
#include "SerializableStream.h"

class StdIOSerializableStream :
  public SerializableStream
{
public:
  StdIOSerializableStream(const std::string &filename, bool bWritable);
  ~StdIOSerializableStream();

  uint64_t Read64();
  uint32_t Read32();
  uint16_t Read16();
  uint8_t Read8();
  double ReadDouble();
  std::string ReadString();

  void Write64(uint64_t i);
  void Write32(uint32_t i);
  void Write16(uint16_t i);
  void Write8(uint8_t i);
  void WriteDouble(double d);
  void WriteString(const std::string& string);

protected:
  FILE *m_File;
};

#endif // _STD_IO_SERIALIZABLE_STREAM_H_