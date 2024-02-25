#! /bin/tcsh -f
 
set COMMON=../zowe-common-c/c
set MYCODE=./c
set JWT=./zowe-common-c/jwt/jwt
set RCCRYPTO=./zowe-common-c/jwt/rscrypto
set DOLLAR='$'
 
foreach module ( `echo $1 | tr ',' ' '` )
 
  echo "About to build '${module}':"
 
  if ("$module" == "test") then
    echo "Test"
  endif
 
  if ("$module" == "ussTest") then
 
    xlc -D_XOPEN_SOURCE=600 "-Wc,langlvl(extc99),agg,exp,list(),so(),gonum,goff,hgpr,roconst,list,ASM,asmlib('SYS1.MACLIB')" \
        '-Wl,ac=0' -I ../h -I ../zowe-common-c/h/ -o ussTest ./c/test.c \
        ../zowe-common-c/c/alloc.c ../zowe-common-c/c/utils.c ../zowe-common-c/c/timeutls.c ../zowe-common-c/c/zos.c
 
    ./ussTest
 
  endif

  if ("$module" == "metalTest") then
 
    ./compile64.sh $COMMON alloc utils timeutls zos metalio qsam
    ./assemble.sh alloc utils timeutls zos metalio qsam

    ./compile64.sh $MYCODE test
    ./assemble.sh test

    ./link.sh MTLTEST test alloc utils timeutls zos metalio qsam

    submit metalTest.jcl
 
  endif

  if ("$module" == "metalHTTP") then

    ./compile64.sh $COMMON alloc utils timeutls zos metalio qsam \
                   stcbase scheduling collections socketmgmt mtlskt \
                   logging recovery le xlate http httpserver json zosfile \
                   charsets crypto impersonation xml icsf
    ./assemble.sh alloc utils timeutls zos metalio qsam \
                   stcbase scheduling collections socketmgmt mtlskt \
                   logging recovery le xlate http httpserver json zosfile \
                   charsets crypto impersonation xml icsf

    ./compile64.sh $MYCODE myhttp
    ./assemble.sh myhttp

    ./link.sh MTLHTTP myhttp alloc utils timeutls zos metalio qsam \
                   stcbase scheduling collections socketmgmt mtlskt \
                   logging recovery le xlate http httpserver json zosfile \
                   charsets crypto impersonation xml icsf

    # submit metalHTTP.jcl

  endif

  if ("$module" == "http") then
 
    xlc -D_XOPEN_SOURCE=600 "-Wc,langlvl(extc99),agg,exp,list(),so(),gonum,goff,hgpr,roconst,list,ASM,asmlib('SYS1.MACLIB')" \
        '-Wl,ac=0' -I ../h -I ../zowe-common-c/h/ -o myHTTP ./c/myhttp.c \
        ../zowe-common-c/c/alloc.c ../zowe-common-c/c/utils.c ../zowe-common-c/c/timeutls.c ../zowe-common-c/c/zos.c \
        ../zowe-common-c/c/stcbase.c ../zowe-common-c/c/scheduling.c ../zowe-common-c/c/collections.c ../zowe-common-c/c/socketmgmt.c
 
    ./myHTTP
 
  endif
 
end