#!/bin/sh

#echo "MPICC je '$MPICC'"
#echo "MPIRUN je '$MPIRUN'"
#echo "NSLOTS je '$NSLOTS'"

PROGRAM="aiplay"
ARGUMENTI_ZAGON="$@"                   #parametri, dobljeni iz xrsl datoteke

#1. Compile
make

echo "MPIRUN:""$MPIRUN"
echo "MPIARGS:""$MPIARGS"

#2. Zagon programa
$MPIRUN $MPIARGS ${PWD}/$PROGRAM $ARGUMENTI_ZAGON

#3. Uspesen?
exitcode=$?
echo Program je koncal s kodo $exitcode

exit $exitcode

#arcproxy --voms=gen.vo.sling.si:/gen.vo.sling.si

