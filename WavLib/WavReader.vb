Imports System.ComponentModel
Imports System.Runtime.InteropServices

<ClassInterface(Runtime.InteropServices.ClassInterfaceType.AutoDual)> _
Public Class WavReader
    Public Shared FOURCC_RIFF As Int32 = &H46464952 ' "RIFF"
    Public Shared FOURCC_WAVE As Int32 = &H45564157 ' "WAVE"
    Public Shared FOURCC_FMT As Int32 = &H20746D66  ' "fmt "
    Public Shared FOURCC_FACT As Int32 = &H74636166 ' "fact"
    Public Shared FOURCC_DATA As Int32 = &H61746164 ' "data"

    Private m_Reader As IO.BinaryReader

    Private m_DataChunkStart As Integer
    Private m_DataChunkSize As Integer
    Private m_FactChunkSampleCount As Integer
    Private m_Wfx As WaveFormatEx

    <DescriptionAttribute("Get the sample count of the wav file.")> _
    ReadOnly Property SampleCount() As Integer
        Get
            Return m_FactChunkSampleCount
        End Get
    End Property

    <DescriptionAttribute("Get the WaveFormatEx of the wav file.")> _
    ReadOnly Property WaveFormatExHeader() As WaveFormatEx
        Get
            Return m_Wfx
        End Get
    End Property

    Public Sub New()
        Init()
    End Sub

    Private Sub Init()
        m_Reader = Nothing
        m_Wfx = Nothing
        m_DataChunkStart = 0
        m_DataChunkSize = 0
        m_FactChunkSampleCount = 0
    End Sub

    <DescriptionAttribute("Close the .wav file.")> _
    Public Sub Close()
        If Not (m_Reader Is Nothing) Then
            m_Reader.Close()
            m_Reader = Nothing
        End If
        Init()
    End Sub

    Public Sub Open(ByVal filename As String)
        Close()
        Dim file As IO.Stream
        file = New IO.FileStream(filename, IO.FileMode.Open)
        m_Reader = New IO.BinaryReader(file)
        ParseHeaders()
    End Sub

    Private Sub ParseHeaders()
        Dim header As Int32
        Dim size As Int32

        ' Start at the beginning of the file
        m_Reader.BaseStream.Position = 0

        ' Read the RIFF header
        header = m_Reader.ReadInt32()
        size = m_Reader.ReadInt32()
        ' Check if we are reading a RIFF file at least
        If Not header = FOURCC_RIFF Then
            Throw New NotSupportedException("Invalid WAV File. No RIFF structure.")
        End If

        header = m_Reader.ReadInt32()
        If Not header = FOURCC_WAVE Then
            Throw New NotSupportedException("Invalid WAV File. No WAVE structure.")
        End If

        ' Find all the chunks we are interested in
        While (m_Reader.BaseStream.Position < m_Reader.BaseStream.Length)
            header = m_Reader.ReadInt32()
            size = m_Reader.ReadInt32()
            Select Case header
                Case FOURCC_FMT
                    m_Wfx = New WaveFormatEx
                    m_Wfx.Read(m_Reader, size)
                Case FOURCC_FACT
                    If size >= 4 Then
                        m_FactChunkSampleCount = m_Reader.ReadInt32()
                    End If
                Case FOURCC_DATA
                    m_DataChunkStart = m_Reader.BaseStream.Position
          m_DataChunkSize = size
          ' Skip this chunk
          m_Reader.BaseStream.Position += size

                Case Else
                    ' Skip this chunk
                    m_Reader.BaseStream.Position += size
            End Select
        End While
        ' Check that we got fmt and data chunks
        If m_Wfx Is Nothing Then
            Throw New NotSupportedException("Invalid WAV File. No fmt chunk found.")
        End If
        If m_DataChunkStart = 0 Then
            Throw New NotSupportedException("Invalid WAV File. No data chunk found.")
        End If
        Seek(0)
    End Sub

    Public Sub Seek(ByVal pos As Integer)
        If pos > m_DataChunkSize Then
            Throw New ArgumentOutOfRangeException("Position is past EOS")
        End If
        m_Reader.BaseStream.Position = m_DataChunkStart + pos
    End Sub

    Private Sub CheckReaderState()
        If m_DataChunkStart = 0 Then
            Throw New InvalidOperationException("Invalid data chunk position.")
        End If
        If m_DataChunkSize = 0 Then
            Throw New InvalidOperationException("Invalid data chunk size.")
        End If
        If m_Reader.BaseStream.Position < m_DataChunkStart Then
            Throw New InvalidOperationException("File pointer is before data chunk")
        End If
        If m_Reader.BaseStream.Position > (m_DataChunkStart + m_DataChunkSize) Then
            Throw New InvalidOperationException("File pointer is past data chunk")
        End If
    End Sub

    Public Function ReadSamples(ByRef buffer() As Byte, ByVal offset As Integer, ByVal count As Integer) As Integer
        CheckReaderState()
        Return m_Reader.Read(buffer, offset, count)
    End Function
End Class
