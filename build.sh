# this is for linux, should work on macos and cygwin also, though I haven't tested it
# in future i might add build scripts for other platforms

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

if [ -z "$CC" ]; then 
    CC="gcc"
fi

echo "using $CC as compiler"

if [ -n "$EXCLUDE" ]; then # EXCLUDE="filename.c" ./build.sh
    cd lib
    rm $EXCLUDE
    echo "excluded libs: $EXCLUDE"
    cd ..
fi

#if all libs are excluded we don't need to do anything but build the main file
if [ -z "$(ls -A lib)" ]; then
    echo "no libraries to build"
    $CC ./include/main.c ./include/bruter.c -o bruter -O3 -lm -I./include
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

    # experimental
    if [ -n "$WASICC" ]; then
        $WASMCC -o bruter.wasm ./include/main.c ./include/bruter.c ./lib/*.c -O3 -lm -I./include
        echo 'wasmtime --dir=. bruter.wasm $@' > run_bruter.sh && chmod +x run_bruter.sh
    fi

    # experimental
    if [ -n "$EMCC" ]; then
        mkdir web
        cd web
        $EMCC -o index.html ../include/main.c ../include/bruter.c ../lib/*.c -O3 -lm -I../include
        cd ..
    fi

    $CC ./include/bruter.c -c -O3 -lm -I./include
    $CC ./lib/*.c -c -O3 -lm -I./include 
    ar rcs lib/bruter.a *.o
    rm -rf lib/*.c lib/*.o
    $CC ./include/main.c lib/*.a -o bruter -O3 -lm
fi



rm -rf *.o
rm -rf lib/*.c
rm -rf lib/*.o
rm -rf include/*.c