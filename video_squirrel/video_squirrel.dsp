# Microsoft Developer Studio Project File - Name="video_squirrel" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=video_squirrel - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "video_squirrel.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "video_squirrel.mak" CFG="video_squirrel - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "video_squirrel - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "video_squirrel - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "video_squirrel - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W4 /Gm /GX /ZI /Od /I ".\\" /I ".\tinyxml" /I ".\avilib" /I "..\MatroskaUtils" /I "..\libmatroska\\" /I "..\libebml\\" /I "..\libebml\src\platform\win32" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D WINVER=0x400 /D "_MT" /D wxUSE_GUI=1 /D "__WXDEBUG__" /D WXDEBUG=1 /D "_UNICODE" /I /I /GF /GZ "" ..\wxWindows-2.4.1\INCLUDE"" "" ..\wxWindows-2.4.1\LIB\mswud"" PRECOMP_VC7_TOBEREMOVED /c
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /I ".\\" /I ".\tinyxml" /I ".\avilib" /I "..\MatroskaUtils" /I "..\libmatroska\\" /I "..\libebml\\" /I "..\libebml\src\platform\win32" /I "..\wxWindows-2.4.2\INCLUDE" /I "..\wxWindows-2.4.2\LIB\mswud" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D WINVER=0x400 /D "_MT" /D wxUSE_GUI=1 /D "__WXDEBUG__" /D WXDEBUG=1 /D "_UNICODE" /D "UNICODE" /GF /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib wxmswdu.lib ZLIBD.LIB PNGD.LIB Shlwapi.lib Winmm.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"..\wxWindows-2.4.1\LIB"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib wxmswud.lib ZLIBD.LIB PNGD.LIB Shlwapi.lib Winmm.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"..\wxWindows-2.4.2\LIB"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "video_squirrel - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W4 /Gm /GX /Zi /Ox /I ".\\" /I ".\tinyxml" /I ".\avilib" /I "..\MatroskaUtils" /I "..\libmatroska\\" /I "..\libebml\\" /I "..\libebml\src\platform\win32" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D WINVER=0x400 /D "_MT" /D wxUSE_GUI=1 /D "_UNICODE" /I /I /GF "" ..\wxWindows-2.4.1\INCLUDE"" "" ..\wxWindows-2.4.1\LIB\mswu"" PRECOMP_VC7_TOBEREMOVED /c
# ADD CPP /nologo /MD /W2 /Gm /GX /Zi /O2 /Oy- /I ".\\" /I ".\tinyxml" /I ".\avilib" /I "..\MatroskaUtils" /I "..\libmatroska\\" /I "..\libebml\\" /I "..\libebml\src\platform\win32" /I "..\wxWindows-2.4.2\INCLUDE" /I "..\wxWindows-2.4.2\LIB\mswu" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D WINVER=0x400 /D "_MT" /D wxUSE_GUI=1 /D "_UNICODE" /D "UNICODE" /GF /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib wxmswu.lib ZLIB.LIB PNG.LIB Shlwapi.lib Winmm.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"LIBCMT" /pdbtype:sept /libpath:"..\wxWindows-2.4.1\LIB" /mapinfo:exports
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib wxmswu.lib ZLIB.LIB PNG.LIB Shlwapi.lib Winmm.lib Unicows.lib /nologo /subsystem:windows /map /debug /machine:I386 /pdbtype:sept /libpath:"..\wxWindows-2.4.2\LIB" /mapinfo:exports
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "video_squirrel - Win32 Debug"
# Name "video_squirrel - Win32 Release"
# Begin Group "avilib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=avilib\avilib.c
DEP_CPP_AVILI=\
	".\avilib\avilib.h"\
	

!IF  "$(CFG)" == "video_squirrel - Win32 Debug"

# ADD CPP /nologo /GX /TC /GZ

!ELSEIF  "$(CFG)" == "video_squirrel - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=avilib\avilib.h
# End Source File
# End Group
# Begin Group "MatroskaUtils"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\MatroskaUtils\DbgOut.cpp
DEP_CPP_DBGOU=\
	"..\MatroskaUtils\DbgOut.h"\
	
# End Source File
# Begin Source File

SOURCE=..\MatroskaUtils\DbgOut.h
# End Source File
# Begin Source File

SOURCE=..\MatroskaUtils\JCThread.h
# End Source File
# Begin Source File

SOURCE=..\MatroskaUtils\JList.h
# End Source File
# Begin Source File

SOURCE=..\MatroskaUtils\JString.cpp
DEP_CPP_JSTRI=\
	"..\libebml\ebml\c\libebml_t.h"\
	"..\libebml\ebml\EbmlConfig.h"\
	"..\libebml\ebml\EbmlEndian.h"\
	"..\libebml\ebml\EbmlTypes.h"\
	"..\MatroskaUtils\JString.h"\
	
