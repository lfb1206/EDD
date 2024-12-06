#!/bin/bash

test_dir="T2"
output_dir="outputs"


# Se corre en el directorio donde se encuentra Makefile
current_dir=$(pwd)

while [[ "$current_dir" != "" && ! -e "$current_dir/Makefile" ]]; do
  current_dir=${current_dir%/*}
done

if [ "$current_dir" == "" ]; then
  echo "No se encontro Makefile"
  exit 1
fi

cd $current_dir


# Se ejecutan los test

mkdir -p $output_dir

for program in $(ls $test_dir); do
    if [ ! -d $test_dir/$program ] || [ ! -f $program ]; then
        continue
    fi

    rm -rf $output_dir/$program
    mkdir $output_dir/$program

    for test in $(ls $test_dir/$program); do
        input_file="$test_dir/$program/$test/input.txt"
        expected_output_file="$test_dir/$program/$test/output.txt"

        mkdir $output_dir/$program/$test
        student_output="$output_dir/$program/$test/output.txt"
        student_diff="$output_dir/$program/$test/output.diff"

        timeout 10s ./$program $input_file $student_output > /dev/null 2>&1
        exit_code=$?

        if [ $exit_code -eq 124 ]; then
            echo "$program $test: TIMEOUT"
            continue
        elif [ $exit_code -eq 139 ]; then
            echo "$program $test: SEGMENTATION FAULT"
            continue
        elif [ $exit_code -ne 0 ]; then
            echo "$program $test: ERROR (exit code $exit_code)"
            continue
        fi

        diff $expected_output_file $student_output > $student_diff

        if [ $? -ne 0 ]; then
            echo "$program $test: INCORRECT OUTPUT"
        else
            echo "$program $test: CORRECT OUTPUT"
        fi
    done
done
