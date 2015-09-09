# Microsoft Developer Studio Project File - Name="fltkdock" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=fltkdock - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "fltkdock.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fltkdock.mak" CFG="fltkdock - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fltkdock - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "fltkdock - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "fltkdock - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "fltkdock_r"
# PROP Intermediate_Dir "fltkdock_r"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W2 /GR /GX /O2 /I ".." /I "..\fltk\fltkdock" /I "..\..\fltk-2.0" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\fltkdock.lib"

!ELSEIF  "$(CFG)" == "fltkdock - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "fltkdock_d"
# PROP Intermediate_Dir "fltkdock_d"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W2 /Gm /GR /GX /ZI /Od /I ".." /I "..\fltk\fltkdock" /I "..\..\fltk-2.0" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\fltkdockd.lib"

!ENDIF 

# Begin Target

# Name "fltkdock - Win32 Release"
# Name "fltkdock - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\src\BarHolder.cxx
# End Source File
# Begin Source File

SOURCE=..\src\DockBase.cxx
# End Source File
# Begin Source File

SOURCE=..\src\DockHolder.cxx
# End Source File
# Begin Source File

SOURCE=..\src\DockStatus.cxx
# End Source File
# Begin Source File

SOURCE=..\src\DockWindow.cxx
# End Source File
# Begin Source File

SOURCE=..\src\GripperBar.cxx
# End Source File
# Begin Source File

SOURCE=..\src\PlainHolder.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Preview.cxx
# End Source File
# Begin Source File

SOURCE=..\src\RegionHolder.cxx
# End Source File
# Begin Source File

SOURCE=..\src\Resizer.cxx
# End Source File
# Begin Source File

SOURCE=..\src\SubBlock.cxx
# End Source File
# Begin Source File

SOURCE=..\src\ThinWindow.cxx
# End Source File
# Begin Source File

SOURCE=..\src\ToolBar.cxx
# End Source File
# Begin Source File

SOURCE=..\src\ToolWindow.cxx
# End Source File
# Begin Source File

SOURCE=..\src\WindowHolder.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\fltk\BarHolder.h
# End Source File
# Begin Source File

SOURCE=..\fltk\DockFlags.h
# End Source File
# Begin Source File

SOURCE=..\fltk\DockHolder.h
# End Source File
# Begin Source File

SOURCE=..\fltk\DockWindow.h
# End Source File
# Begin Source File

SOURCE=..\fltk\GripperBar.h
# End Source File
# Begin Source File

SOURCE=..\fltk\RegionHolder.h
# End Source File
# Begin Source File

SOURCE=..\fltk\ToolBar.h
# End Source File
# Begin Source File

SOURCE=..\fltk\ToolDivider.h
# End Source File
# Begin Source File

SOURCE=..\fltk\ToolWindow.h
# End Source File
# Begin Source File

SOURCE=..\fltk\WindowHolder.h
# End Source File
# End Group
# End Target
# End Project