# End Source File
# Begin Source File

SOURCE=..\MatroskaUtils\JString.h
# End Source File
# Begin Source File

SOURCE=..\MatroskaUtils\MatroskaTagData.cpp
DEP_CPP_MATRO=\
	"..\libebml\ebml\c\libebml_t.h"\
	"..\libebml\ebml\EbmlBinary.h"\
	"..\libebml\ebml\EbmlConfig.h"\
	"..\libebml\ebml\EbmlContexts.h"\
	"..\libebml\ebml\EbmlCrc32.h"\
	"..\libebml\ebml\EbmlDate.h"\
	"..\libebml\ebml\EbmlElement.h"\
	"..\libebml\ebml\EbmlEndian.h"\
	"..\libebml\ebml\EbmlFloat.h"\
	"..\libebml\ebml\EbmlHead.h"\
	"..\libebml\ebml\EbmlId.h"\
	"..\libebml\ebml\EbmlMaster.h"\
	"..\libebml\ebml\EbmlSInteger.h"\
	"..\libebml\ebml\EbmlStream.h"\
	"..\libebml\ebml\EbmlString.h"\
	"..\libebml\ebml\EbmlSubHead.h"\
	"..\libebml\ebml\EbmlTypes.h"\
	"..\libebml\ebml\EbmlUInteger.h"\
	"..\libebml\ebml\EbmlUnicodeString.h"\
	"..\libebml\ebml\EbmlVersion.h"\
	"..\libebml\ebml\EbmlVoid.h"\
	"..\libebml\ebml\IOCallback.h"\
	"..\libebml\ebml\StdIOCallback.h"\
	"..\libmatroska\matroska\c\libmatroska_t.h"\
	"..\libmatroska\matroska\KaxAttached.h"\
	"..\libmatroska\matroska\KaxAttachments.h"\
	"..\libmatroska\matroska\KaxBlock.h"\
	"..\libmatroska\matroska\KaxCluster.h"\
	"..\libmatroska\matroska\KaxClusterData.h"\
	"..\libmatroska\matroska\KaxConfig.h"\
	"..\libmatroska\matroska\KaxContexts.h"\
	"..\libmatroska\matroska\KaxCues.h"\
	"..\libmatroska\matroska\KaxInfo.h"\
	"..\libmatroska\matroska\KaxInfoData.h"\
	"..\libmatroska\matroska\KaxSeekHead.h"\
	"..\libmatroska\matroska\KaxSegment.h"\
	"..\libmatroska\matroska\KaxTag.h"\
	"..\libmatroska\matroska\KaxTagMulti.h"\
	"..\libmatroska\matroska\KaxTags.h"\
	"..\libmatroska\matroska\KaxTrackAudio.h"\
	"..\libmatroska\matroska\KaxTrackEntryData.h"\
	"..\libmatroska\matroska\KaxTracks.h"\
	"..\libmatroska\matroska\KaxTrackVideo.h"\
	"..\libmatroska\matroska\KaxTypes.h"\
	"..\libmatroska\matroska\KaxVersion.h"\
	"..\MatroskaUtils\DbgOut.h"\
	"..\MatroskaUtils\JString.h"\
	"..\MatroskaUtils\MatroskaTagData.h"\
	"..\MatroskaUtils\MatroskaUtilsDebug.h"\
	
NODEP_CPP_MATRO=\
	"..\MatroskaUtils\safe_gettext_dll.h"\
	
# End Source File
# Begin Source File

SOURCE=..\MatroskaUtils\MatroskaTagData.h
# End Source File
# Begin Source File

