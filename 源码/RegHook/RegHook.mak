# Microsoft Developer Studio Generated NMAKE File, Based on RegHook.dsp
!IF "$(CFG)" == ""
CFG=RegHook - Win32 Debug
!MESSAGE No configuration specified. Defaulting to RegHook - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "RegHook - Win32 Release" && "$(CFG)" != "RegHook - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RegHook.mak" CFG="RegHook - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RegHook - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RegHook - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "RegHook - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\RegHook.dll"


CLEAN :
	-@erase "$(INTDIR)\RegHook.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\RegHook.dll"
	-@erase "$(OUTDIR)\RegHook.exp"
	-@erase "$(OUTDIR)\RegHook.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "REGHOOK_EXPORTS" /D "_WINDLL" /Fp"$(INTDIR)\RegHook.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\RegHook.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /dll /incremental:no /pdb:"$(OUTDIR)\RegHook.pdb" /machine:I386 /nodefaultlib:"LIBC.lib" /def:".\RegHook.def" /out:"$(OUTDIR)\RegHook.dll" /implib:"$(OUTDIR)\RegHook.lib" 
DEF_FILE= \
	".\RegHook.def"
LINK32_OBJS= \
	"$(INTDIR)\RegHook.obj"

"$(OUTDIR)\RegHook.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "RegHook - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\RegHook.dll" "$(OUTDIR)\RegHook.bsc"


CLEAN :
	-@erase "$(INTDIR)\RegHook.obj"
	-@erase "$(INTDIR)\RegHook.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\RegHook.bsc"
	-@erase "$(OUTDIR)\RegHook.dll"
	-@erase "$(OUTDIR)\RegHook.exp"
	-@erase "$(OUTDIR)\RegHook.ilk"
	-@erase "$(OUTDIR)\RegHook.lib"
	-@erase "$(OUTDIR)\RegHook.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "REGHOOK_EXPORTS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\RegHook.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\RegHook.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\RegHook.sbr"

"$(OUTDIR)\RegHook.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\RegHook.pdb" /debug /machine:I386 /def:".\RegHook.def" /out:"$(OUTDIR)\RegHook.dll" /implib:"$(OUTDIR)\RegHook.lib" /pdbtype:sept 
DEF_FILE= \
	".\RegHook.def"
LINK32_OBJS= \
	"$(INTDIR)\RegHook.obj"

"$(OUTDIR)\RegHook.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("RegHook.dep")
!INCLUDE "RegHook.dep"
!ELSE 
!MESSAGE Warning: cannot find "RegHook.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "RegHook - Win32 Release" || "$(CFG)" == "RegHook - Win32 Debug"
SOURCE=.\RegHook.cpp

!IF  "$(CFG)" == "RegHook - Win32 Release"


"$(INTDIR)\RegHook.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "RegHook - Win32 Debug"


"$(INTDIR)\RegHook.obj"	"$(INTDIR)\RegHook.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

