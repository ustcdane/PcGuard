# Microsoft Developer Studio Generated NMAKE File, Based on RegMonitor.dsp
!IF "$(CFG)" == ""
CFG=RegMonitor - Win32 Debug
!MESSAGE No configuration specified. Defaulting to RegMonitor - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "RegMonitor - Win32 Release" && "$(CFG)" != "RegMonitor - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RegMonitor.mak" CFG="RegMonitor - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RegMonitor - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "RegMonitor - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "RegMonitor - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\RegMonitor.exe" "$(OUTDIR)\RegMonitor.bsc"


CLEAN :
	-@erase "$(INTDIR)\RegMonitor.obj"
	-@erase "$(INTDIR)\RegMonitor.pch"
	-@erase "$(INTDIR)\RegMonitor.res"
	-@erase "$(INTDIR)\RegMonitor.sbr"
	-@erase "$(INTDIR)\RegMonitorDlg.obj"
	-@erase "$(INTDIR)\RegMonitorDlg.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\RegMonitor.bsc"
	-@erase "$(OUTDIR)\RegMonitor.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\RegMonitor.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\RegMonitor.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\RegMonitor.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\RegMonitor.sbr" \
	"$(INTDIR)\RegMonitorDlg.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\RegMonitor.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\RegMonitor.pdb" /machine:I386 /out:"$(OUTDIR)\RegMonitor.exe" 
LINK32_OBJS= \
	"$(INTDIR)\RegMonitor.obj" \
	"$(INTDIR)\RegMonitorDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\RegMonitor.res"

"$(OUTDIR)\RegMonitor.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "RegMonitor - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\RegMonitor.exe"


CLEAN :
	-@erase "$(INTDIR)\RegMonitor.obj"
	-@erase "$(INTDIR)\RegMonitor.pch"
	-@erase "$(INTDIR)\RegMonitor.res"
	-@erase "$(INTDIR)\RegMonitorDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\RegMonitor.exe"
	-@erase "$(OUTDIR)\RegMonitor.ilk"
	-@erase "$(OUTDIR)\RegMonitor.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\RegMonitor.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\RegMonitor.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\RegMonitor.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\RegMonitor.pdb" /debug /machine:I386 /out:"$(OUTDIR)\RegMonitor.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\RegMonitor.obj" \
	"$(INTDIR)\RegMonitorDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\RegMonitor.res"

"$(OUTDIR)\RegMonitor.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("RegMonitor.dep")
!INCLUDE "RegMonitor.dep"
!ELSE 
!MESSAGE Warning: cannot find "RegMonitor.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "RegMonitor - Win32 Release" || "$(CFG)" == "RegMonitor - Win32 Debug"
SOURCE=.\RegMonitor.cpp

!IF  "$(CFG)" == "RegMonitor - Win32 Release"


"$(INTDIR)\RegMonitor.obj"	"$(INTDIR)\RegMonitor.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\RegMonitor.pch"


!ELSEIF  "$(CFG)" == "RegMonitor - Win32 Debug"


"$(INTDIR)\RegMonitor.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\RegMonitor.pch"


!ENDIF 

SOURCE=.\RegMonitor.rc

"$(INTDIR)\RegMonitor.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\RegMonitorDlg.cpp

!IF  "$(CFG)" == "RegMonitor - Win32 Release"


"$(INTDIR)\RegMonitorDlg.obj"	"$(INTDIR)\RegMonitorDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\RegMonitor.pch"


!ELSEIF  "$(CFG)" == "RegMonitor - Win32 Debug"


"$(INTDIR)\RegMonitorDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\RegMonitor.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "RegMonitor - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\RegMonitor.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\RegMonitor.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "RegMonitor - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\RegMonitor.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\RegMonitor.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