SOURCE=..\MatroskaUtils\MatroskaTrackData.cpp
DEP_CPP_MATROS=\
	"..\libebml\ebml\c\libebml_t.h"\
	"..\libebml\ebml\EbmlBinary.h"\
	"..\libebml\ebml\EbmlConfig.h"\
	"..\libebml\ebml\EbmlContexts.h"\
	"..\libebml\ebml\EbmlCrc32.h"\
	"..\libebml\ebml\EbmlDate.h"\
	"..\libebml\ebml\EbmlElement.h"\
	"..\libebml\ebml\EbmlEndian.h"\
	"..\libebml\ebml\EbmlFloat.h"\
	"..\libebml\ebml\EbmlHead.h"\
	"..\libebml\ebml\EbmlId.h"\
	"..\libebml\ebml\EbmlMaster.h"\
	"..\libebml\ebml\EbmlSInteger.h"\
	"..\libebml\ebml\EbmlStream.h"\
	"..\libebml\ebml\EbmlString.h"\
	"..\libebml\ebml\EbmlSubHead.h"\
	"..\libebml\ebml\EbmlTypes.h"\
	"..\libebml\ebml\EbmlUInteger.h"\
	"..\libebml\ebml\EbmlUnicodeString.h"\
	"..\libebml\ebml\EbmlVersion.h"\
	"..\libebml\ebml\EbmlVoid.h"\
	"..\libebml\ebml\IOCallback.h"\
	"..\libebml\ebml\MemIOCallback.h"\
	"..\libebml\ebml\StdIOCallback.h"\
	"..\libebml\src\platform\win32\WinIOCallback.h"\
	"..\libmatroska\matroska\c\libmatroska_t.h"\
	"..\libmatroska\matroska\KaxAttached.h"\
	"..\libmatroska\matroska\KaxAttachments.h"\
	"..\libmatroska\matroska\KaxBlock.h"\
	"..\libmatroska\matroska\KaxCluster.h"\
	"..\libmatroska\matroska\KaxClusterData.h"\
	"..\libmatroska\matroska\KaxConfig.h"\
	"..\libmatroska\matroska\KaxContexts.h"\
	"..\libmatroska\matroska\KaxCues.h"\
	"..\libmatroska\matroska\KaxInfo.h"\
	"..\libmatroska\matroska\KaxInfoData.h"\
	"..\libmatroska\matroska\KaxSeekHead.h"\
	"..\libmatroska\matroska\KaxSegment.h"\
	"..\libmatroska\matroska\KaxTag.h"\
	"..\libmatroska\matroska\KaxTagMulti.h"\
	"..\libmatroska\matroska\KaxTags.h"\
	"..\libmatroska\matroska\KaxTrackAudio.h"\
	"..\libmatroska\matroska\KaxTrackEntryData.h"\
	"..\libmatroska\matroska\KaxTracks.h"\
	"..\libmatroska\matroska\KaxTrackVideo.h"\
	"..\libmatroska\matroska\KaxTypes.h"\
	"..\libmatroska\matroska\KaxVersion.h"\
	"..\MatroskaUtils\DbgOut.h"\
	"..\MatroskaUtils\JString.h"\
	"..\MatroskaUtils\MatroskaTagData.h"\
	"..\MatroskaUtils\MatroskaTrackData.h"\
	"..\MatroskaUtils\MatroskaUtilsDebug.h"\
	"..\MatroskaUtils\MatroskaUtilsReader.h"\
	
NODEP_CPP_MATROS=\
	"..\MatroskaUtils\CxImage\ximage.h"\
	"..\MatroskaUtils\safe_gettext_dll.h"\
	
# End Source File
# Begin Source File

SOURCE=..\MatroskaUtils\MatroskaTrackData.h
# End Source File
# Begin Source File

SOURCE=..\MatroskaUtils\MatroskaUtils.cpp
DEP_CPP_MATROSK=\
	"..\libebml\ebml\c\libebml_t.h"\
	"..\libebml\ebml\EbmlBinary.h"\
	"..\libebml\ebml\EbmlConfig.h"\
	"..\libebml\ebml\EbmlContexts.h"\
	"..\libebml\ebml\EbmlCrc32.h"\
	"..\libebml\ebml\EbmlDate.h"\
	"..\libebml\ebml\EbmlElement.h"\
	"..\libebml\ebml\EbmlEndian.h"\
	"..\libebml\ebml\EbmlFloat.h"\
	"..\libebml\ebml\EbmlHead.h"\
	"..\libebml\ebml\EbmlId.h"\
	"..\libebml\ebml\EbmlMaster.h"\
	"..\libebml\ebml\EbmlSInteger.h"\
	"..\libebml\ebml\EbmlStream.h"\
	"..\libebml\ebml\EbmlString.h"\
	"..\libebml\ebml\EbmlSubHead.h"\
	"..\libebml\ebml\EbmlTypes.h"\
	"..\libebml\ebml\EbmlUInteger.h"\
	"..\libebml\ebml\EbmlUnicodeString.h"\
	"..\libebml\ebml\EbmlVersion.h"\
	"..\libebml\ebml\EbmlVoid.h"\
	"..\libebml\ebml\IOCallback.h"\
	"..\libebml\ebml\MemIOCallback.h"\
	"..\libebml\ebml\StdIOCallback.h"\
	"..\libebml\src\platform\win32\WinIOCallback.h"\
	"..\libmatroska\matroska\c\libmatroska_t.h"\
	"..\libmatroska\matroska\KaxAttached.h"\
	"..\libmatroska\matroska\KaxAttachments.h"\
	"..\libmatroska\matroska\KaxBlock.h"\
	"..\libmatroska\matroska\KaxBlockData.h"\
	"..\libmatroska\matroska\KaxCluster.h"\
	"..\libmatroska\matroska\KaxClusterData.h"\
	"..\libmatroska\matroska\KaxConfig.h"\
	"..\libmatroska\matroska\KaxContexts.h"\
	"..\libmatroska\matroska\KaxCues.h"\
	"..\libmatroska\matroska\KaxInfo.h"\
	"..\libmatroska\matroska\KaxInfoData.h"\
	"..\libmatroska\matroska\KaxSeekHead.h"\
	"..\libmatroska\matroska\KaxSegment.h"\
	"..\libmatroska\matroska\KaxTag.h"\
	"..\libmatroska\matroska\KaxTagMulti.h"\
	"..\libmatroska\matroska\KaxTags.h"\
	"..\libmatroska\matroska\KaxTrackAudio.h"\
	"..\libmatroska\matroska\KaxTrackEntryData.h"\
	"..\libmatroska\matroska\KaxTracks.h"\
	"..\libmatroska\matroska\KaxTrackVideo.h"\
	"..\libmatroska\matroska\KaxTypes.h"\
	"..\libmatroska\matroska\KaxVersion.h"\
	"..\MatroskaUtils\DbgOut.h"\
	"..\MatroskaUtils\JCThread.h"\
	"..\MatroskaUtils\JList.h"\
	"..\MatroskaUtils\JString.h"\
	"..\MatroskaUtils\MatroskaTagData.h"\
	"..\MatroskaUtils\MatroskaTrackData.h"\
	"..\MatroskaUtils\MatroskaUtils.h"\
	"..\MatroskaUtils\MatroskaUtilsBitrate.h"\
	"..\MatroskaUtils\MatroskaUtilsDebug.h"\
	"..\MatroskaUtils\MatroskaUtilsReader.h"\
	".\tinyxml\tinystr.h"\
	".\tinyxml\tinyxml.h"\
	
