# this is for linux, should work on macos and cygwin also, though I haven't tested it
# in future i might add build scripts for other platforms

# if $1 is not provided, then it will run the repl
FILE=$1
# -z checks if the variable is empty
if [ -z "$FILE" ]
then
    FILE=""
fi

rm -rf build
mkdir build

mkdir build/lib
mkdir build/include
mkdir build/example
cp ./src/*.c build/include/
cp ./lib/* build/lib/ -r
cp ./include/* build/include/ -r
cp ./example/* build/example/ -r
cd build

rm -rf bruter.a

if [ -n "$EXCLUDE" ]; then # EXCLUDE="filename.c" ./build.sh
    cd lib
    rm $EXCLUDE
    echo "excluded libs: $EXCLUDE"
    cd ..
fi

if [ -z "$(ls -A lib)" ]; then
    echo "no libraries to build"
    gcc ./include/main.c ./include/bruter.c -o bruter -O3 -lm -I./include -g
else
    for file in ./lib/*.c; do
        filename="${file##*/}"  
        filename="${filename%.*}" 
        echo "building $filename"
        sed -i "s/<libraries header>/<libraries header>\\nvoid init_$filename(VirtualMachine* vm);/g" include/bruter.h
        sed -i "s/<libraries init>/<libraries init>\\ninit_$filename(vm);/g" include/*.c 
        sed -i "s/<libraries init>/<libraries init>\\ninit_$filename(vm);/g" lib/*.c
    done
    echo "building bruter"
    gcc ./include/bruter.c -c -O3 -lm -I./include -g
    gcc ./lib/*.c -c -O3 -lm -I./include -g
    ar rcs lib/bruter.a *.o
    rm -rf lib/*.c lib/*.o
    gcc ./include/main.c lib/*.a -o bruter -O3 -lm -g
fi

rm -rf *.o
rm -rf lib/*.c
rm -rf include/*.c
rm -rf lib/*.o

valgrind --tool=massif --stacks=yes --detailed-freq=1 --verbose  ./bruter $FILE
ms_print massif.out.* > massif-out.txt
rm -rf massif.out.*

valgrind \
    --leak-check=full \
    --show-leak-kinds=all \
    --track-origins=yes \
    --log-file=valgrind-out.txt \
    --verbose ./bruter $FILE

#valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind-out.txt --verbose build/bruter example/threads.br