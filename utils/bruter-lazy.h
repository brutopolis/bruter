#ifndef BRUTER_LAZY_H
#define BRUTER_LAZY_H 1

#define BRUTER_LAZY_VERSION "0.3.2"

// bruter lazy - a lazy bruter-code generator
// we will limit args to 16
#define REVERSE_1(a) a
#define REVERSE_2(a, b) b a
#define REVERSE_3(a, b, c) c b a
#define REVERSE_4(a, b, c, d) d c b a
#define REVERSE_5(a, b, c, d, e) e d c b a
#define REVERSE_6(a, b, c, d, e, f) f e d c b a
#define REVERSE_7(a, b, c, d, e, f, g) g f e d c b a
#define REVERSE_8(a, b, c, d, e, f, g, h) h g f e d c b a
#define REVERSE_9(a, b, c, d, e, f, g, h, i) i h g f e d c b a
#define REVERSE_10(a, b, c, d, e, f, g, h, i, j) j i h g f e d c b a
#define REVERSE_11(a, b, c, d, e, f, g, h, i, j, k) k j i h g f e d c b a
#define REVERSE_12(a, b, c, d, e, f, g, h, i, j, k, l) l k j i h g f e d c b a
#define REVERSE_13(a, b, c, d, e, f, g, h, i, j, k, l, m) m l k j i h g f e d c b a
#define REVERSE_14(a, b, c, d, e, f, g, h, i, j, k, l, m, n) n m l k j i h g f e d c b a
#define REVERSE_15(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o) o n m l k j i h g f e d c b a
#define REVERSE_16(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p) p o n m l k j i h g f e d c b a
#define GET_REVERSE_MACRO(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, NAME, ...) NAME
#define REVERSE(...) GET_REVERSE_MACRO(__VA_ARGS__, REVERSE_16, REVERSE_15, REVERSE_14, REVERSE_13, REVERSE_12, REVERSE_11, REVERSE_10, REVERSE_9, REVERSE_8, REVERSE_7, REVERSE_6, REVERSE_5, REVERSE_4, REVERSE_3, REVERSE_2, REVERSE_1)(__VA_ARGS__)

// 
#define context @
#define code %
#define stack &

// concat
#define concat_(a, b) a##b
#define concat(a, b) concat_(a, b)

// calling functions
#define call(a, ...) REVERSE(__VA_ARGS__) a ! 
#define print(...) call(print, __VA_ARGS__)

#define static(...) concat($, __VA_ARGS__)
#define label(...) :__VA_ARGS__

// 
#define ifgo(condition, destination) destination condition ?

