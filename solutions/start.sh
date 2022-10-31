#!/bin/bash

. $(dirname $0)/include.sh


qemu=qemu-aarch64
gcc_arch=/bin/aarch64-linux-gnu-gcc
lib_arch=/usr/aarch64-linux-gnu/



task=$1
problem=$2


if [ $task -eq "4" ]; then
    # for arm
    task_name=task_0$task
    problem_name=inf-I-0$task-$problem-asm-arm

    souce_folder=./$task_name/$problem_name
    # source_file=$(find $souce_folder -type f \( -name "*ed.s" \))
    source_file=$(find $souce_folder -type f \( -name "*64.s" \))
    test_source_file=$(find $souce_folder -type f \( -name "*test.c" \))

    build_folder=./build/$task_name/$problem_name
    build_file=$build_folder/runner.o
    test_build_file=$build_folder/test_runner.o

    echo -e "${GRN}Found sources ${RESET}$source_file"
    echo -e "${GRN}Found tests ${RESET}$test_source_file"
    if [ ! -d $build_folder ]; then
        echo -e "${GRN}Output folder ${RESET}$build_folder ${GRN}will be created${RESET}"
        mkdir -p $build_folder
    fi
    echo -e "${GRN}Output folder set to ${RESET}$build_folder"

    if [ -f $build_file ]; then
        rm $build_file
        echo -e "${GRN}Removed old file${RESET} $build_file"
    fi

    if [ $problem -eq "0" ]; then
        $gcc_arch -O0 $source_file -c -o $build_file
    else
        $gcc_arch -O0 $source_file -o $build_file
    fi

    if [ -f $build_file ]; then
        echo -e "${GRN}Succefully compiled ${RESET}$build_file"

        if [ -f $test_build_file ]; then
            rm $test_build_file
            echo -e "${GRN}Removed old file${RESET} $test_build_file"
        fi

        if [ $problem -eq "0" ]; then
            $gcc_arch -O0 $test_source_file $source_file -o $test_build_file -I./
        else
            $gcc_arch -O0 $test_source_file -o $test_build_file -I./
        fi

        if [ -f $test_build_file ]; then
            echo -e "${GRN}Succefully compiled ${RESET}$test_build_file"
            chmod +x $build_file
            chmod +x $test_build_file
            report_file=$build_folder/report.txt
            full_build_folder=$(readlink -f $build_folder)
            $qemu -L $lib_arch $test_build_file $full_build_folder $qemu -L $lib_arch > $report_file
            echo -e "${GRN}Results written in${RESET} $report_file"

            echo -e "${GRN}Printing results${RESET}"
            cat $report_file
        else
            echo -e "${RED}Coud not compile tests${RESET}";
        fi

    else
        echo -e "${RED}Looks like error occurred${RESET}";
    fi

elif [ $task -eq "5" ] || [ $task -eq "6" ]; then
    # for intel
    task_name=task_0$task
    problem_name=inf-I-0$task-$problem-asm-x86

    souce_folder=./$task_name/$problem_name
    source_file=$(find $souce_folder -type f \( -name "*64.s" \))
    test_source_file=$(find $souce_folder -type f \( -name "*test.c" \))

    build_folder=./build/$task_name/$problem_name
    build_file=$build_folder/runner.o
    test_build_file=$build_folder/test_runner.o

    echo -e "${GRN}Found sources ${RESET}$source_file"
    echo -e "${GRN}Found tests ${RESET}$test_source_file"
    if [ ! -d $build_folder ]; then
        echo -e "${GRN}Output folder ${RESET}$build_folder ${GRN}will be created${RESET}"
        mkdir -p $build_folder
    fi
    echo -e "${GRN}Output folder set to ${RESET}$build_folder"

    if [ -f $build_file ]; then
        rm $build_file
        echo -e "${GRN}Removed old file${RESET} $build_file"
    fi

    gcc -masm=intel -no-pie -mavx -O0 $source_file -c -o $build_file

    if [ -f $build_file ]; then
        echo -e "${GRN}Succefully compiled ${RESET}$build_file"

        if [ -f $test_build_file ]; then
            rm $test_build_file
            echo -e "${GRN}Removed old file${RESET} $test_build_file"
        fi

        gcc -masm=intel -no-pie -mavx -O0 $test_source_file $source_file -o $test_build_file -I./ -lm

        if [ -f $test_build_file ]; then
            echo -e "${GRN}Succefully compiled ${RESET}$test_build_file"
            chmod +x $build_file
            chmod +x $test_build_file
            report_file=$build_folder/report.txt
            $test_build_file > $report_file
            echo -e "${GRN}Results written in${RESET} $report_file"

            echo -e "${GRN}Printing results${RESET}"
            cat $report_file
        else
            echo -e "${RED}Coud not compile tests${RESET}";
        fi

    else
        echo -e "${RED}Looks like error occurred${RESET}";
    fi
    
fi
