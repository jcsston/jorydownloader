using System;
using System.IO;
using System.Runtime.InteropServices;

namespace MPALib
{
  public enum MPAVersion
  {
    Invalid = -1,
    MPEG25 = 0,
    MPEGReserved,
    MPEG2,
    MPEG1
  } ;

  public enum MPALayer
  {
    Invalid = -1,
    Layer1 = 0,
    Layer2,
    Layer3,
    LayerReserved
  } ;

  public enum MPAChannel
  {
    Invalid = -1,
    Stereo = 0,
    JointStereo,
    DualChannel,
    SingleChannel,    
  };

  public enum MPAEmphasis
  {
    Invalid = -1,
    EmphNone = 0,
    Emph5015,
    EmphReserved,
    EmphCCITJ17
  };
  /// <summary>
  /// Summary description for MPAFrame.
  /// </summary>
  [ClassInterface(ClassInterfaceType.AutoDual)]
  public class MPAFrame
  {
    protected MPAVersion m_Version = MPAVersion.Invalid;
    protected MPALayer m_Layer = MPALayer.Invalid;
    protected MPAChannel m_Channels = MPAChannel.Invalid;
    protected MPAEmphasis m_Emphasis = MPAEmphasis.Invalid;
    protected bool m_bCRC = false;
    protected int m_BitRate = 0;
    protected int m_SamplingRate = 0;
    protected bool m_bValid = false;
    protected bool m_bLSF = false;
    protected bool m_bPadding = false;
    protected int m_PaddingSize = 0;
    protected bool m_bPrivate = false;
    protected bool m_bCopyright = false;
    protected bool m_bOriginal = false; 
    protected int m_FrameSize = 0;

    static public Int16 MPA_HEADER_SIZE = 4; // MPEG-Audio Header Size 32bit
    static Int32 [,]SamplingRates =  new Int32[4, 4] 
    { //  00,   01,   10, 11
      /*
      {44100,48000,32000, 0}, // v1
      {22050,24000,16000, 0}, // v2
      {11025,12000, 8000, 0}, // v2.5
      {    0,    0,    0, 0}  // reserved
      */
      {11025, 12000, 8000,  0},	// MPEG 2.5
      {0,     0,     0,     0},	// reserved
      {22050, 24000, 16000, 0},	// MPEG 2
      {44100, 48000, 32000, 0}		// MPEG 1
    };

    // bitrates:
    // 1. index = LSF,
    // 2. index = Layer
    // 3. index = bitrate index
    static Int16 [,,]Bitrates = new Int16[2, 3, 16]
    {
      { // MPEG 1
        { 0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, -1}, // Layer 1
        { 0, 32, 48, 56,  64,  80,  96, 112, 128, 160, 192, 224, 256, 320, 384, -1}, // Layer 2
        { 0, 32, 40, 48,  56,  64,  80,  96, 112, 128, 160, 192, 224, 256, 320, -1} // Layer 3
      },
      { // MPEG 2, 2.5
        { 0, 32, 48, 56,  64,  80,  96, 112, 128, 144, 160, 176, 192, 224, 256, -1}, // Layer 1
        { 0,  8, 16, 24,  32,  40,  48,  56,  64,  80,  96, 112, 128, 144, 160, -1}, // Layer 2
        { 0,  8, 16, 24,  32,  40,  48,  56,  64,  80,  96, 112, 128, 144, 160, -1} // Layer 3
      }
    };

    // Samples per Frame:
    // 1. index = LSF
    // 2. index = Layer
    static Int16 [,]SamplesPerFrames = new Int16[2, 3]
    {
      {	// MPEG 1
        384,	// Layer1
        1152,	// Layer2	
        1152	// Layer3
      },
      {	// MPEG 2, 2.5
        384,	// Layer1
        1152,	// Layer2
        576		// Layer3
      }	
    };

    // Samples per Frame / 8
    static Int16 [,] Coefficients = new Int16[2, 3]
    {
      {	// MPEG 1
        48,		// Layer1
        144,	// Layer2
        144		// Layer3
      },
      {	// MPEG 2, 2.5
        48,		// Layer1
        144,	// Layer2
        72		// Layer3
      }	
    };


