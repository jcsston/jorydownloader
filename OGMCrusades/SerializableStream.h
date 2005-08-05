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

#ifndef _SERIALIZABLESTREAM_H_
#define _SERIALIZABLESTREAM_H_

#include <inttypes.h>
#include <string>

class SerializableStream
{
public:
  SerializableStream();
  virtual ~SerializableStream();

  virtual uint64_t Read64() = 0;
  virtual uint32_t Read32() = 0;
  virtual uint16_t Read16() = 0;
  virtual uint8_t Read8() = 0;
  virtual double ReadDouble() = 0;
  virtual std::string ReadString() = 0;

  virtual void Write64(uint64_t i) = 0;
  virtual void Write32(uint32_t i) = 0;
  virtual void Write16(uint16_t i) = 0;
  virtual void Write8(uint8_t i) = 0;
  virtual void WriteDouble(double d) = 0;
  virtual void WriteString(const std::string& string) = 0;
};

#endif // _SERIALIZABLESTREAM_H_