// list.c
#define list(...)           call(list, __VA_ARGS__)
#define pop(...)            call(pop, __VA_ARGS__)
#define push(...)           call(push, __VA_ARGS__)
#define shift(...)          call(shift, __VA_ARGS__)
#define unshift(...)        call(unshift, __VA_ARGS__)
#define insert(...)         call(insert, __VA_ARGS__)
#define remove(...)         call(remove, __VA_ARGS__)
#define get(...)            call(get, __VA_ARGS__)
#define set(...)            call(set, __VA_ARGS__)
#define where(...)          call(where, __VA_ARGS__)
#define find(...)           call(find, __VA_ARGS__)
#define length(...)         call(length, __VA_ARGS__)
#define copy(...)           call(copy, __VA_ARGS__)
#define swap(...)           call(swap, __VA_ARGS__)
#define reverse(...)        call(reverse, __VA_ARGS__)
#define point(...)          call(point, __VA_ARGS__)
#define alloc(...)          call(alloc, __VA_ARGS__)
#define dup(...)            call(dup, __VA_ARGS__)
#define free(...)           call(free, __VA_ARGS__)
#define nameof(...)         call(nameof, __VA_ARGS__)
#define typeof(...)         call(typeof, __VA_ARGS__)
#define rename(...)         call(rename, __VA_ARGS__)
#define retype(...)         call(retype, __VA_ARGS__)
// math.c
#define add(...)            call(add, __VA_ARGS__)
#define sub(...)            call(sub, __VA_ARGS__)
#define mul(...)            call(mul, __VA_ARGS__)
#define div(...)            call(div, __VA_ARGS__)
#define mod(...)            call(mod, __VA_ARGS__)
#define pow(...)            call(pow, __VA_ARGS__)
#define sqrt(...)           call(sqrt, __VA_ARGS__)
#define abs(...)            call(abs, __VA_ARGS__)
#define min(...)            call(min, __VA_ARGS__)
#define max(...)            call(max, __VA_ARGS__)
#define sin(...)            call(sin, __VA_ARGS__)
#define cos(...)            call(cos, __VA_ARGS__)
#define tan(...)            call(tan, __VA_ARGS__)
#define inc(...)            call(inc, __VA_ARGS__)
#define dec(...)            call(dec, __VA_ARGS__)
#define seed(...)           call(seed, __VA_ARGS__)
#define rand(...)           call(rand, __VA_ARGS__)
#define random(...)         call(random, __VA_ARGS__)
#define floor(...)          call(floor, __VA_ARGS__)
#define ceil(...)           call(ceil, __VA_ARGS__)
#define round(...)          call(round, __VA_ARGS__)
#define trunc(...)          call(trunc, __VA_ARGS__)
#define bit_and(...)        call(bit_and, __VA_ARGS__)
#define bit_or(...)         call(bit_or, __VA_ARGS__)
#define bit_xor(...)        call(bit_xor, __VA_ARGS__)
#define bit_not(...)        call(bit_not, __VA_ARGS__)
#define lshift(...)         call(lshift, __VA_ARGS__)
#define rshift(...)         call(rshift, __VA_ARGS__)
#define neg(...)            call(neg, __VA_ARGS__)
// mem.c
#define malloc(...)         call(malloc, __VA_ARGS__)
#define calloc(...)         call(calloc, __VA_ARGS__)
#define realloc(...)        call(realloc, __VA_ARGS__)
#define memset(...)         call(memset, __VA_ARGS__)
#define memcpy(...)         call(memcpy, __VA_ARGS__)
#define memcmp(...)         call(memcmp, __VA_ARGS__)
#define memmove(...)        call(memmove, __VA_ARGS__)
// io.c
#define print(...)          call(print, __VA_ARGS__)
#define print_string(...)   call(print_string, __VA_ARGS__)
#define print_int(...)      call(print_int, __VA_ARGS__)
#define print_float(...)    call(print_float, __VA_ARGS__)
#define print_bool(...)     call(print_bool, __VA_ARGS__)
#define println(...)        call(println, __VA_ARGS__)
#define ls(...)             call(ls, __VA_ARGS__)
#define scan(...)           call(scan, __VA_ARGS__)
// string.c
#define strdup(...)         call(strdup, __VA_ARGS__)
#define strcat(...)         call(strcat, __VA_ARGS__)
#define strcpy(...)         call(strcpy, __VA_ARGS__)
#define strncpy(...)        call(strncpy, __VA_ARGS__)
#define strcmp(...)         call(strcmp, __VA_ARGS__)
#define strlen(...)         call(strlen, __VA_ARGS__)
#define strchr(...)         call(strchr, __VA_ARGS__)
#define strstr(...)         call(strstr, __VA_ARGS__)
#define strsplit(...)       call(strsplit, __VA_ARGS__)
// condition.c
#define equals(...)         call(equals, __VA_ARGS__)
#define not_equals(...)     call(not_equals, __VA_ARGS__)
#define greater(...)        call(greater, __VA_ARGS__)
#define greater_equal(...)  call(greater_equal, __VA_ARGS__)
#define less(...)           call(less, __VA_ARGS__)
#define less_equal(...)     call(less_equal, __VA_ARGS__)
#define and(...)            call(and, __VA_ARGS__)
#define or(...)             call(or, __VA_ARGS__)
#define includes(...)       call(includes, __VA_ARGS__)
// types.c
#define to_int(...)         call(int, __VA_ARGS__)
#define to_float(...)       call(float, __VA_ARGS__)
#define to_string(...)      call(string, __VA_ARGS__)
// os.c
#define read_bin(...)       call(read_bin, __VA_ARGS__)
#define write_bin(...)      call(write_bin, __VA_ARGS__)
#define read_file(...)      call(read_file, __VA_ARGS__)
#define write_file(...)     call(write_file, __VA_ARGS__)
#define file_exists(...)    call(file_exists, __VA_ARGS__)
// time.c
#define time(...)           call(time, __VA_ARGS__)
#define clock(...)          call(clock, __VA_ARGS__)
#define ms(...)             call(ms, __VA_ARGS__)
// std.c
#define drop(...)           call(drop, __VA_ARGS__)
#define eval(...)           call(eval, __VA_ARGS__)