    // needed later for CRC check
    // sideinformation size: 1.index = lsf, 2. index = layer, 3. index = mono
    static Int16 [,,] SideinfoSizes = new Int16[2, 3, 2]
    {
      {	// MPEG 1 (not mono, mono
        {0,0},	// Layer1
        {0,0},	// Layer2
        {9,17}	// Layer3
      },
      {	// MPEG 2, 2.5
        {0,0},	// Layer1
        {0,0},	// Layer2
        {17,32}	// Layer3
      }	
    };

    // padding sizes in bytes for different layers: 1. index = layer
    static Int16 [] PaddingSizes = new Int16[3]
    {
      4, // Layer1
      1, // Layer2
      1  // Layer3
    };

    /// <summary>
    /// The class constructor.
    /// </summary>
    public MPAFrame()
    {
      
    }

    /// <summary>
    /// Decode a mpa frame.
    /// </summary>
    /// <param name="Header">4 byte array that starts at the mpa frame sync code</param>
    /// <returns>Returns true if the frame was parsed correctly</returns>
    public bool DecodeHeader(byte [] Header) 
    {
      if (Header.Length < MPA_HEADER_SIZE) 
      {
        throw new MPAException("MPA Frame passed to MPAFrame.DecodeHeader() has invalid length! Expected " + MPA_HEADER_SIZE + " got " + Header.Length);
      }

      UInt32 header = Endian.BuildUInt32FromBytesBE(Header);

      // Double-Check SYNC bits (last eleven bits set)
      // The passed in header should have already been checked...
      if ((header >> 24 != 0xFF) || ((((header >> 16)) & 0xE0) != 0xE0)) 
      {
        //throw new MPAException("MPA Frame passed to MPAFrame.DecodeHeader() has invalid sync bytes!");
        return (m_bValid = false);
      }

      // get MPEG version
      m_Version = (MPAVersion)((header >> 19) & 0x03); // mask only the rightmost 2 bits
      if (m_Version == MPAVersion.MPEGReserved)
        return (m_bValid = false);

      if (m_Version == MPAVersion.MPEG1)
        m_bLSF = false;
      else
        m_bLSF = true;

      // get layer (0 = layer1, 2 = layer2, ...)
      int layer = (int)((header >> 17) & 0x03);
      m_Layer = (MPALayer)(3 - layer);	
      if (m_Layer == MPALayer.LayerReserved)
        return (m_bValid = false);

      // protection bit (inverted)
      m_bCRC = (((header >> 16) & 0x01) == 0);
      
      // sampling rate
      int samplingIndex = (int)((header >> 10) & 0x03);
      m_SamplingRate = SamplingRates[(int)m_Version, samplingIndex];

      if (m_SamplingRate == 0)
        return (m_bValid = false);

      byte bitrateIndex = (byte)((header >> 12) & 0x0F);
      if (bitrateIndex == 0x0) 
      {
        // all bits set is reserved
        return (m_bValid = false);
      }

      m_BitRate = Bitrates[m_bLSF ? 1 : 0, (int)m_Layer, bitrateIndex];

      if (m_BitRate == -1)
        return (m_bValid = false);

      m_bPadding = (((header >> 9) & 0x01) != 0);
      m_PaddingSize = (int)(PaddingSizes[(int)m_Layer] * (header >> 9) & 0x01);

      // calculate frame size
      int coefficient = Coefficients[m_bLSF ? 1 : 0, (int)m_Layer];
      m_FrameSize = (coefficient * (m_BitRate * 1000) / m_SamplingRate) + m_PaddingSize;

      m_bPrivate = (((header >> 8) & 0x01) != 0);
      m_bCopyright = (((header >> 3) & 0x01) != 0);
      m_bOriginal = (((header >> 2) & 0x01) != 0);

      m_Channels = (MPAChannel)((header >> 6) & 0x03);
      m_Emphasis = (MPAEmphasis)((header >> 0) & 0x03);

      return (m_bValid = true);
    }
    
    public bool Valid
    { 
      get 
      { 
        return m_bValid; 
      } 
    }
    
    public Int16 SampleSize
    {
      get
      {
        return SamplesPerFrames[m_bLSF ? 1 : 0, (int)m_Layer];
      }
    }
    
    public int SamplingRate 
    { 
      get 
      {
        return m_SamplingRate;
      }
    }
    
