using System;

namespace MPALib
{
	/// <summary>
	/// Summary description for Endian.
	/// </summary>
	public class Endian
	{
    static public UInt32 BuildUInt32FromBytesLE(byte [] buffer) 
    {
      return ((UInt32)(byte)(buffer[0]) 
        | ((UInt32)(byte)(buffer[1]) << 8) 
        | ((UInt32)(byte)(buffer[2]) << 16) 
        | ((UInt32)(byte)(buffer[3]) << 24));
    }
    static public UInt32 BuildUInt32FromBytesBE(byte [] buffer) 
    {
      return ((UInt32)(byte)(buffer[3]) 
        | ((UInt32)(byte)(buffer[2]) << 8) 
        | ((UInt32)(byte)(buffer[1]) << 16) 
        | ((UInt32)(byte)(buffer[0]) << 24));
    }
	}
}
