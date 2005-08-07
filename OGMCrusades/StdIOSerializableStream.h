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

  Uint64 Read64();
  Uint32 Read32();
  Uint16 Read16();
  Uint8 Read8();
  double ReadDouble();
  std::string ReadString();

  void Write64(Uint64 i);
  void Write32(Uint32 i);
  void Write16(Uint16 i);
  void Write8(Uint8 i);
  void WriteDouble(double d);
  void WriteString(const std::string& string);

protected:
  FILE *m_File;
};

#endif // _STD_IO_SERIALIZABLE_STREAM_H_