using System;
using System.IO;
using System.Collections;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace MPALib
{
	/// <summary>
	/// Summary description for MPAReader.
	/// </summary>
	[ClassInterface(ClassInterfaceType.AutoDual)]
	public class MPAReader
	{
    protected Stream m_Input = null;
    protected MPAFrame m_Frame = new MPAFrame();
    protected byte [] m_FrameData = new byte[0];

    // MPEG-I/II audio header is 32 bits in big endian
    protected byte [] Header = new byte[4];
    
    ArrayList m_BitrateTable = new ArrayList(); // bitrate/frame count
    protected int m_AverageBitrate = 0;

    protected int m_FrameNumber = 0;
    protected long m_TotalBitRate = 0;

    protected void ClearHeader() 
    {
      Header[0] = 0;
      Header[1] = 0;
      Header[2] = 0;
      Header[3] = 0;
    }

    protected void SkipID3v2Tag() 
    {
      m_Input.Position = 0; // restart from the beggining of the file
      m_Input.Position += 6;

      UInt32 Size = 0;
      
      // Read Extended Header Size
      byte [] buffer = new byte[1];
      if (m_Input.Read(buffer, 0, 1) != 1)
        return;
      Size |= ((UInt32)buffer[0]) << 21;

      // Read Frame size
      if (m_Input.Read(buffer, 0, 1) != 1)
        return;
      Size |= ((UInt32)buffer[0]) << 14;

      // Read Padding size
      if (m_Input.Read(buffer, 0, 1) != 1)
        return;
      Size |= ((UInt32)buffer[0]) << 7;

      // Read Footer size
      if (m_Input.Read(buffer, 0, 1) != 1)
        return;
      Size |= ((UInt32)buffer[0]) << 0;

      // Now we can skip over the ID3v2 tag junk
      m_Input.Position += Size;
    }

    public MPAReader()
    {

    }

		public MPAReader(Stream input)
		{
      Open(input);
		}

    public MPAReader(string filename)
    {
      Open(filename);
    }

    public void Open(Stream input)
    {
      m_Input = input;
      ResetFrameStats();
    }

    public void Open(string filename)
    {
      m_Input = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.Read);
      ResetFrameStats();
    }

    protected void ResetFrameStats()
    {
      ClearHeader();
      // Reset the frame stats
      m_BitrateTable.Clear();
      m_FrameNumber = 0;
      m_TotalBitRate = 0;
    }

    /// <summary>
    /// Skip from the beggining of the file to the first valid frame
    /// </summary>
    /// <returns></returns>
    public bool SkipToFrameStart() 
    {
      ResetFrameStats();
      m_Input.Position = 0;
      
      if (m_Input.Read(Header, 0, 3) != 3)
        return false;

      if (Header[0] == 'I' && Header[1] == 'D' && Header[2] == '3')
      {
        SkipID3v2Tag();
      }
      else
      {
        m_Input.Position = 0;
      }

      return true;
    }

    /// \brief Find the next syncword in the file
    protected bool FindNextSyncWord() 
    {
      do 
      {
        Header[0] = Header[1];
        Header[1] = Header[2];
        Header[2] = Header[3];
        if (m_Input.Read(Header, 3, 1) != 1)
          break;

        UInt32 myHeaderValue = Endian.BuildUInt32FromBytesBE(Header);

        if ((myHeaderValue >> 21) == (0xFFFFFFFF >> 21))
        {
          // this is a valid syncword
          return true;
        }
      } while (true);

      return false;
    }

    /// <summary>
    /// Read in the next frame.
    /// </summary>
    public void ReadNextFrame() 
    {
      ReadNextFrame(false);
    }

    /// <summary>
    /// Read in the next frame
    /// </summary>
    /// <param name="bSkipData">Skip the frame data</param>
    public bool ReadNextFrame(bool bSkipData)
    {
      while (!m_Frame.DecodeHeader(Header))
      {
        if (!FindNextSyncWord()) 
        {
          // EOS?
          return false;
        }
      } 
      
      m_BitrateTable.Add(m_Frame.BitRate);
      m_FrameNumber++;
      m_TotalBitRate += m_Frame.BitRate;

      if (bSkipData) 
      {
        SkipFrameData();
      } 
      else 
      {
        if (m_FrameData.Length == m_Frame.FrameDataSize) 
        {
          m_FrameData = new byte[m_Frame.FrameDataSize];
        }
        ReadFrameData(m_FrameData);
      }
      return true;
    }

    /// <summary>
    /// Get the current frame.
    /// </summary>
    /// <returns></returns>
    public MPAFrame CurrentFrame 
    {
      get 
      {
        return m_Frame;
      }
    }

    /// <summary>
    /// Get the current frame data.
    /// </summary>
    /// <returns></returns>
    public byte [] CurrentFrameData 
    {
      get 
      {
        return m_FrameData;
      }
    }

    /// <summary>
    /// Skip the frame data. Useful to read file stats.
    /// </summary>
    protected void SkipFrameData() 
    {
      Debug.Assert(m_Frame.Valid);

      m_Input.Position += m_Frame.FrameDataSize;
      ClearHeader();
    }

    /// <summary>
    /// Actually read the frame data.
    /// </summary>
    /// <param name="buffer">
    /// Buffer to store the frame data in. It must be larger than 
    /// the MPAReader.GetFrame().FrameDataSize value.
    /// </param>
    protected void ReadFrameData(byte [] buffer) 
    {
      Debug.Assert(m_Frame.Valid);
      
      m_Input.Read(buffer, 0, m_Frame.FrameDataSize);
      ClearHeader();
    }

    public bool CBR 
    {
      get 
      {
        return (m_BitrateTable.Count == 1);
      }
    }

    public int AverageBitRate 
    {
      get 
      {
        return (int)m_AverageBitrate;
      }
    }

    public void UpdateAverageBitrate() 
    {      
      if (this.CBR) 
      {
        int bitrate = (int)m_BitrateTable[0];
        m_AverageBitrate = (int)bitrate * 1000;
      }
      else
      {
        m_AverageBitrate = (int)((m_TotalBitRate * 1000) / m_FrameNumber);
      }
    }
	}
}
