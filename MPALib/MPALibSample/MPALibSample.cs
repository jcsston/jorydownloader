using System;
using System.IO;

namespace MPALib
{
	/// <summary>
	/// Summary description for Class1.
	/// </summary>
	class MPALibSample
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main(string[] args)
		{
      Console.WriteLine("MPALibSample - (c) 2004 Jory Stone");
      Console.WriteLine("Under the GNU LGPL License");
		
      string filename = args[0];
      bool bVerbose = false;

      if (bVerbose)
        Console.WriteLine("In Verbose Mode, prepare to be blasted with numbers");

      Stream inputStream = new FileStream(filename, FileMode.Open, FileAccess.Read);
      // open the input file		
      MPAReader inputMPA = new MPAReader(inputStream);
    
      // prepare to read the whole file for statistics
      if (bVerbose)
      {
        Console.WriteLine("Reading statistics in "+ filename);
      }

      inputMPA.SkipToFrameStart();

      int FrameNumber = 0;
      long SampleNumbers = 0;
      long TotalFrameSize = 0;
      MPAFrame frame = new MPAFrame();
    
      while (inputMPA.ReadNextFrame(true)) 
      {
        FrameNumber++;
        frame = inputMPA.CurrentFrame;
        /// \todo handle the bitrate of the frame in a key/value table, so that we can detect real CBR files
        TotalFrameSize += (long)frame.FrameSize;
        SampleNumbers += (long)frame.SampleSize;        
      }

      long totalByteSize = TotalFrameSize * frame.SamplingRate;
      long AverageBitrate, AverageByterate;
      if ((8 * totalByteSize) % SampleNumbers > SampleNumbers>>1)
        AverageBitrate = 1 + 8 * totalByteSize / SampleNumbers;
      else
        AverageBitrate = 8 * totalByteSize / SampleNumbers;

      if (AverageBitrate % 8 > 4)
        AverageByterate = 1 + (AverageBitrate >> 3);
      else
        AverageByterate = AverageBitrate >> 3;

      double Duration_total = (((double)SampleNumbers) / frame.SamplingRate);

      Console.WriteLine("Sampling Rate: " + frame.SamplingRate);
      Console.WriteLine("Sample Size: " + frame.SampleSize);
      Console.WriteLine("Layer: " + frame.Layer);
      Console.WriteLine("Channel Count: " + frame.Channels);
      Console.WriteLine("Total Frame Size: " + TotalFrameSize);
      Console.WriteLine("Sample Count: " + SampleNumbers);
      Console.WriteLine("Average Bitrate: " + (AverageByterate * 8));
      Console.WriteLine("Duration: " + Duration_total);
		}
	}
}