NODEP_CPP_MATROSK=\
	"..\MatroskaUtils\CxImage\ximage.h"\
	"..\MatroskaUtils\safe_gettext_dll.h"\
	
# End Source File
# Begin Source File

SOURCE=..\MatroskaUtils\MatroskaUtils.h
# End Source File
# Begin Source File

SOURCE=..\MatroskaUtils\MatroskaUtilsDebug.h
# End Source File
# Begin Source File

SOURCE=..\MatroskaUtils\MatroskaUtilsReader.cpp
DEP_CPP_MATROSKA=\
	"..\libebml\ebml\c\libebml_t.h"\
	"..\libebml\ebml\EbmlBinary.h"\
	"..\libebml\ebml\EbmlConfig.h"\
	"..\libebml\ebml\EbmlCrc32.h"\
	"..\libebml\ebml\EbmlElement.h"\
	"..\libebml\ebml\EbmlEndian.h"\
	"..\libebml\ebml\EbmlId.h"\
	"..\libebml\ebml\EbmlTypes.h"\
	"..\libebml\ebml\IOCallback.h"\
	"..\libebml\ebml\MemIOCallback.h"\
	"..\libebml\ebml\StdIOCallback.h"\
	"..\libebml\src\platform\win32\WinIOCallback.h"\
	"..\MatroskaUtils\JString.h"\
	"..\MatroskaUtils\MatroskaUtilsReader.h"\
	
# End Source File
# Begin Source File

SOURCE=..\MatroskaUtils\MatroskaUtilsReader.h
# End Source File
# Begin Source File

