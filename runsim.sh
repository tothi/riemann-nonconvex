#!/bin/bash

BDIR=`pwd`
SDIR=$BDIR/src
MDIR=$BDIR/mpg
TDIR=/tmp/`whoami`/sim_tmp
cd $SDIR

######################################################################

N=5000
A=0
B=0.0176654
DT_SHOT=5000
SHOTS=5000
SUM=250

echo -n "[+] Compiling sim.c..."

rm sim.o sim > /dev/null 2>&1
gcc sim.c -o sim -O3 -lgsl -lcblas -lm \
    -DN=$N \
    -DA=$A \
    -DB=$B \
    -DDT_SHOT=$DT_SHOT \
    -DSHOTS=$SHOTS \
    -DSUM=$SUM

echo "done."

######################################################################

function make_anim {

SDATE=`date +%s`

echo -n "[*] ############ SIM: b=$B vl=$1 vr=$2 DT_SHOT=$DT_SHOT SHOTS=$SHOTS SUM=$SUM ############ "

rm -fr $TDIR2 > /dev/null 2>&1
mkdir -p $TDIR2 > /dev/null 2>&1
cd $TDIR2

GSL_RNG_SEED=$RANDOM $SDIR/sim sim.out $1 $2

BASENAME="plot"

echo -n "[+] Splitting data..."
split -$N -d -a5 sim.out ${BASENAME}. && rm sim.out
echo "done."

echo -n "[+] Plotting to screen..."
plotfile="sim-view.gnuplot"
echo "set yrange [-1:1]" > $plotfile
for i in ${BASENAME}.* ; do
  echo "plot '$i' smooth unique with lines lt 1" >> $plotfile
done
#echo "pause -1" >> $plotfile

#gnuplot -noraise $plotfile
echo "done."

echo -n "[+] Making png files..."
plotfile="sim-png.gnuplot"
echo "set yrange [-1:1]" > $plotfile
echo "set terminal png" >> $plotfile
for i in ${BASENAME}.* ; do
  echo "set output \"$i.png\"" >> $plotfile
  echo "plot '$i' smooth unique with lines lt 1" >> $plotfile
done

#gnuplot -noraise $plotfile
gnuplot $plotfile
echo "done."

echo -n "[+] Converting png files to jpeg..."
for i in ${BASENAME}.*.png ; do
  convert $i -quality 100 ${i%.png}.jpg
  rm $i
done
echo "done."

echo -n "[+] Assembling mpeg file..."
jpeg2yuv -f25 -Ip -v0 -j${BASENAME}.%05d.jpg | \
  mpeg2enc -v0 -a2 -b3000 -f3 -o ${BASENAME}.mpg && rm ${BASENAME}.*.jpg
echo "done."

mv ${BASENAME}.mpg $3

echo "[*] Processing time: `date -d "0 $(date +%s) sec - ${SDATE} sec" +"%H hours %M minutes %S seconds"`"

echo
}

######################################################################

function anim_with_symlink {
  TDIR2="${TDIR}/$3"
  make_anim $1 $2 $MDIR/sim_${B}_$1_$2.mpg
  cd $MDIR
  ln -sf sim_${B}_$1_$2.mpg sim_$3.mpg
}

######################################################################

[ -d $MDIR ] || mkdir -p $MDIR

######################################################################


anim_with_symlink  0.8  -0.65 RSR-S
anim_with_symlink  0.75  0    RS-S
#anim_with_symlink -0.4   0.5  SRS-RS
#anim_with_symlink  0.7   0.5  R-S
#anim_with_symlink  0.15 -0.55 SR-SR
#anim_with_symlink -0.85  0.85 S-RSR
#anim_with_symlink -0.1   0.3  RS-S2
#anim_with_symlink -0.74 -0.4  S-R


######################################################################

exit 0
