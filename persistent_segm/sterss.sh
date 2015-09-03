#!/usr/bin/env bash


for i in $(seq 1 1 1000); do
	./TestGen | tee >(./stupid > ans.cheker) | ./main > ans.kStat
	diff ans.cheker ans.kStat
	echo "test done"
done
