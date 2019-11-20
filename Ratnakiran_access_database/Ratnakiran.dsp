# Microsoft Developer Studio Project File - Name="Ratnakiran" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Ratnakiran - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Ratnakiran.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Ratnakiran.mak" CFG="Ratnakiran - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Ratnakiran - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Ratnakiran - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Ratnakiran - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Ratnakiran - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Ratnakiran - Win32 Release"
# Name "Ratnakiran - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AddDialogExport.cpp
# End Source File
# Begin Source File

SOURCE=.\AddDialogStock.cpp
# End Source File
# Begin Source File

SOURCE=.\AppendListDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\DescriptionAddDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\DetailsDialogExport.cpp
# End Source File
# Begin Source File

SOURCE=.\DetailsDialogKundan.cpp
# End Source File
# Begin Source File

SOURCE=.\DetailsDialogStock.cpp
# End Source File
# Begin Source File

SOURCE=.\excel.cpp
# End Source File
# Begin Source File

SOURCE=.\ExcelWrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\ExportSerialsDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\GenericListBoxDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\GenericOneEditDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\GenericTwoEditDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Globals.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MarkedValueFormulaDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\MoveOptionDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Picture.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Ratnakiran.cpp
# End Source File
# Begin Source File

SOURCE=.\Ratnakiran.rc
# End Source File
# Begin Source File

SOURCE=.\RatnakiranDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\RatnakiranStockView.cpp
# End Source File
# Begin Source File

SOURCE=.\RatnakiranTreeView.cpp
# End Source File
# Begin Source File

SOURCE=.\RatnakiranView.cpp
# End Source File
# Begin Source File

SOURCE=.\ReportOptionDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ShortInvoiceDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TableDescription.cpp
# End Source File
# Begin Source File

SOURCE=.\TableItem.cpp
# End Source File
# Begin Source File

SOURCE=.\TableSet.cpp
# End Source File
# Begin Source File

SOURCE=.\TableSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\TableSubItem.cpp
# End Source File
# Begin Source File

SOURCE=.\TableVoucher.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AddDialogExport.h
# End Source File
# Begin Source File

SOURCE=.\AddDialogStock.h
# End Source File
# Begin Source File

SOURCE=.\AppendListDialog.h
# End Source File
# Begin Source File

SOURCE=.\DescriptionAddDialog.h
# End Source File
# Begin Source File

SOURCE=.\DetailsDialogExport.h
# End Source File
# Begin Source File

SOURCE=.\DetailsDialogKundan.h
# End Source File
# Begin Source File

SOURCE=.\DetailsDialogStock.h
# End Source File
# Begin Source File

SOURCE=.\excel.h
# End Source File
# Begin Source File

SOURCE=.\ExcelWrapper.h
# End Source File
# Begin Source File

SOURCE=.\ExportSerialsDialog.h
# End Source File
# Begin Source File

SOURCE=.\GenericListBoxDialog.h
# End Source File
# Begin Source File

SOURCE=.\GenericOneEditDialog.h
# End Source File
# Begin Source File

SOURCE=.\GenericTwoEditDialog.h
# End Source File
# Begin Source File

SOURCE=.\Globals.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MarkedValueFormulaDialog.h
# End Source File
# Begin Source File

SOURCE=.\MoveOptionDialog.h
# End Source File
# Begin Source File

SOURCE=.\Picture.h
# End Source File
# Begin Source File

SOURCE=.\ProgressDialog.h
# End Source File
# Begin Source File

SOURCE=.\Ratnakiran.h
# End Source File
# Begin Source File

SOURCE=.\RatnakiranDoc.h
# End Source File
# Begin Source File

SOURCE=.\RatnakiranStockView.h
# End Source File
# Begin Source File

SOURCE=.\RatnakiranTreeView.h
# End Source File
# Begin Source File

SOURCE=.\RatnakiranView.h
# End Source File
# Begin Source File

SOURCE=.\ReportOptionDialog.h
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
# Begin Source File

SOURCE=.\TableDescription.h
# End Source File
# Begin Source File

SOURCE=.\TableItem.h
# End Source File
# Begin Source File

SOURCE=.\TableSet.h
# End Source File
# Begin Source File

SOURCE=.\TableSettings.h
# End Source File
# Begin Source File

SOURCE=.\TableSubItem.h
# End Source File
# Begin Source File

SOURCE=.\TableVoucher.h
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

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\next.bmp
# End Source File
# Begin Source File

SOURCE=.\res\previous.bmp
# End Source File
# Begin Source File

SOURCE=.\res\print.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Ratnakiran.ico
# End Source File
# Begin Source File

SOURCE=.\res\Ratnakiran.rc2
# End Source File
# Begin Source File

SOURCE=.\res\RatnakiranDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