SOURCE=..\MatroskaUtils\MatroskaUtilsTag.cpp
DEP_CPP_MATROSKAU=\
	"..\libebml\ebml\c\libebml_t.h"\
	"..\libebml\ebml\EbmlBinary.h"\
	"..\libebml\ebml\EbmlConfig.h"\
	"..\libebml\ebml\EbmlContexts.h"\
	"..\libebml\ebml\EbmlCrc32.h"\
	"..\libebml\ebml\EbmlDate.h"\
	"..\libebml\ebml\EbmlElement.h"\
	"..\libebml\ebml\EbmlEndian.h"\
	"..\libebml\ebml\EbmlFloat.h"\
	"..\libebml\ebml\EbmlHead.h"\
	"..\libebml\ebml\EbmlId.h"\
	"..\libebml\ebml\EbmlMaster.h"\
	"..\libebml\ebml\EbmlSInteger.h"\
	"..\libebml\ebml\EbmlStream.h"\
	"..\libebml\ebml\EbmlString.h"\
	"..\libebml\ebml\EbmlSubHead.h"\
	"..\libebml\ebml\EbmlTypes.h"\
	"..\libebml\ebml\EbmlUInteger.h"\
	"..\libebml\ebml\EbmlUnicodeString.h"\
	"..\libebml\ebml\EbmlVersion.h"\
	"..\libebml\ebml\EbmlVoid.h"\
	"..\libebml\ebml\IOCallback.h"\
	"..\libebml\ebml\MemIOCallback.h"\
	"..\libebml\ebml\StdIOCallback.h"\
	"..\libebml\src\platform\win32\WinIOCallback.h"\
	"..\libmatroska\matroska\c\libmatroska_t.h"\
	"..\libmatroska\matroska\KaxAttached.h"\
	"..\libmatroska\matroska\KaxAttachments.h"\
	"..\libmatroska\matroska\KaxBlock.h"\
	"..\libmatroska\matroska\KaxBlockData.h"\
	"..\libmatroska\matroska\KaxCluster.h"\
	"..\libmatroska\matroska\KaxClusterData.h"\
	"..\libmatroska\matroska\KaxConfig.h"\
	"..\libmatroska\matroska\KaxContexts.h"\
	"..\libmatroska\matroska\KaxCues.h"\
	"..\libmatroska\matroska\KaxInfo.h"\
	"..\libmatroska\matroska\KaxInfoData.h"\
	"..\libmatroska\matroska\KaxSeekHead.h"\
	"..\libmatroska\matroska\KaxSegment.h"\
	"..\libmatroska\matroska\KaxTag.h"\
	"..\libmatroska\matroska\KaxTagMulti.h"\
	"..\libmatroska\matroska\KaxTags.h"\
	"..\libmatroska\matroska\KaxTrackAudio.h"\
	"..\libmatroska\matroska\KaxTrackEntryData.h"\
	"..\libmatroska\matroska\KaxTracks.h"\
	"..\libmatroska\matroska\KaxTrackVideo.h"\
	"..\libmatroska\matroska\KaxTypes.h"\
	"..\libmatroska\matroska\KaxVersion.h"\
	"..\MatroskaUtils\DbgOut.h"\
	"..\MatroskaUtils\JCThread.h"\
	"..\MatroskaUtils\JList.h"\
	"..\MatroskaUtils\JString.h"\
	"..\MatroskaUtils\MatroskaTagData.h"\
	"..\MatroskaUtils\MatroskaTrackData.h"\
	"..\MatroskaUtils\MatroskaUtils.h"\
	"..\MatroskaUtils\MatroskaUtilsBitrate.h"\
	"..\MatroskaUtils\MatroskaUtilsDebug.h"\
	"..\MatroskaUtils\MatroskaUtilsReader.h"\
	".\tinyxml\tinystr.h"\
	".\tinyxml\tinyxml.h"\
	
NODEP_CPP_MATROSKAU=\
	"..\MatroskaUtils\CxImage\ximage.h"\
	"..\MatroskaUtils\safe_gettext_dll.h"\
	
# End Source File
# Begin Source File

SOURCE=..\libebml\src\platform\win32\WinIOCallback.cpp
DEP_CPP_WINIO=\
	"..\libebml\ebml\c\libebml_t.h"\
	"..\libebml\ebml\Debug.h"\
	"..\libebml\ebml\EbmlConfig.h"\
	"..\libebml\ebml\EbmlEndian.h"\
	"..\libebml\ebml\EbmlTypes.h"\
	"..\libebml\ebml\IOCallback.h"\
	"..\libebml\src\platform\win32\WinIOCallback.h"\
	
# End Source File
# Begin Source File

SOURCE=..\libebml\src\platform\win32\WinIOCallback.h
# End Source File
# End Group
# Begin Group "tinyxml"

# PROP Default_Filter ""
# Begin Source File

SOURCE=tinyxml\tinystr.cpp
DEP_CPP_TINYS=\
	".\tinyxml\tinystr.h"\
	".\tinyxml\tinyxml.h"\
	
# End Source File
# Begin Source File

SOURCE=tinyxml\tinystr.h
# End Source File
# Begin Source File

SOURCE=tinyxml\tinyxml.cpp
DEP_CPP_TINYX=\
	".\tinyxml\tinystr.h"\
	".\tinyxml\tinyxml.h"\
	
# End Source File
# Begin Source File

SOURCE=tinyxml\tinyxml.h
# End Source File
# Begin Source File

SOURCE=tinyxml\tinyxmlerror.cpp
DEP_CPP_TINYXM=\
	".\tinyxml\tinystr.h"\
	".\tinyxml\tinyxml.h"\
	
# End Source File
# Begin Source File

SOURCE=tinyxml\tinyxmlparser.cpp
DEP_CPP_TINYXML=\
	".\tinyxml\tinystr.h"\
	".\tinyxml\tinyxml.h"\
	
# End Source File
# End Group
# Begin Source File

SOURCE=.\images.cpp
DEP_CPP_IMAGE=\
	".\images.h"\
	
# End Source File
# Begin Source File

SOURCE=.\images.h
# End Source File
# Begin Source File

SOURCE=MPEGInfo.cpp
DEP_CPP_MPEGI=\
	".\MPEGInfo.h"\
	
# End Source File
# Begin Source File

SOURCE=MPEGInfo.h
# End Source File
# Begin Source File

SOURCE=realmedia_reader.cpp
DEP_CPP_REALM=\
	".\realmedia_reader.h"\
	
