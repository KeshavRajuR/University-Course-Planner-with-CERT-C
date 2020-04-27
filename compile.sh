#!/bin/bash

for i in $(ls | grep .c$)
do
	gcc -c $i
done

gcc *.o -o output
