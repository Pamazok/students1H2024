#! /bin/tcsh -f 

# links a module

if ($#argv == 0) then
  echo "Usage: link.sh <module name> <object names>"
  echo "  module name is the module to link edit"
  echo "  object names are without file extension"
  echo "  Output goes to dataset /$USER.LOADLIB"
  echo "Examples:"
  echo "  link.sh MODULE alloc metalio qsam sync ca1col"
  exit 1
endif

set modulename=` echo $1 | tr "[a-z]" "[A-Z]" `
shift

set objects=""
foreach file ($*)
  # do not append .o to dataset 
  if ("`echo ${file} | cut -c 1-2`" == "//") then
    set objects="${objects} ${file}"
  else
    set objects="${objects} ${file}.o"
  endif
end

echo "Linking module ${modulename}"
setenv _LD_SYSLIB "//'SYS1.CSSLIB'://'CEE.SCEELKEX'://'CEE.SCEELKED'://'CEE.SCEERUN'://'CEE.SCEERUN2'://'CSF.SCSFSTUB'"
ld -V -b ac=0 -b rent -b map -b xref -b reus -e MAIN -o "//'$USER.LOADLIB(${modulename})'" ${objects} > ./outlist/$modulename.link