// static version of the std

// list.c
#define $list(...)           call($list, __VA_ARGS__)
#define $pop(...)            call($pop, __VA_ARGS__)
#define $push(...)           call($push, __VA_ARGS__)
#define $shift(...)          call($shift, __VA_ARGS__)
#define $unshift(...)        call($unshift, __VA_ARGS__)
#define $insert(...)         call($insert, __VA_ARGS__)
#define $remove(...)         call($remove, __VA_ARGS__)
#define $get(...)            call($get, __VA_ARGS__)
#define $set(...)            call($set, __VA_ARGS__)
#define $where(...)          call($where, __VA_ARGS__)
#define $find(...)           call($find, __VA_ARGS__)
#define $length(...)         call($length, __VA_ARGS__)
#define $copy(...)           call($copy, __VA_ARGS__)
#define $swap(...)           call($swap, __VA_ARGS__)
#define $reverse(...)        call($reverse, __VA_ARGS__)
#define $point(...)          call($point, __VA_ARGS__)
#define $alloc(...)          call($alloc, __VA_ARGS__)
#define $dup(...)            call($dup, __VA_ARGS__)
#define $free(...)           call($free, __VA_ARGS__)
#define $nameof(...)         call($nameof, __VA_ARGS__)
#define $typeof(...)         call($typeof, __VA_ARGS__)
#define $rename(...)         call($rename, __VA_ARGS__)
#define $retype(...)         call($retype, __VA_ARGS__)
// math.c
#define $add(...)            call($add, __VA_ARGS__)
#define $sub(...)            call($sub, __VA_ARGS__)
#define $mul(...)            call($mul, __VA_ARGS__)
#define $div(...)            call($div, __VA_ARGS__)
#define $mod(...)            call($mod, __VA_ARGS__)
#define $pow(...)            call($pow, __VA_ARGS__)
#define $sqrt(...)           call($sqrt, __VA_ARGS__)
#define $abs(...)            call($abs, __VA_ARGS__)
#define $min(...)            call($min, __VA_ARGS__)
#define $max(...)            call($max, __VA_ARGS__)
#define $sin(...)            call($sin, __VA_ARGS__)
#define $cos(...)            call($cos, __VA_ARGS__)
#define $tan(...)            call($tan, __VA_ARGS__)
#define $inc(...)            call($inc, __VA_ARGS__)
#define $dec(...)            call($dec, __VA_ARGS__)
#define $seed(...)           call($seed, __VA_ARGS__)
#define $rand(...)           call($rand, __VA_ARGS__)
#define $random(...)         call($random, __VA_ARGS__)
#define $floor(...)          call($floor, __VA_ARGS__)
#define $ceil(...)           call($ceil, __VA_ARGS__)
#define $round(...)          call($round, __VA_ARGS__)
#define $trunc(...)          call($trunc, __VA_ARGS__)
#define $bit_and(...)        call($bit_and, __VA_ARGS__)
#define $bit_or(...)         call($bit_or, __VA_ARGS__)
#define $bit_xor(...)        call($bit_xor, __VA_ARGS__)
#define $bit_not(...)        call($bit_not, __VA_ARGS__)
#define $lshift(...)         call($lshift, __VA_ARGS__)
#define $rshift(...)         call($rshift, __VA_ARGS__)
#define $neg(...)            call($neg, __VA_ARGS__)
// mem.c
#define $malloc(...)         call($malloc, __VA_ARGS__)
#define $calloc(...)         call($calloc, __VA_ARGS__)
#define $realloc(...)        call($realloc, __VA_ARGS__)
#define $memset(...)         call($memset, __VA_ARGS__)
#define $memcpy(...)         call($memcpy, __VA_ARGS__)
#define $memcmp(...)         call($memcmp, __VA_ARGS__)
#define $memmove(...)        call($memmove, __VA_ARGS__)
// io.c
#define $print(...)          call($print, __VA_ARGS__)
#define $print_string(...)   call($print_string, __VA_ARGS__)
#define $print_int(...)      call($print_int, __VA_ARGS__)
#define $print_float(...)    call($print_float, __VA_ARGS__)
#define $print_bool(...)     call($print_bool, __VA_ARGS__)
#define $println(...)        call($println, __VA_ARGS__)
#define $ls(...)             call($ls, __VA_ARGS__)
#define $scan(...)           call($scan, __VA_ARGS__)
// string.c
#define $strdup(...)         call($strdup, __VA_ARGS__)
#define $strcat(...)         call($strcat, __VA_ARGS__)
#define $strcpy(...)         call($strcpy, __VA_ARGS__)
#define $strncpy(...)        call($strncpy, __VA_ARGS__)
#define $strcmp(...)         call($strcmp, __VA_ARGS__)
#define $strlen(...)         call($strlen, __VA_ARGS__)
#define $strchr(...)         call($strchr, __VA_ARGS__)
#define $strstr(...)         call($strstr, __VA_ARGS__)
#define $strsplit(...)       call($strsplit, __VA_ARGS__)
// condition.c
#define $equals(...)         call($equals, __VA_ARGS__)
#define $not_equals(...)     call($not_equals, __VA_ARGS__)
#define $greater(...)        call($greater, __VA_ARGS__)
#define $greater_equal(...)  call($greater_equal, __VA_ARGS__)
#define $less(...)           call($less, __VA_ARGS__)
#define $less_equal(...)     call($less_equal, __VA_ARGS__)
#define $and(...)            call($and, __VA_ARGS__)
#define $or(...)             call($or, __VA_ARGS__)
#define $includes(...)       call($includes, __VA_ARGS__)
// types.c
#define $to_int(...)         call($int, __VA_ARGS__)
#define $to_float(...)       call($float, __VA_ARGS__)
#define $to_string(...)      call($string, __VA_ARGS__)
// os.c
#define $read_bin(...)       call($read_bin, __VA_ARGS__)
#define $write_bin(...)      call($write_bin, __VA_ARGS__)
#define $read_file(...)      call($read_file, __VA_ARGS__)
#define $write_file(...)     call($write_file, __VA_ARGS__)
#define $file_exists(...)    call($file_exists, __VA_ARGS__)
// time.c
#define $time(...)           call($time, __VA_ARGS__)
#define $clock(...)          call($clock, __VA_ARGS__)
#define $ms(...)             call($ms, __VA_ARGS__)
// std.c
#define $drop(...)           call($drop, __VA_ARGS__)
#define $eval(...)           call($eval, __VA_ARGS__)

// if implementation
#define if_(label_before, label_after, label_end, condition, ...) \
goto(label_after) \
:label_before \
__VA_ARGS__ \
goto(label_end) \
:label_after \
label_before condition ? \
:label_end
#define if(condition, ...) if_(concat(concat(if, __COUNTER__), before), concat(concat(if, __COUNTER__), after), concat(concat(if, __COUNTER__), end), condition, __VA_ARGS__)

// while loop implementation
#define while_(label_before, label_after, condition, ...) \
goto(label_after) \
:label_before \
__VA_ARGS__ \
:label_after \
label_before condition ? 
#define while(condition, ...) while_(concat(while, __COUNTER__), concat(while, __COUNTER__), condition, __VA_ARGS__)

// for loop implementation
#define for_(label_before, label_after, init, condition, step, ...) \
init \
goto(label_after) \
:label_before \
__VA_ARGS__ \
step \
:label_after \
label_before condition ?
#define for(init, condition, step, ...) for_(concat(for, __COUNTER__), concat(for, __COUNTER__), init, condition, step, __VA_ARGS__)

// goto implementation
#define goto(label) label 1 ?

#define define(...) \
set(context, __VA_ARGS__)

#endif // BRUTER_LAZY_H