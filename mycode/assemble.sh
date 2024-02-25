#! /bin/tcsh -f 

# assemble an object

if ($#argv == 0) then
  echo "Usage: assemble.sh [asm args] <object names>"
  echo "  asm args will be inserted before source file"
  echo "  object names are without file extension" 
  echo "Examples: "
  echo "  assemble.sh alloc qsam metalio"
  exit 1
endif

set options=""
foreach file ($*)
  if ("`echo ${file} | cut -c 1`" == "-") then
    set options="${options} ${file}"
  else
    if ("${options}" == "") then
      echo "assembling ${file}"
    else
      echo "assembling ${file} with options: ${options}"
    endif
    as -mgoff -mobject -mflag=nocont --TERM --RENT -aegimrsx=outlist/${file}.asm -o ${file}.o ${options} ${file}.s
  endif
end