    public int FrameSize
    {
      get
      {
        return m_FrameSize;
      }
    }
    
    public int FrameDataSize
    {
      get
      {
        if (m_FrameSize > MPA_HEADER_SIZE) 
        {
          return m_FrameSize - MPA_HEADER_SIZE;
        }
        return 0;
      }
    }

    public int Channels 
    { 
      get 
      {
        return (m_Channels == MPAChannel.SingleChannel) ? 1 : 2;
      }
    }
    
    public bool CRC 
    { 
      get 
      {
        return m_bCRC;
      }
    }

    public bool LSF 
    { 
      get 
      {
        return m_bLSF;
      }
    }
    
    public MPAChannel ChannelMode 
    {
      get 
      {
        return m_Channels;
      }
    }
    
    public MPALayer Layer
    { 
      get 
      {
        return m_Layer; 
      }
    }
    
    public MPAEmphasis Emphasis 
    {
      get 
      {
        return m_Emphasis;
      }
    }

    public MPAVersion Version 
    {
      get 
      {
        return m_Version;
      }
    }

    
    public int BitRate 
    { 
      get 
      {
        return m_BitRate;
      }
    }
	
    public bool Private 
    { 
      get 
      {
        return m_bPrivate;
      }
    }
    
    public bool Padding 
    { 
      get 
      {
        return m_bPadding;
      }
    }

    public int PaddingSize 
    { 
      get 
      {
        return m_PaddingSize;
      }
    }

    
    public bool Copyright 
    { 
      get 
      {
        return m_bCopyright;
      }
    }
    
    public bool Original 
    { 
      get 
      {
        return m_bOriginal;
      }
    }
        

    public bool Compatible(MPAFrame frame) 
    {
      if (!frame.Valid)
        return false;

      if (m_SamplingRate != frame.SamplingRate)
        return false;

      if (m_Layer != frame.Layer)
        return false;

      if (Channels != frame.Channels)
        return false;

      return true;
    }
  }

  public class MPAInfoFrame
  {
    protected bool bIsLame;
    protected byte [] totalBuffer;
    protected MPAFrame frame;

    public MPAInfoFrame()
    {
      bIsLame = false;
    }
    public MPAInfoFrame(MPAFrame frame)
    {
      this.frame = frame;
    }

    public MPAFrame CurrentFrame
    {
      get
      {
        return frame;
      }
      set
      {
        frame = value;
      }
    }
    /// \brief read the frame data and parse them
    public virtual bool ParseFrame(Stream input) 
    {
      if (frame == null) 
      {
        throw new InvalidOperationException("No source MPAFrame set. Set CurrentFrame before calling ParseFrame.");
        //return false;
      }

      int size = (int)frame.FrameDataSize;
      totalBuffer = new byte[size];

      if (input.Read(totalBuffer, 0, size) != size)
      {
        return false;
      }

      int index = 0;
      //  determine offset of header
      if (frame.Version == MPAVersion.MPEG1)
      {
        // mpeg1
        if (frame.Layer == MPALayer.Layer3) 
        {
          index += 32;
        }
        else 
        {
          index += 17;
        }
      }
      else
      {
        // mpeg2
        if (frame.Layer == MPALayer.Layer3) 
        {
          index += 17;
        }
        else 
        {
          index += 9;
        }
      }

      if (totalBuffer[index+3] == 'g' 
        && totalBuffer[index+2] == 'n' 
        && totalBuffer[index+1] == 'i' 
        && totalBuffer[index+0] == 'X') // Xing frame
      {
        // this is a Xing frame
        bIsLame = false;
      }
      else if (totalBuffer[index+3] == 'o' 
        && totalBuffer[index+2] == 'f' 
        && totalBuffer[index+1] == 'n' 
        && totalBuffer[index+0] == 'I') // Lame frame
      {
        // this is a LAME frame
        bIsLame = true;
      }
      else
      {
        return false;
      }

      return true;
    }
  }

  public class MPALameFrame : MPAInfoFrame
  {
    public MPALameFrame()
    {
      
    }

    public override bool ParseFrame(Stream input) 
    {
      if (!base.ParseFrame(input))
        return false;

      if (bIsLame)
      {
      }

      return true;
    }
  }
}
