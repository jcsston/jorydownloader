using System;

namespace WavLib
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class WavLibSample
  {
    static void Usage()
    {
      Console.Out.WriteLine("Usage:");
      Console.Out.WriteLine("   WavLibSample <filename>");
    }

		static void Main(string[] args)
		{
      Console.Out.WriteLine("WavLibSample v1.0 - (c) 2004 Jory Stone <wavlib@jory.info>");
      if (args.Length == 0) 
      {
        Usage();
        return;
      }
			WriteFile(args[0]);
		}

		static void WriteFile(string filename)
		{
      Console.Out.WriteLine("Writing to: " + filename);
		  WavLib.WaveFormatEx wfx = new WavLib.WaveFormatEx();
       wfx.wFormatTag = 0x55; // MP3
       wfx.nSamplesPerSec = 44100;
       wfx.nChannels = 1;
       wfx.wBitsPerSample = 16;
       wfx.nBlockAlign = 4;

       WavLib.WavWriter writer = new WavLib.WavWriter();
       writer.Open(filename, wfx);
       sbyte [] data = new sbyte[80];
      writer.WriteSampleData(ref data, 0, data.Length);
      writer.Close();
    }
	}
}
