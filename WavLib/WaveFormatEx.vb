<System.Runtime.InteropServices.ClassInterface(Runtime.InteropServices.ClassInterfaceType.AutoDual)> _
Public Class WaveFormatEx
    Public Shared WAVE_FORMAT_PCM As Integer = &H1
    Public Shared WAVE_FORMAT_ADPCM As Integer = &H2
    Public Shared WAVE_FORMAT_ALAW As Integer = &H6
    Public Shared WAVE_FORMAT_MULAW As Integer = &H7
    Public Shared WAVE_FORMAT_OKI_ADPCM As Integer = &H10
    Public Shared WAVE_FORMAT_DIGISTD As Integer = &H15
    Public Shared WAVE_FORMAT_DIGIFIX As Integer = &H16
    Public Shared WAVE_FORMAT_GSM610 As Integer = &H31
    Public Shared WAVE_FORMAT_IBM_MULAW As Integer = &H101
    Public Shared WAVE_FORMAT_IBM_ALAW As Integer = &H102
    Public Shared WAVE_FORMAT_IBM_ADPCM As Integer = &H103
    Public Shared WAVE_FORMAT_DVI_ADPCM As Integer = &H11
    Public Shared WAVE_FORMAT_SX7383 As Integer = &H1C07
    Public Shared WAVE_FORMAT_DSPGROUP_TRUESPEECH As Integer = &H22

    Public Shared WAVE_FORMAT_MSNAUDIO As Integer = &H32
    Public Shared WAVE_FORMAT_MSG723 As Integer = &H42
    Public Shared WAVE_FORMAT_MPEG_LAYER12 As Integer = &H50
    Public Shared WAVE_FORMAT_MPEG_LAYER3 As Integer = &H55
    Public Shared WAVE_FORMAT_VOXWARE_AC8 As Integer = &H70
    Public Shared WAVE_FORMAT_VOXWARE_AC10 As Integer = &H71
    Public Shared WAVE_FORMAT_VOXWARE_AC16 As Integer = &H72
    Public Shared WAVE_FORMAT_VOXWARE_AC20 As Integer = &H73
    Public Shared WAVE_FORMAT_VOXWARE_METAVOICE As Integer = &H74
    Public Shared WAVE_FORMAT_VOXWARE_METASOUND As Integer = &H75
    Public Shared WAVE_FORMAT_VOXWARE_RT29H As Integer = &H76
    Public Shared WAVE_FORMAT_VOXWARE_VR12 As Integer = &H77
    Public Shared WAVE_FORMAT_VOXWARE_VR18 As Integer = &H78
    Public Shared WAVE_FORMAT_VOXWARE_TQ40 As Integer = &H79
    Public Shared WAVE_FORMAT_VOXWARE_TQ60 As Integer = &H81
    Public Shared WAVE_FORMAT_MSRT24 As Integer = &H82

    Public wFormatTag As Int16
    Public nChannels As Int16
    Public nSamplesPerSec As Int32
    Public nAvgBytesPerSec As Int32
    Public nBlockAlign As Int16
    Public wBitsPerSample As Int16
    Public cbSize As Int16
    Public cbData As Byte()

    Public Function GetFormatTagString() As String
        GetFormatTagString = "Unknown (" + wFormatTag + ")"
        Select Case wFormatTag
            Case &H0 : GetFormatTagString = "Microsoft Unknown Wave Format"
            Case &H1 : GetFormatTagString = "Microsoft PCM Format"
            Case &H2 : GetFormatTagString = "Microsoft ADPCM Format"
            Case &H3 : GetFormatTagString = "IEEE Float"
            Case &H4 : GetFormatTagString = "Compaq Computer VSELP (codec for Windows CE 2.0 devices)"
            Case &H5 : GetFormatTagString = "IBM CVSD"
            Case &H6 : GetFormatTagString = "Microsoft ALAW (CCITT A-Law)"
            Case &H7 : GetFormatTagString = "Microsoft MULAW (CCITT u-Law)"
            Case &H8 : GetFormatTagString = "Microsoft DTS"
            Case &H10 : GetFormatTagString = "OKI ADPCM"
            Case &H11 : GetFormatTagString = "Intel DVI ADPCM (IMA ADPCM)"
            Case &H12 : GetFormatTagString = "Videologic MediaSpace ADPCM"
            Case &H13 : GetFormatTagString = "Sierra Semiconductor ADPCM"
            Case &H14 : GetFormatTagString = "Antex Electronics G.723 ADPCM"
            Case &H15 : GetFormatTagString = "DSP Solutions DIGISTD"
            Case &H16 : GetFormatTagString = "DSP Solutions DIGIFIX"
            Case &H17 : GetFormatTagString = "Dialogic OKI ADPCM"
            Case &H18 : GetFormatTagString = "MediaVision ADPCM"
            Case &H19 : GetFormatTagString = "HP CU codec"
            Case &H20 : GetFormatTagString = "Yamaha ADPCM"
            Case &H21 : GetFormatTagString = "Speech Compression SONARC"
            Case &H22 : GetFormatTagString = "DSP Group True Speech"
            Case &H23 : GetFormatTagString = "Echo Speech EchoSC1"
            Case &H24 : GetFormatTagString = "Audiofile AF36"
            Case &H25 : GetFormatTagString = "APTX"
            Case &H26 : GetFormatTagString = "AudioFile AF10"
            Case &H27 : GetFormatTagString = "Prosody 1612 codec for CTI Speech Card"
            Case &H28 : GetFormatTagString = "Merging Technologies S.A. LRC"
            Case &H30 : GetFormatTagString = "Dolby Labs AC2"
            Case &H31 : GetFormatTagString = "Microsoft GSM 6.10"
            Case &H32 : GetFormatTagString = "MSNAudio"
            Case &H33 : GetFormatTagString = "Antex Electronics ADPCME"
            Case &H34 : GetFormatTagString = "Control Resources VQLPC"
            Case &H35 : GetFormatTagString = "DSP Solutions Digireal"
            Case &H36 : GetFormatTagString = "DSP Solutions DigiADPCM"
            Case &H37 : GetFormatTagString = "Control Resources Ltd CR10"
            Case &H38 : GetFormatTagString = "Natural MicroSystems VBXADPCM"
            Case &H39 : GetFormatTagString = "Roland RDAC (Crystal Semiconductor IMA ADPCM)"
            Case &H3A : GetFormatTagString = "Echo Speech EchoSC3"
            Case &H3B : GetFormatTagString = "Rockwell ADPCM"
            Case &H3C : GetFormatTagString = "Rockwell Digit LK"
            Case &H3D : GetFormatTagString = "Xebec Multimedia Solutions"
            Case &H40 : GetFormatTagString = "Antex Electronics G.721 ADPCM"
            Case &H41 : GetFormatTagString = "Antex Electronics G.728 CELP"
            Case &H42 : GetFormatTagString = "Microsoft MSG723"
            Case &H43 : GetFormatTagString = "IBM AVC ADPCM"
            Case &H50 : GetFormatTagString = "Microsoft MPEG-1 layer 1, 2"
            Case &H52 : GetFormatTagString = "InSoft RT24 (ACM codec is an alternative codec)"
            Case &H53 : GetFormatTagString = "InSoft PAC"
            Case &H55 : GetFormatTagString = "MPEG-1 Layer 3 (MP3)"
            Case &H59 : GetFormatTagString = "Lucent G.723"
            Case &H60 : GetFormatTagString = "Cirrus Logic"
            Case &H61 : GetFormatTagString = "ESS Technology ESPCM / Duck DK4 ADPCM"
            Case &H62 : GetFormatTagString = "Voxware file-mode codec / Duck DK3 ADPCM"
            Case &H63 : GetFormatTagString = "Canopus Atrac"
            Case &H64 : GetFormatTagString = "APICOM G.726 ADPCM"
            Case &H65 : GetFormatTagString = "APICOM G.722 ADPCM"
            Case &H66 : GetFormatTagString = "Microsoft DSAT"
            Case &H67 : GetFormatTagString = "Microsoft DSAT Display"
            Case &H69 : GetFormatTagString = "Voxware Byte Aligned (bitstream-mode codec)"
            Case &H70 : GetFormatTagString = "Voxware AC8 (Lernout & Hauspie CELP 4.8 kbps)"
            Case &H71 : GetFormatTagString = "Voxware AC10 (Lernout & Hauspie CBS 8kbps)"
            Case &H72 : GetFormatTagString = "Voxware AC16 (Lernout & Hauspie CBS 12kbps)"
            Case &H73 : GetFormatTagString = "Voxware AC20 (Lernout & Hauspie CBS 16kbps)"
            Case &H74 : GetFormatTagString = "Voxware MetaVoice (file and stream oriented)"
            Case &H75 : GetFormatTagString = "Voxware MetaSound (file and stream oriented)"
            Case &H76 : GetFormatTagString = "Voxware RT29HW"
            Case &H77 : GetFormatTagString = "Voxware VR12"
            Case &H78 : GetFormatTagString = "Voxware VR18"
            Case &H79 : GetFormatTagString = "Voxware TQ40"
            Case &H80 : GetFormatTagString = "Softsound"
            Case &H81 : GetFormatTagString = "Voxware TQ60"
            Case &H82 : GetFormatTagString = "Microsoft MSRT24 (ACM codec is an alternative codec)"
            Case &H83 : GetFormatTagString = "AT&T Labs G.729A"
            Case &H84 : GetFormatTagString = "Motion Pixels MVI MV12"
            Case &H85 : GetFormatTagString = "DataFusion Systems G.726"
            Case &H86 : GetFormatTagString = "DataFusion Systems GSM610"
            Case &H88 : GetFormatTagString = "Iterated Systems ISIAudio"
            Case &H89 : GetFormatTagString = "Onlive"
            Case &H91 : GetFormatTagString = "Siemens Business Communications SBC24"
            Case &H92 : GetFormatTagString = "Sonic Foundry Dolby AC3 SPDIF"
            Case &H93 : GetFormatTagString = "MediaSonic G.723"
            Case &H94 : GetFormatTagString = "Aculab 8KBPS"
            Case &H97 : GetFormatTagString = "ZyXEL ADPCM"
            Case &H98 : GetFormatTagString = "Philips LPCBB"
            Case &H99 : GetFormatTagString = "Studer Professional Audio AG Packed"
            Case &HA0 : GetFormatTagString = "Malden Electronics PHONYTALK"
            Case &HFF : GetFormatTagString = "AAC"
            Case &H100 : GetFormatTagString = "Rhetorex ADPCM"
            Case &H101 : GetFormatTagString = "BeCubed Software IRAT"
            Case &H111 : GetFormatTagString = "Vivo G.723"
            Case &H112 : GetFormatTagString = "Vivo Siren"
            Case &H123 : GetFormatTagString = "Digital G.723"
            Case &H125 : GetFormatTagString = "Sanyo ADPCM"
            Case &H130 : GetFormatTagString = "Sipro Lab Telecom ACELP.net"
            Case &H131 : GetFormatTagString = "Sipro Lab Telecom ACELP.4800"
            Case &H132 : GetFormatTagString = "Sipro Lab Telecom ACELP.8V3"
            Case &H133 : GetFormatTagString = "Sipro Lab Telecom ACELP.G.729"
            Case &H134 : GetFormatTagString = "Sipro Lab Telecom ACELP.G.729A"
            Case &H135 : GetFormatTagString = "Sipro Lab Telecom ACELP.KELVIN"
            Case &H140 : GetFormatTagString = "Dictaphone G.726 ADPCM"
            Case &H150 : GetFormatTagString = "Qualcomm PUREVOICE"
            Case &H151 : GetFormatTagString = "Qualcomm HALFRATE"
            Case &H155 : GetFormatTagString = "Ring Zero Systems TUBGSM"
            Case &H160 : GetFormatTagString = "Windows Media Audio V1 / DivX audio (WMA)"
            Case &H161 : GetFormatTagString = "Windows Media Audio V2 / DivX audio (WMA) / Alex AC3 Audio"
            Case &H200 : GetFormatTagString = "Creative Labs ADPCM"
            Case &H202 : GetFormatTagString = "Creative Labs FastSpeech8"
            Case &H203 : GetFormatTagString = "Creative Labs FastSpeech10"
            Case &H210 : GetFormatTagString = "UHER informatic GmbH ADPCM"
            Case &H216 : GetFormatTagString = "Ulead DV ACM"
            Case &H220 : GetFormatTagString = "Quarterdeck"
            Case &H230 : GetFormatTagString = "I-link Worldwide ILINK VC"
            Case &H240 : GetFormatTagString = "Aureal Semiconductor RAW SPORT"
            Case &H250 : GetFormatTagString = "Interactive Products HSX"
            Case &H251 : GetFormatTagString = "Interactive Products RPELP"
            Case &H260 : GetFormatTagString = "Consistent Software CS2"
            Case &H270 : GetFormatTagString = "Sony ATRAC3 (SCX, same as MiniDisk LP2)"
            Case &H300 : GetFormatTagString = "Fujitsu TOWNS SND"
            Case &H400 : GetFormatTagString = "BTV Digital (Brooktree digital audio format)"
            Case &H401 : GetFormatTagString = "Intel Music Coder (IMC)"
            Case &H450 : GetFormatTagString = "QDesign Music"
            Case &H680 : GetFormatTagString = "AT&T Labs VME VMPCM"
            Case &H681 : GetFormatTagString = "AT&T Labs TPC"
            Case &H1000 : GetFormatTagString = "Olivetti GSM"
            Case &H1001 : GetFormatTagString = "Olivetti ADPCM"
            Case &H1002 : GetFormatTagString = "Olivetti CELP"
            Case &H1003 : GetFormatTagString = "Olivetti SBC"
            Case &H1004 : GetFormatTagString = "Olivetti OPR"
            Case &H1100 : GetFormatTagString = "Lernout & Hauspie codec"
            Case &H1101 : GetFormatTagString = "Lernout & Hauspie CELP codec"
            Case &H1102 : GetFormatTagString = "Lernout & Hauspie SBC codec"
            Case &H1103 : GetFormatTagString = "Lernout & Hauspie SBC codec"
            Case &H1104 : GetFormatTagString = "Lernout & Hauspie SBC codec"
            Case &H1400 : GetFormatTagString = "Norris Communication"
            Case &H1401 : GetFormatTagString = "ISIAudio"
            Case &H1500 : GetFormatTagString = "AT&T Labs Soundspace Music Compression"
            Case &H181C : GetFormatTagString = "VoxWare RT24 speech codec"
            Case &H1FC4 : GetFormatTagString = "NTC ALF2CD ACM"
            Case &H2000 : GetFormatTagString = "FAST Multimedia AG DVM (Dolby AC3)"
            Case &H674F : GetFormatTagString = "Ogg Vorbis (mode 1)"
            Case &H676F : GetFormatTagString = "Ogg Vorbis (mode 1+)"
            Case &H6750 : GetFormatTagString = "Ogg Vorbis (mode 2)"
            Case &H6770 : GetFormatTagString = "Ogg Vorbis (mode 2+)"
            Case &H6751 : GetFormatTagString = "Ogg Vorbis (mode 3)"
            Case &H6771 : GetFormatTagString = "Ogg Vorbis (mode 3+)"
        End Select
    End Function

    Public Function GetSize() As Integer
        GetSize = 2 + 2 + 4 + 4 + 2
        If Me.wFormatTag = WAVE_FORMAT_PCM Then
            ' Bits Per Sample (per channel)
            GetSize += 2
        End If
        If Me.cbSize > 0 Then
            GetSize += Me.cbSize
        End If
    End Function

    Public Sub Read(ByVal reader As IO.Stream, ByVal size As Integer)
        Dim binReader As IO.BinaryReader
        binReader = New IO.BinaryReader(reader)
        Me.Read(binReader, size)
    End Sub

    Public Sub Read(ByVal reader As IO.BinaryReader, ByVal size As Integer)
        Me.wFormatTag = reader.ReadInt16()
        ' Channel count
        Me.nChannels = reader.ReadInt16()
        ' Sample Rate
        Me.nSamplesPerSec = reader.ReadInt32()
        ' Bytes Per Second
        Me.nAvgBytesPerSec = reader.ReadInt32()
        ' Bytes Per Sample (all chanels)
        Me.nBlockAlign = reader.ReadInt16()
        If Me.wFormatTag = WAVE_FORMAT_PCM Then
            ' Bits Per Sample (per channel)
            Me.wBitsPerSample = reader.ReadInt16()
        End If
        If size > GetSize() Then
            Me.cbSize = reader.ReadInt16()
            ReDim Me.cbData(Me.cbSize)
            reader.Read(Me.cbData, 0, Me.cbSize)
        End If
    End Sub

    Public Sub Write(ByVal writer As IO.Stream)
        Dim binWriter As IO.BinaryWriter
        binWriter = New IO.BinaryWriter(writer)
        Me.Write(binWriter)
        binWriter.Flush()
    End Sub

    Public Sub Write(ByVal writer As IO.BinaryWriter)
        writer.Write(Me.wFormatTag)
        ' Channel count
        writer.Write(Me.nChannels)
        ' Sample Rate
        writer.Write(Me.nSamplesPerSec)
        ' Bytes Per Second
        writer.Write(Me.nAvgBytesPerSec)
        ' Bytes Per Sample (all chanels)
        writer.Write(Me.nBlockAlign)
        If Me.wFormatTag = WAVE_FORMAT_PCM Then
            ' Bits Per Sample (per channel)
            writer.Write(Me.wBitsPerSample)
        End If
        If Me.cbSize > 0 Then
            writer.Write(Me.cbSize)
            writer.Write(Me.cbData, 0, Me.cbSize)
        End If
    End Sub
End Class
