#!/bin/sh

. $TESTDIR/lib.sh

make_eduos

check() {
	time $1 | \
		awk 'NR == 1 { l = $1 } NR == 2 { if ($1 - l >= 2.0) print "OK" }' | \
		grep "OK"
}

#check /bin/sh < $CDIR/1.sh 
check ./image < $CDIR/1.in 
