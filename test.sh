#!/bin/bash

if [ -d bin/ ]; then
	cd bin
else
	echo "Can not find bin directory."
	exit 1
fi

assert() {
	expected="$1"
	input="$2"

	./c_compiler "$input"
	cc -o 9cc 9cc.s
	./9cc
	actual="$?"

	if [ "$actual" = "$expected" ]; then
		echo "input:$input => actual:$actual"
	else
		echo "input:$input => $expected expected, but got $actual"
		exit 1
	fi
}

assert	0	"0;"
assert	42	"42;"
assert	21	"5+20-4;"
assert	41	" 12 + 34 - 5;"
assert	47	"5+6*7;"
assert	15	"5*(9-6);"
assert	4	"(3+5)/2;"
assert	10	"-10+20;"

# 比較演算子
echo "0=false, 1=true"
assert	0	"0==1;"
assert	1	"42==42;"
assert	1	"0!=1;"
assert	0	"42!=42;"

assert	1	"0<1;"
assert	0	"1<1;"
assert	0	"2<1;"
assert	1	"0<=1;"
assert	1	"1<=1;"
assert	0	"2<=1;"

assert	1	"1>0;"
assert	0	"1>1;"
assert	0	"1>2;"
assert	1	"1>=0;"
assert	1	"1>=1;"
assert	0	"1>=2;"

assert	14	"a = 3;
			 b = 5 * 6 - 8;
			 a + b / 2;"

echo OK
