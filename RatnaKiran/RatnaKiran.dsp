# Microsoft Developer Studio Project File - Name="RatnaKiran" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=RatnaKiran - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RatnaKiran.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RatnaKiran.mak" CFG="RatnaKiran - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RatnaKiran - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "RatnaKiran - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RatnaKiran - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /I "./ExcelWrappers" /I "./CommonWrappers" /I "./RatnakiranDatabase" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /incremental:yes

!ELSEIF  "$(CFG)" == "RatnaKiran - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "./ExcelWrappers" /I "./CommonWrappers" /I "./RatnakiranDatabase" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:""../ZipArchive

!ENDIF 

# Begin Target

# Name "RatnaKiran - Win32 Release"
# Name "RatnaKiran - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AboutDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\AddDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\AddUserDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\AppendListDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\BackupHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\DescriptionAddDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\DetailsDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\EditPermissions.cpp
# End Source File
# Begin Source File

SOURCE=.\ExportSerialsDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\GenericListBoxDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MainGlobal.cpp
# End Source File
# Begin Source File

SOURCE=.\MarkedValueFormulaDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\MoveOptionDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\RatnaKiran.cpp
# End Source File
# Begin Source File

SOURCE=.\RatnaKiran.rc
# End Source File
# Begin Source File

SOURCE=.\RatnaKiranDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\RatnaKiranReports.cpp
# End Source File
# Begin Source File

SOURCE=.\RatnaKiranStockView.cpp
# End Source File
# Begin Source File

SOURCE=.\RatnaKiranTreeView.cpp
# End Source File
# Begin Source File

SOURCE=.\RatnaKiranView.cpp
# End Source File
# Begin Source File

SOURCE=.\ShortInvoiceDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AboutDialog.h
# End Source File
# Begin Source File

SOURCE=.\AddDialog.h
# End Source File
# Begin Source File

SOURCE=.\AddUserDialog.h
# End Source File
# Begin Source File

SOURCE=.\AppendListDialog.h
# End Source File
# Begin Source File

SOURCE=.\BackupHandler.h
# End Source File
# Begin Source File

SOURCE=.\DescriptionAddDialog.h
# End Source File
# Begin Source File

SOURCE=.\DetailsDialog.h
# End Source File
# Begin Source File

SOURCE=.\EditPermissions.h
# End Source File
# Begin Source File

SOURCE=.\ExportSerialsDialog.h
# End Source File
# Begin Source File

SOURCE=.\GenericListBoxDialog.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MainGlobal.h
# End Source File
# Begin Source File

SOURCE=.\MarkedValueFormulaDialog.h
# End Source File
# Begin Source File

SOURCE=.\MoveOptionDialog.h
# End Source File
# Begin Source File

SOURCE=.\OptionsDialog.h
# End Source File
# Begin Source File

SOURCE=.\RatnaKiran.h
# End Source File
# Begin Source File

SOURCE=.\RatnaKiranDoc.h
# End Source File
# Begin Source File

SOURCE=.\RatnaKiranReports.h
# End Source File
# Begin Source File

SOURCE=.\RatnaKiranStockView.h
# End Source File
# Begin Source File

SOURCE=.\RatnaKiranTreeView.h
# End Source File
# Begin Source File

SOURCE=.\RatnaKiranView.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ShortInvoiceDialog.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\empty.bmp
# End Source File
# Begin Source File

SOURCE=.\res\empty_im.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MAIL19B.ICO
# End Source File
# Begin Source File

SOURCE=.\res\mainicon.ICO
# End Source File
# Begin Source File

SOURCE=.\res\POINT02.ICO
# End Source File
# Begin Source File

SOURCE=.\res\POINT04.ICO
# End Source File
# Begin Source File

SOURCE=.\res\print.bmp
# End Source File
# Begin Source File

SOURCE=.\res\RatnaKiran.ico
# End Source File
# Begin Source File

SOURCE=.\res\RatnaKiran.rc2
# End Source File
# Begin Source File

SOURCE=.\res\RatnaKiranDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\state.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\W95MBX01.ICO
# End Source File
# End Group
# Begin Source File

SOURCE=.\RatnaKiran.reg
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
