Imports System.ComponentModel
Imports System.Runtime.InteropServices

<ClassInterface(Runtime.InteropServices.ClassInterfaceType.AutoDual)> _
Public Class WavWriter
    Private m_Writer As IO.BinaryWriter
    Private m_HeadersWritten As Boolean
    ' Positions to update
    Private m_RIFFSizePosition As Integer
    Private m_DataSizePosition As Integer
    Private m_FactSampleCountPosition As Integer

    Private m_DataBytesWritten As Integer
    Private m_WriteFactChunk As Boolean
    Private m_SamplesWritten As Integer

    Private m_Wfx As WaveFormatEx

    <DescriptionAttribute("Controls if the fact chunk is written in the .wav file.")> _
    Property WriteFactChunk() As Boolean
        Get
            Return m_WriteFactChunk
        End Get
        Set(ByVal Value As Boolean)
            If m_HeadersWritten Then
                Throw New InvalidOperationException("Wav Headers have already been written")
            End If
            m_WriteFactChunk = Value
        End Set
    End Property

    Public Sub New()
        Init()
    End Sub

    Private Sub Init()
        m_Writer = Nothing
        m_Wfx = Nothing
        m_HeadersWritten = False
        m_DataBytesWritten = 0
        m_RIFFSizePosition = 0
        m_DataSizePosition = 0
        m_FactSampleCountPosition = 0
        m_SamplesWritten = 0
        m_WriteFactChunk = False
    End Sub

    <DescriptionAttribute("Write any final headers and close the .wav file. This will also reset the writer state. You'll need to reset any options.")> _
    Public Sub Close()
        If Not (m_Writer Is Nothing) Then
            Try
                UpdateHeaders()
            Catch ex As InvalidOperationException
                ' Opps, somehow we opened the file without writing the headers...
            End Try
            m_Writer.Close()
            m_Writer = Nothing
        End If
        Init()
    End Sub

    Public Sub Open(ByVal filename As String, ByVal wfx As WaveFormatEx)
        Close()
        Dim file As IO.Stream
        file = New IO.FileStream(filename, IO.FileMode.Create)
        m_Writer = New IO.BinaryWriter(file)
        m_Wfx = wfx
        WriteHeaders()
    End Sub

    Private Sub WriteHeaders()
        If m_HeadersWritten Then
            Throw New InvalidOperationException("Wav Headers have already been written")
        End If

        Dim buffer(4) As Byte
        Dim size As Int32

        ' Start at the beginning of the file
        m_Writer.BaseStream.Position = 0

        ' Write the RIFF header
        buffer(0) = &H52 ' "R"
        buffer(1) = &H49 ' "I"
        buffer(2) = &H46 ' "F"
        buffer(3) = &H46 ' "F"
        m_Writer.Write(buffer, 0, 4)
        ' Remeber this position as we will update it at the EOF
        m_RIFFSizePosition = m_Writer.BaseStream.Position
        size = 0 ' Unknown yet
        m_Writer.Write(size)

        ' Write the WAVE header
        buffer(0) = &H57 ' "W"
        buffer(1) = &H41 ' "A"
        buffer(2) = &H56 ' "V"
        buffer(3) = &H45 ' "E"
        m_Writer.Write(buffer, 0, 4)

        ' Write the Format chunk header
        buffer(0) = &H66 ' "f"
        buffer(1) = &H6D ' "m"
        buffer(2) = &H74 ' "t"
        buffer(3) = &H20 ' " "
        m_Writer.Write(buffer, 0, 4)
        size = m_Wfx.GetSize()
        m_Writer.Write(size)

        ' Format Tag
        m_Wfx.Write(m_Writer)

        If m_WriteFactChunk Then
            ' Fact chunk header
            buffer(0) = &H66 ' "f"
            buffer(1) = &H61 ' "a"
            buffer(2) = &H63 ' "c"
            buffer(3) = &H74 ' "t"
            m_Writer.Write(buffer, 0, 4)
            size = 4
            m_Writer.Write(size)
            m_FactSampleCountPosition = m_Writer.BaseStream.Position
            size = 0 ' Unknown yet
            m_Writer.Write(size)
        End If

        ' Data chunk header
        buffer(0) = &H64 ' "d"
        buffer(1) = &H61 ' "a"
        buffer(2) = &H74 ' "t"
        buffer(3) = &H61 ' "a"
        m_Writer.Write(buffer, 0, 4)
        m_DataSizePosition = m_Writer.BaseStream.Position
        size = 0 ' Unknown yet
        m_Writer.Write(size)

        m_HeadersWritten = True
    End Sub

    Public Sub WriteSampleData(ByRef data As SByte(), ByVal offset As Integer, ByVal count As Integer)
        WriteSampleData(data, offset, count, -1)
    End Sub

    <DescriptionAttribute("Write sample data to the file.")> _
    Public Sub WriteSampleData(ByRef data As SByte(), ByVal offset As Integer, ByVal count As Integer, ByVal sampleCount As Integer)
        If Not m_HeadersWritten Then
            Throw New InvalidOperationException("Wav Headers have not been written yet")
        End If
        Dim size As Integer
        size = count - offset

        Dim i As Integer
        For i = 0 To (size - 1)
            m_Writer.Write(data(i))
        Next i
        m_DataBytesWritten += size
        If sampleCount > 0 Then
            m_SamplesWritten += sampleCount
        End If
    End Sub

    Public Sub WriteSampleData(ByRef data As Byte(), ByVal offset As Integer, ByVal count As Integer)
        WriteSampleData(data, offset, count, -1)
    End Sub

    <DescriptionAttribute("Write sample data to the file.")> _
    Public Sub WriteSampleData(ByRef data As Byte(), ByVal offset As Integer, ByVal count As Integer, ByVal sampleCount As Integer)
        If Not m_HeadersWritten Then
            Throw New InvalidOperationException("Wav Headers have not been written yet")
        End If
        m_Writer.Write(data, offset, count)
        m_DataBytesWritten += count - offset
        If sampleCount > 0 Then
            m_SamplesWritten += sampleCount
        End If
    End Sub

    Public Sub WriteSampleData(ByRef data As Single(), ByVal offset As Integer, ByVal count As Integer)
        WriteSampleData(data, offset, count, -1)
    End Sub

    <DescriptionAttribute("Write sample data to the file.")> _
    Public Sub WriteSampleData(ByRef data As Single(), ByVal offset As Integer, ByVal count As Integer, ByVal sampleCount As Integer)
        If Not m_HeadersWritten Then
            Throw New InvalidOperationException("Wav Headers have not been written yet")
        End If
        Dim i As Integer
        For i = 0 To count
            m_Writer.Write(data(offset + i))
        Next i
        m_DataBytesWritten += (count - offset) * (32 / 8)
        If sampleCount > 0 Then
            m_SamplesWritten += sampleCount
        End If
    End Sub

    Public Sub WriteSampleData(ByRef data As Double(), ByVal offset As Integer, ByVal count As Integer)
        WriteSampleData(data, offset, count, -1)
    End Sub

    <DescriptionAttribute("Write sample data to the file.")> _
    Public Sub WriteSampleData(ByRef data As Double(), ByVal offset As Integer, ByVal count As Integer, ByVal sampleCount As Integer)
        If Not m_HeadersWritten Then
            Throw New InvalidOperationException("Wav Headers have not been written yet")
        End If
        Dim i As Integer
        For i = 0 To count
            m_Writer.Write(data(offset + i))
        Next i
        m_DataBytesWritten += (count - offset) * (64 / 8)
        If sampleCount > 0 Then
            m_SamplesWritten += sampleCount
        End If
    End Sub

    Private Sub UpdateHeaders()
        If Not m_HeadersWritten Then
            Throw New InvalidOperationException("Unable to update headers if they have not been written yet")
        End If

        Dim pos As Long
        Dim size As Int32

        ' Remeber the current position, we restore it at the end
        ' So one could repeatedly update the headers
        pos = m_Writer.BaseStream.Position

        m_Writer.BaseStream.Position = m_RIFFSizePosition
        ' <RIFF<size>> Is not included in this size
        ' <WAVE = 4
        size = 4
        ' <fmt <size>> = 8
        size += 8
        If m_WriteFactChunk Then
            ' <fact<size<dwSampleCount>> = 12 
            size += 12
        End If
        ' <data<size>> = 8
        size += 8
        ' The actual "fmt " and "data" chunks content
        size += m_Wfx.GetSize() + m_DataBytesWritten
        ' Now we write the size of the whole RIFF tree
        m_Writer.Write(size)

        If m_WriteFactChunk Then
            m_Writer.BaseStream.Position = m_FactSampleCountPosition
            size = m_SamplesWritten
            m_Writer.Write(size)
        End If

        m_Writer.BaseStream.Position = m_DataSizePosition
        size = m_DataBytesWritten
        m_Writer.Write(size)

        m_Writer.BaseStream.Position = pos
    End Sub
End Class
