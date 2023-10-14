#!/bin/bash

if [ -s bin/c_compiler ]; then
	cd bin
else
	exit1
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

assert 0  0
assert 42 42
assert 21 "5+20-4"
assert 41 " 12 + 34 - 5 "
assert 47 "5+6*7"
assert 15 "5*(9-6)"
assert 4  "(3+5)/2"

echo OK
