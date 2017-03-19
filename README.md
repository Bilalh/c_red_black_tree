# Red Black Tree in C

## Requeiments:

Tested on MacOS 10.10 and Ubuntu 16.04 using `clang`.

## Usage

build:

	make

usage example:

	./bin/rb_usage

See `rb_usage.c` for details


For an example of converting a sorted array to a red black tree run:

	./bin/rb_array

and see `rb_array.c` for details.


The code to produce the printed trees is in `rb_print`.

	|100-----
	|       |50------
	|       |       |22------
	|       |       |       |11
	|       |       |       |33------
	|       |       |       |       |25
	|       |       |75
	|       |150-----
	|       |       |125-----
	|       |       |       |110
	|       |       |175