# End Source File
# Begin Source File

SOURCE=realmedia_reader.h
# End Source File
# Begin Source File

SOURCE=video_squirrel.cpp
DEP_CPP_VIDEO=\
	"..\libebml\ebml\c\libebml_t.h"\
	"..\libebml\ebml\EbmlBinary.h"\
	"..\libebml\ebml\EbmlConfig.h"\
	"..\libebml\ebml\EbmlContexts.h"\
	"..\libebml\ebml\EbmlCrc32.h"\
	"..\libebml\ebml\EbmlDate.h"\
	"..\libebml\ebml\EbmlElement.h"\
	"..\libebml\ebml\EbmlEndian.h"\
	"..\libebml\ebml\EbmlFloat.h"\
	"..\libebml\ebml\EbmlHead.h"\
	"..\libebml\ebml\EbmlId.h"\
	"..\libebml\ebml\EbmlMaster.h"\
	"..\libebml\ebml\EbmlSInteger.h"\
	"..\libebml\ebml\EbmlStream.h"\
	"..\libebml\ebml\EbmlString.h"\
	"..\libebml\ebml\EbmlSubHead.h"\
	"..\libebml\ebml\EbmlTypes.h"\
	"..\libebml\ebml\EbmlUInteger.h"\
	"..\libebml\ebml\EbmlUnicodeString.h"\
	"..\libebml\ebml\EbmlVersion.h"\
	"..\libebml\ebml\EbmlVoid.h"\
	"..\libebml\ebml\IOCallback.h"\
	"..\libebml\ebml\MemIOCallback.h"\
	"..\libebml\ebml\StdIOCallback.h"\
	"..\libebml\src\platform\win32\WinIOCallback.h"\
	"..\libmatroska\matroska\c\libmatroska_t.h"\
	"..\libmatroska\matroska\KaxAttached.h"\
	"..\libmatroska\matroska\KaxAttachments.h"\
	"..\libmatroska\matroska\KaxBlock.h"\
	"..\libmatroska\matroska\KaxBlockData.h"\
	"..\libmatroska\matroska\KaxCluster.h"\
	"..\libmatroska\matroska\KaxClusterData.h"\
	"..\libmatroska\matroska\KaxConfig.h"\
	"..\libmatroska\matroska\KaxContexts.h"\
	"..\libmatroska\matroska\KaxCues.h"\
	"..\libmatroska\matroska\KaxInfo.h"\
	"..\libmatroska\matroska\KaxInfoData.h"\
	"..\libmatroska\matroska\KaxSeekHead.h"\
	"..\libmatroska\matroska\KaxSegment.h"\
	"..\libmatroska\matroska\KaxTag.h"\
	"..\libmatroska\matroska\KaxTagMulti.h"\
	"..\libmatroska\matroska\KaxTags.h"\
	"..\libmatroska\matroska\KaxTrackAudio.h"\
	"..\libmatroska\matroska\KaxTrackEntryData.h"\
	"..\libmatroska\matroska\KaxTracks.h"\
	"..\libmatroska\matroska\KaxTrackVideo.h"\
	"..\libmatroska\matroska\KaxTypes.h"\
	"..\libmatroska\matroska\KaxVersion.h"\
	"..\MatroskaUtils\DbgOut.h"\
	"..\MatroskaUtils\JCThread.h"\
	"..\MatroskaUtils\JList.h"\
	"..\MatroskaUtils\JString.h"\
	"..\MatroskaUtils\MatroskaTagData.h"\
	"..\MatroskaUtils\MatroskaTrackData.h"\
	"..\MatroskaUtils\MatroskaUtils.h"\
	"..\MatroskaUtils\MatroskaUtilsBitrate.h"\
	"..\MatroskaUtils\MatroskaUtilsDebug.h"\
	"..\MatroskaUtils\MatroskaUtilsReader.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\accel.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\app.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\bitmap.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\bmpbuttn.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\brush.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\buffer.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\build.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\busyinfo.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\button.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\checkbox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\checklst.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\chkconf.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\choicdlg.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\choice.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\clntdata.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\cmndata.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\colour.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\combobox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\confbase.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\config.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\containr.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\control.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\ctrlsub.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\cursor.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\dataobj.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\datetime.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\datetime.inl"\
	"..\wxWindows-2.4.2\INCLUDE\wx\dc.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\dcclient.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\dcmemory.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\dcprint.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\dcps.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\dcscreen.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\debug.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\defs.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\dialog.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\dir.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\dirdlg.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\dnd.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\dynarray.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\encconv.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\event.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\features.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\ffile.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\file.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\fileconf.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\filedlg.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\filefn.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\filename.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\filesys.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\font.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\fontenc.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\frame.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\fs_zip.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\gauge.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\gdicmn.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\gdiobj.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\generic\accel.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\generic\choicdgg.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\generic\dcpsg.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\generic\dirdlgg.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\generic\filedlgg.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\generic\grid.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\generic\gridg.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\generic\imaglist.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\generic\listctrl.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\generic\mdig.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\generic\msgdlgg.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\generic\notebook.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\generic\paletteg.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\generic\panelg.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\generic\printps.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\generic\prntdlgg.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\generic\scrolwin.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\generic\splitter.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\generic\statusbr.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\generic\tabg.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\generic\textdlgg.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\generic\timer.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\generic\treectlg.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\grid.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\gsocket.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\hash.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\hashmap.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\helpbase.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\html\helpctrl.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\html\helpdata.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\html\helpfrm.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\html\htmlcell.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\html\htmldefs.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\html\htmlfilt.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\html\htmlpars.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\html\htmltag.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\html\htmlwin.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\html\htmprint.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\html\m_templ.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\html\winpars.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\icon.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\iconbndl.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\imagbmp.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\image.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\imaggif.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\imagiff.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\imagjpeg.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\imaglist.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\imagpcx.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\imagpng.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\imagpnm.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\imagtiff.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\imagxpm.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\intl.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\ioswrap.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\layout.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\list.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\listbase.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\listbox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\listctrl.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\listimpl.cpp"\
	"..\wxWindows-2.4.2\INCLUDE\wx\log.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\longlong.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\mdi.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\memory.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\menu.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\menuitem.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\module.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msgdlg.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\accel.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\app.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\bitmap.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\bmpbuttn.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\brush.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\button.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\checkbox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\checklst.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\choice.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\colour.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\combobox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\control.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\cursor.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\dc.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\dcclient.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\dcmemory.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\dcprint.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\dcscreen.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\dialog.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\dirdlg.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\filedlg.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\font.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\frame.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\gauge95.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\gaugemsw.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\gccpriv.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\gdiimage.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\gdiobj.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\icon.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\imaglist.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\iniconf.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\listbox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\listctrl.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\mdi.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\menu.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\menuitem.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\msgdlg.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\notebook.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\ole\dataform.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\ole\dataobj.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\ole\dataobj2.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\ole\dropsrc.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\ole\droptgt.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\palette.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\pen.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\printdlg.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\printwin.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\radiobox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\radiobut.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\regconf.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\region.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\registry.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\scrolbar.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\slider95.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\slidrmsw.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\statbmp.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\statbox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\statbr95.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\stattext.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\tbar95.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\tbarmsw.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\textctrl.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\timer.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\toplevel.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\treectrl.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\window.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\msw\winundef.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\notebook.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\object.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\ownerdrw.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\palette.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\panel.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\pen.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\platform.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\print.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\printdlg.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\prntbase.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\protocol\http.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\protocol\protocol.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\radiobox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\radiobut.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\region.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\sckaddr.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\scrolbar.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\scrolwin.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\settings.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\sizer.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\slider.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\socket.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\splitter.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\statbmp.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\statbox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\stattext.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\statusbr.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\strconv.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\stream.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\string.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\tbarbase.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\tbarsmpl.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\textbuf.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\textctrl.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\textdlg.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\textfile.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\thread.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\timer.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\toolbar.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\toplevel.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\treebase.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\treectrl.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\txtstrm.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\univ\bmpbuttn.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\univ\button.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\univ\checkbox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\univ\checklst.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\univ\choice.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\univ\combobox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\univ\control.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\univ\dialog.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\univ\frame.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\univ\gauge.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\univ\inpcons.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\univ\inphand.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\univ\listbox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\univ\menu.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\univ\menuitem.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\univ\notebook.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\univ\radiobox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\univ\radiobut.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\univ\scrarrow.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\univ\scrolbar.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\univ\scrthumb.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\univ\slider.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\univ\statbmp.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\univ\statbox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\univ\stattext.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\univ\statusbr.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\univ\textctrl.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\univ\theme.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\univ\toolbar.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\univ\toplevel.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\univ\window.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\url.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\utils.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\validate.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\valtext.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\version.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wfstream.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\window.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wxchar.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wxhtml.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wxprec.h"\
	"..\wxWindows-2.4.2\LIB\mswu\wx\setup.h"\
	".\avilib\avilib.h"\
	".\images.h"\
	".\MPEGInfo.h"\
	".\realmedia_reader.h"\
	".\tinyxml\tinystr.h"\
	".\tinyxml\tinyxml.h"\
	".\video_squirrel.h"\
	
