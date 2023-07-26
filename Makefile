# Env variable CC (C Compiler), if CC is not assigned it will be gcc. To set in terminal use: export CC=clang
CC ?= gcc

# Prepare env variables for set of flags for compiler, C_WARNS should be assigned dependent on which compiler was choosed
C_STD   := -std=c99
C_OPT   := -O3
C_FLAGS :=
C_WARNS :=
RM		:= rm -rf

VALGRIND_MEMCHECK := valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes --error-exitcode=1
VALGRIND_CACHE := valgrind --tool=cachegrind --branch-sim=yes

# If CC is set as clang use this warnings otherwise if its cc or gcc use listed under else
ifeq ($(CC),clang)
	C_WARNS +=  -Weverything -Wno-padded -Wno-gnu-zero-variadic-macro-arguments -Wno-newline-eof -Wno-reserved-id-macro \
				-Wno-missing-variable-declarations -Wno-declaration-after-statement
else ifneq (, $(filter $(CC), cc gcc))
	C_WARNS += -Wall -Wextra -pedantic -Wcast-align \
			   -Winit-self -Wlogical-op -Wmissing-include-dirs \
			   -Wredundant-decls -Wshadow -Wstrict-overflow=5 -Wundef  \
			   -Wwrite-strings -Wpointer-arith -Wmissing-declarations \
			   -Wuninitialized -Wno-old-style-definition -Wno-old-style-declaration -Wstrict-prototypes \
			   -Wmissing-prototypes -Wswitch-default -Wbad-function-cast \
			   -Wnested-externs -Wconversion -Wunreachable-code
endif

# If statement "DEBBUG=1" will be placed next to some make command, properly set env variable GGDB and change optimization flag
ifeq ("$(origin DEBUG)", "command line")
	GGDB := -ggdb3
	C_OPT := -O0
else
	GGDB :=
endif

# If statement "VERBOSE=1" will be placed next to some make command, it wll print what is executed under make command
ifeq ("$(origin VERBOSE)", "command line")
	QUIET :=
else
	QUIET ?= @
endif

C_FLAGS += $(C_STD) $(C_OPT) $(GGDB) $(C_WARNS)

all: 
	$(QUIET) $(CC) $(C_FLAGS) ./app/*.c ./src/*.c -I./include -o main.out

.PHONY: test
test:
	$(QUIET)$(MAKE) unit_test --no-print-directory
	$(QUIET)$(MAKE) mocks_test --no-print-directory
	
.PHONY: test_run
test_run:
	./unit_test.out
	./mocks_test.out
	
.PHONY: unit_tests
unit_test:
	$(QUIET) $(CC) $(C_FLAGS) -ggdb3 -O0 -Wl,-allow-multiple-definition src/*.c test/unit/*.c -I./include -I./src -o unit_test.out
	
.PHONY: mocks_test
mocks_test:
	$(QUIET) $(CC) $(C_FLAGS) -ggdb3 -O0 -Wl,-allow-multiple-definition src/*.c test/mocks/*.c -I./include -I./src -o mocks_test.out

.PHONY: memcheck
memcheck:
	$(QUIET)$(MAKE) test --no-print-directory
	$(QUIET)$(VALGRIND_MEMCHECK) ./unit_test.out
	$(QUIET)$(VALGRIND_MEMCHECK) ./mocks_test.out

.PHONY: clean
clean: 
	$(QUIET) $(RM) *.out

