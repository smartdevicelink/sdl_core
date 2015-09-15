# Microsoft Developer Studio Generated NMAKE File, Based on xml.dsp
!IF "$(CFG)" == ""
CFG=xml - Win32 Debug
!MESSAGE No configuration specified. Defaulting to xml - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "xml - Win32 Release" && "$(CFG)" != "xml - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "xml.mak" CFG="xml - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "xml - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "xml - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "xml - Win32 Release"

OUTDIR=.\LibR
INTDIR=.\LibR
# Begin Custom Macros
OutDir=.\LibR
# End Custom Macros

ALL : ".\expat.h" ".\config.h" "$(OUTDIR)\xml.lib"


CLEAN :
	-@erase "$(INTDIR)\xml_src.idb"
	-@erase "$(INTDIR)\xml_src.pdb"
	-@erase "$(INTDIR)\xmlparse.obj"
	-@erase "$(INTDIR)\xmlrole.obj"
	-@erase "$(INTDIR)\xmltok.obj"
	-@erase "$(OUTDIR)\xml.lib"
	-@erase ".\config.h"
	-@erase ".\expat.h"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /Zi /O2 /Oy- /I "." /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D VERSION=\"expat_1.95.2\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\xml_src" /FD /c 

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

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\xml.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\xml.lib" 
LIB32_OBJS= \
	"$(INTDIR)\xmlparse.obj" \
	"$(INTDIR)\xmlrole.obj" \
	"$(INTDIR)\xmltok.obj"

"$(OUTDIR)\xml.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "xml - Win32 Debug"

OUTDIR=.\LibD
INTDIR=.\LibD
# Begin Custom Macros
OutDir=.\LibD
# End Custom Macros

ALL : ".\expat.h" ".\config.h" "$(OUTDIR)\xml.lib"


CLEAN :
	-@erase "$(INTDIR)\xml_src.idb"
	-@erase "$(INTDIR)\xml_src.pdb"
	-@erase "$(INTDIR)\xmlparse.obj"
	-@erase "$(INTDIR)\xmlrole.obj"
	-@erase "$(INTDIR)\xmltok.obj"
	-@erase "$(OUTDIR)\xml.lib"
	-@erase ".\config.h"
	-@erase ".\expat.h"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Zi /Od /I "." /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D VERSION=\"expat_1.95.2\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\xml_src" /FD /EHsc /c 

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

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\xml.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\xml.lib" 
LIB32_OBJS= \
	"$(INTDIR)\xmlparse.obj" \
	"$(INTDIR)\xmlrole.obj" \
	"$(INTDIR)\xmltok.obj"

"$(OUTDIR)\xml.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("xml.dep")
!INCLUDE "xml.dep"
!ELSE 
!MESSAGE Warning: cannot find "xml.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "xml - Win32 Release" || "$(CFG)" == "xml - Win32 Debug"
SOURCE=.\xmlparse.c

"$(INTDIR)\xmlparse.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\xmlrole.c

"$(INTDIR)\xmlrole.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\xmltok.c

"$(INTDIR)\xmltok.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=xmltok_impl.c
SOURCE=xmltok_ns.c
SOURCE=.\expat.h.in

!IF  "$(CFG)" == "xml - Win32 Release"

InputPath=.\expat.h.in

".\expat.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	type .\expat.h.in > .\expat.h
<< 
	

!ELSEIF  "$(CFG)" == "xml - Win32 Debug"

InputPath=.\expat.h.in

".\expat.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	type .\expat.h.in > .\expat.h
<< 
	

!ENDIF 

SOURCE=.\winconfig.h

!IF  "$(CFG)" == "xml - Win32 Release"

InputPath=.\winconfig.h

".\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	type .\winconfig.h > .\config.h
<< 
	

!ELSEIF  "$(CFG)" == "xml - Win32 Debug"

InputPath=.\winconfig.h

".\config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	type .\winconfig.h > .\config.h
<< 
	

!ENDIF 


!ENDIF 

