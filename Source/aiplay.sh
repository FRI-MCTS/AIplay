#!/bin/sh

#echo "MPICC je '$MPICC'"
#echo "MPIRUN je '$MPIRUN'"
#echo "NSLOTS je '$NSLOTS'"

PROGRAM="aiplay"
ARGUMENTI_ZAGON="$@"

#1. Compile
make

echo "MPIRUN:""$MPIRUN"
echo "MPIARGS:""$MPIARGS"
echo "ARG_ZAGON:""$ARGUMENTI_ZAGON"

#2. Zagon programa
$MPIRUN %(MPIARGS) $PROGRAM $ARGUMENTI_ZAGON
#$MPIRUN $MPIARGS $PROGRAM $ARGUMENTI_ZAGON
#$MPIRUN -np 1 $PROGRAM $ARGUMENTI_ZAGON

#3. Uspesen?
exitcode=$?
echo Program je koncal s kodo $exitcode

exit $exitcode

#arcproxy --voms=gen.vo.sling.si:/gen.vo.sling.si