NODEP_CPP_VIDEO=\
	"..\MatroskaUtils\CxImage\ximage.h"\
	"..\MatroskaUtils\safe_gettext_dll.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\accel.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\app.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\bitmap.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\bmpbuttn.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\brush.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\button.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\checkbox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\checklst.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\choice.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\colour.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\combobox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\control.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\cursor.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\dataform.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\dataobj.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\dataobj2.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\dc.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\dcclient.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\dcmemory.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\dcscreen.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\dialog.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\dnd.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\font.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\frame.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\gauge.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\gdiobj.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\icon.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\listbox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\mdi.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\menu.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\menuitem.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\notebook.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\pen.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\radiobox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\radiobut.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\region.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\scrolbar.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\scrolwin.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\slider.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\statbmp.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\statbox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\stattext.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\tbargtk.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\textctrl.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\timer.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\toplevel.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\gtk\window.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\accel.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\app.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\bitmap.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\bmpbuttn.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\brush.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\button.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\checkbox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\checklst.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\choice.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\colour.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\combobox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\control.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\cursor.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\dataform.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\dataobj.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\dataobj2.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\dc.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\dcclient.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\dcmemory.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\dcprint.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\dcscreen.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\dialog.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\dirdlg.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\dnd.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\filedlg.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\font.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\frame.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\gauge.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\gdiobj.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\icon.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\listbox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\mdi.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\menu.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\menuitem.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\msgdlg.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\notebook.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\palette.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\pen.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\printdlg.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\printmac.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\radiobox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\radiobut.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\region.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\scrolbar.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\slider.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\statbmp.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\statbox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\stattext.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\statusbr.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\textctrl.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\timer.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\toolbar.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\toplevel.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mac\window.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mgl\app.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mgl\bitmap.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mgl\brush.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mgl\colour.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mgl\cursor.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mgl\dc.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mgl\dcclient.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mgl\dcmemory.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mgl\dcscreen.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mgl\font.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mgl\gdiobj.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mgl\icon.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mgl\palette.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mgl\pen.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mgl\region.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mgl\toplevel.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\mgl\window.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\accel.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\app.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\bitmap.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\bmpbuttn.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\brush.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\button.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\checkbox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\checklst.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\choice.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\colour.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\combobox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\control.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\cursor.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\dataform.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\dataobj.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\dc.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\dcclient.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\dcmemory.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\dcscreen.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\dialog.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\dnd.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\filedlg.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\font.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\frame.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\gauge.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\gdiobj.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\icon.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\listbox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\mdi.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\menu.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\menuitem.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\msgdlg.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\palette.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\pen.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\radiobox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\radiobut.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\region.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\scrolbar.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\slider.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\statbmp.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\statbox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\stattext.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\textctrl.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\timer.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\toolbar.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\motif\window.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\accel.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\app.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\bitmap.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\bmpbuttn.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\brush.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\button.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\checkbox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\checklst.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\choice.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\colour.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\combobox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\control.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\cursor.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\dataform.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\dataobj.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\dataobj2.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\dc.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\dcclient.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\dcmemory.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\dcprint.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\dcscreen.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\dialog.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\dnd.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\filedlg.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\font.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\frame.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\gauge.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\gdiobj.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\icon.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\listbox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\menu.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\menuitem.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\msgdlg.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\notebook.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\palette.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\pen.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\radiobox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\radiobut.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\region.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\scrolbar.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\slider.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\statbmp.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\statbox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\stattext.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\textctrl.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\timer.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\toolbar.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\toplevel.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\os2\window.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\accel.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\app.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\bitmap.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\bmpbuttn.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\brush.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\button.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\checkbox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\checklst.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\choice.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\colour.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\combobox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\control.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\cursor.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\dc.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\dcclient.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\dcmemory.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\dcscreen.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\dialog.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\dnd.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\filedlg.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\font.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\gauge.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\gdiobj.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\icon.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\listbox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\mdi.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\menu.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\menuitem.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\notebook.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\palette.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\pen.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\radiobox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\radiobut.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\region.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\scrolbar.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\slider.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\statbmp.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\statbox.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\stattext.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\textctrl.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\timer.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\stubs\toolbar.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\x11\app.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\x11\bitmap.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\x11\brush.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\x11\colour.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\x11\cursor.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\x11\dataform.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\x11\dataobj.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\x11\dataobj2.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\x11\dc.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\x11\dcclient.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\x11\dcmemory.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\x11\dcscreen.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\x11\dnd.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\x11\font.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\x11\gdiobj.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\x11\icon.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\x11\palette.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\x11\pen.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\x11\region.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\x11\textctrl.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\x11\toplevel.h"\
	"..\wxWindows-2.4.2\INCLUDE\wx\wx\x11\window.h"\
	
# End Source File
# Begin Source File

SOURCE=.\exe\video_squirrel.exe.Manifest
# End Source File
# Begin Source File

SOURCE=video_squirrel.h
# End Source File
# Begin Source File

SOURCE=.\video_squirrel.ico
# End Source File
# Begin Source File

SOURCE=.\video_squirrel_private.h
# End Source File
# Begin Source File

SOURCE=.\video_squirrel_private.rc
# End Source File
# End Target
# End Project
