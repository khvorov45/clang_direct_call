SCRIPT_DIR=$(dirname "$0")
RUN_BIN=$SCRIPT_DIR/build.exe
clang -g -Wall -Wextra -Wfatal-errors $SCRIPT_DIR/build.c -o $RUN_BIN -lpthread && $RUN_BIN $@
