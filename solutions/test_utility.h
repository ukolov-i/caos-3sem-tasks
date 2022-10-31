#ifndef TEST_UTIL_H
#define TEST_UTIL_H
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-truncation"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>


// Colors
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

// Compilation params
#define NOT_OPTIMIZE __attribute__((optimize(0)))
#define STATIC_ASSERT(cond, msg) \
    {typedef char test[(cond) ? 1: -1]; (void) (test*) #msg;} (void)0

// Floating-point arithmetic
#define DOUBLE_TOLERANCE ((double)(60) * __DBL_EPSILON__)
#define EQUAL_DOUBLE(left, right) (fabs(left - right) < DOUBLE_TOLERANCE)
#define FLOAT_TOLERANCE ((float)(60) * __FLT_EPSILON__)
#define EQUAL_FLOAT(left, right) (fabsf(left - right) < FLOAT_TOLERANCE)

// Test utility
#define TEST(i)                 NOT_OPTIMIZE void test##i()
#define LOG_STARTING_TESTS()    LOG(" --- STARTING TESTS --- ")
#define LOG_FINISHED_TESTS()    LOG(" --- FINISHED TESTS --- ")
#define START_TEST(i)           LOG("RUNNING TEST "#i);

#define BENCHMARK(i)               NOT_OPTIMIZE void benchmark##i()
#define LOG_STARTING_BENCMARKS()   LOG(" --- STARTING BENCHMARKS --- ")
#define LOG_FINISHED_BENCMARKS()   LOG(" --- FINISHED BENCHMARKS --- ")
#define START_BENCHMARK(i)         LOG("RUNNING BENCHMARK "#i);

// Loging
#define LOG(i)          printf(BLU "\n"i"\n" RESET)
#define LOG_DEBUG(i)    printf(YEL " "i"\n" RESET)
#define LOG_SUCCESS(i)  printf(GRN " "i"\n" RESET)
#define LOG_FAILURE(i)  printf(RED " "i"\n" RESET)
#define LOG_CONDITION(cond, success_msg, failure_msg, fmt, ...) \
    if (cond) { \
        LOG_SUCCESS(success_msg); \
    } else { \
        LOG_FAILURE(failure_msg); \
        printf(fmt, ##__VA_ARGS__); \
    }
#define LOG_RESULT(cond, fmt, ...) LOG_CONDITION(cond, "PASSED", "FAILED", fmt, __VA_ARGS__)

// IO managment
#define OPEN_INPUT() \
    input_ptr = fopen(INPUT_FILE, "w"); \
    if (input_ptr == NULL) { \
        LOG_FAILURE("COULD NOT OPEN THE FILE"); \
        printf("%s\n", INPUT_FILE); \
        exit(1); \
    }

#define OPEN_OUPUT() \
    output_ptr = fopen(OUTPUT_FILE, "r"); \
    if (output_ptr == NULL) { \
        LOG_FAILURE("COULD NOT OPEN THE FILE"); \
        printf("%s\n", OUTPUT_FILE); \
        exit(1); \
    }

#define CLOSE_INPUT() fclose(input_ptr);
#define CLOSE_OUTPUT() fclose(output_ptr);
#define WRITE_INPUT(fmt, ...) fprintf(input_ptr, fmt, ##__VA_ARGS__)
#define READ_OUTPUT(fmt, ...) while (fscanf(output_ptr, fmt, ##__VA_ARGS__) != EOF)
#define WRITE_INPUT_SWEEP(fmt, ...) \
    OPEN_INPUT(); \
    WRITE_INPUT(fmt, __VA_ARGS__); \
    CLOSE_INPUT()
#define READ_OUTPUT_SWEEP(fmt, ...) \
    OPEN_OUPUT(); \
    READ_OUTPUT(fmt, __VA_ARGS__); \
    CLOSE_OUTPUT()



char WORKING_DIR[PATH_MAX];
char RUNNER_FILE[PATH_MAX];
char SET_PERMISS[PATH_MAX];
char EXEC_RUNNER[PATH_MAX];

char INPUT_FILE[PATH_MAX];
char OUTPUT_FILE[PATH_MAX];
FILE* input_ptr = NULL;
FILE* output_ptr = NULL;

char QEMU_PATH[PATH_MAX];
char QEMU_FLAGS[PATH_MAX];
char LIB_ARCH_PATH[PATH_MAX];




void set_source() {
    strcpy(RUNNER_FILE, WORKING_DIR);
    strcat(RUNNER_FILE, "/runner.o");
}

void set_permissions() {
    strcpy(SET_PERMISS, "chmod +x ");
    strcat(SET_PERMISS, RUNNER_FILE);

    system(SET_PERMISS);
}

void set_qemu(char** argv) {
    strcpy(QEMU_PATH, argv[2]);
    strcpy(QEMU_FLAGS, argv[3]);
    strcpy(LIB_ARCH_PATH, argv[4]);

    strcat(strcat(QEMU_PATH, " "), QEMU_FLAGS);
    strcat(strcat(QEMU_PATH, " "), LIB_ARCH_PATH);
}

void set_io_files() {
    strcpy(INPUT_FILE, WORKING_DIR);
    strcat(INPUT_FILE, "/tmp_input.txt");
    strcpy(OUTPUT_FILE, WORKING_DIR);
    strcat(OUTPUT_FILE, "/tmp_output.txt");
}

void read_output_sweeps(char* buffer) {
    OPEN_OUPUT();
    int status = 0;
    int i = 0;
    while (status != EOF) {
        status = fscanf(output_ptr, "%c", &buffer[i]);
        ++i;
    }
    buffer[i - 1] = '\0';
    CLOSE_OUTPUT();
}

void read_output_sweepi(int* buffer) {
    OPEN_OUPUT();
    int status = 0;
    int i = 0;
    while (status != EOF) {
        status = fscanf(output_ptr, "%d", &buffer[i]);
        ++i;
    }
    CLOSE_OUTPUT();
}

void set_exec_io() {
    strcat(strcat(EXEC_RUNNER, " < "), INPUT_FILE);
    strcat(strcat(EXEC_RUNNER, " > "), OUTPUT_FILE);
}

void init_arch(int argc, char** argv) {
    if (argc < 5) {
        LOG_FAILURE("NOT ENOUGH ARGUMENTS");
        exit(1);
    }

    strcpy(WORKING_DIR, argv[1]);

    set_source();
    set_permissions();
    set_qemu(argv);
    set_io_files();

    strcpy(EXEC_RUNNER, QEMU_PATH);
    strcat(strcat(EXEC_RUNNER, " "), RUNNER_FILE);    

    set_exec_io();
}

void run_arch() {
    int status = system(EXEC_RUNNER);
    if (status == -1) {
        LOG_FAILURE("EXECUTION FAILED");
        printf(MAG "%s\n" RESET, EXEC_RUNNER);
        exit(1);
    }
}

void quit() {
    if (remove(INPUT_FILE)) {
        LOG_FAILURE("COULD NOT DELETE THE FILE");
    }
    if (remove(OUTPUT_FILE)) {
        LOG_FAILURE("COULD NOT DELETE THE FILE");
    }
}

#pragma GCC diagnostic pop
#endif