#!/bin/bash
PACKAGE="bruter"

debug()
{
    if [ -z "$1" ]; then
        echo "you were suposed to pass a filename."
        echo "aborting..."
        exit
    else
        FILENAME="$1"
    fi

    cpp -P "$FILENAME" > "$FILENAME.br"
    ./utils/bruter-builder "$FILENAME.br" lib/std/*.c > "$FILENAME.debug.c"
    gcc -o $FILENAME.exe "$FILENAME.debug.c" -O3 -g -lm -I./
    valgrind \
    --leak-check=full \
    --show-leak-kinds=all \
    --track-origins=yes \
    --log-file=./valgrind-out.txt \
    --verbose ./$FILENAME.exe
}

install()
{
    sudo cp bruter.h /usr/local/include/
    sudo cp utils/bruter-lazy.h /usr/local/include/
    sudo cp utils/bruter-build /usr/local/bin/
}

"$@"