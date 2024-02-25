#! /bin/tcsh -f 

# compile a single file 

if ($#argv == 0) then
  echo "Usage: compile.sh <module dir> [compiler args] <object names>"
  echo "  module dir is relative to this directory"
  echo "  compiler args will be inserted before source file"
  echo "  object names are without file extension"
  echo "Examples:"
  echo "  compile.sh ../COMMON -DDEBUG=1 alloc qsam metalio"
  exit 1
endif

set moduledir=$1
shift
set options=""

foreach file ($*)
  if ("`echo ${file} | cut -c 1`" == "-") then
    set options="${options} ${file}"
  else
    if ("${options}" == "") then
      echo "compiling ${file}"
    else
      echo "compiling ${file} with options: ${options}"
    endif
#   -DDEBUG=1 -DDEBGLVL=5 -DDEBGPRINT=1 -DDEBGDUMP=1
    xlc -S -DNOIBMHTTP=1 -DIDCAMS_STDOUT_DD='"RTSTIDCO"' -DMETTLE=1 -DSUBPOOL=132 -DMSGPREFIX='"TST"' -qreserved_reg=r12 -qmetal -qroconst -qoffset -qxref  -qgoff -qcsect '-Wc,FLOAT(HEX),LANGLVL(extc99),ARCH(10),TARGET(zOSV2R4),AGG,EXP,SO,LP64,LONGNAME' -qlist=outlist/${file} -o ${file}.s -I ../zowe-common-c/h/ -I ./h/ ${options} ${moduledir}/${file}.c
 endif
end

