SCRIPT_DIR=$(dirname "$0")
RUN_BIN=$SCRIPT_DIR/tests.exe
clang -g -Wall -Wextra -Wfatal-errors $SCRIPT_DIR/tests.c -o $RUN_BIN -lpthread && $RUN_BIN $@
