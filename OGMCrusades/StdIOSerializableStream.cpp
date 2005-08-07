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

#include "StdIOSerializableStream.h"

StdIOSerializableStream::StdIOSerializableStream(const std::string &filename, bool bWritable)
{
  if (bWritable) {
    m_File = fopen(filename.c_str(), "wb");
  } else {
    m_File = fopen(filename.c_str(), "rb");
  }
  fseek(m_File, 0, SEEK_SET);
}

StdIOSerializableStream::~StdIOSerializableStream()
{
  if (m_File != NULL) {
    fclose(m_File);
    m_File = NULL;
  }
}

Uint64 StdIOSerializableStream::Read64()
{
  Uint64 i = 0;
  fread(&i, sizeof(i), 1, m_File);
  return i;
}

Uint32 StdIOSerializableStream::Read32()
{
  Uint32 i = 0;
  fread(&i, sizeof(i), 1, m_File);
  return i;
}

Uint16 StdIOSerializableStream::Read16()
{
  Uint16 i = 0;
  fread(&i, sizeof(i), 1, m_File);
  return i;
}

Uint8 StdIOSerializableStream::Read8()
{
  Uint8 i = 0;
  fread(&i, sizeof(i), 1, m_File);
  return i;
}

double StdIOSerializableStream::ReadDouble()
{
  double d = 0;
  fread(&d, sizeof(d), 1, m_File);
  return d;
}

std::string StdIOSerializableStream::ReadString()
{
  Uint32 strLen = Read32();
  std::string s;
  s.resize(strLen);
  fread((void *)s.c_str(), strLen, 1, m_File);
  return s;
}

void StdIOSerializableStream::Write64(Uint64 i)
{
  fwrite(&i, sizeof(i), 1, m_File);  
}

void StdIOSerializableStream::Write32(Uint32 i)
{
  fwrite(&i, sizeof(i), 1, m_File);
}

void StdIOSerializableStream::Write16(Uint16 i)
{
  fwrite(&i, sizeof(i), 1, m_File);
}

void StdIOSerializableStream::Write8(Uint8 i)
{
  fwrite(&i, sizeof(i), 1, m_File);
}

void StdIOSerializableStream::WriteDouble(double d)
{
  fwrite(&d, sizeof(d), 1, m_File);
}

void StdIOSerializableStream::WriteString(const std::string& string)
{
  Write32(string.length()+1);
  fwrite(string.c_str(), string.length()+1, 1, m_File);
}
