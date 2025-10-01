#!/usr/bin/env bash
set -euo pipefail

if [[ $# -lt 1 ]]; then
    echo "Usage: $(basename "$0") <file1.(c|br)> [file2.(c|br) ...]" >&2
    exit 1
fi

cc_blocks=()
functions=()
embedded_lines=()

for input_file in "$@"; do
    case "$input_file" in
        *.c)
            if [[ -f "$input_file" ]]; then
                preprocessed="$(cat "$input_file")"

                # Adiciona ao bloco de código
                cc_blocks+=("$preprocessed")

                # Varre para achar funções
                while IFS= read -r line; do
                    if [[ "$line" =~ ^[[:space:]]*void[[:space:]]+([a-zA-Z_][a-zA-Z0-9_]*)[[:space:]]*\([[:space:]]*BruterList ]]; then
                        func_name="${BASH_REMATCH[1]}"
                        clean_name="$func_name"
                        [[ "$clean_name" == feraw_* ]] && clean_name="${clean_name:6}"
                        functions+=("$func_name:$clean_name")
                    fi
                done <<< "$preprocessed"
            else
                echo "Warning: C file not found: $input_file" >&2
            fi
            ;;
        *.br)
            if [[ -f "$input_file" ]]; then
                preprocessed="$(cat "$input_file")"

                while IFS= read -r line; do
                    if [[ "$line" =~ ^[[:space:]]*void[[:space:]]+([a-zA-Z_][a-zA-Z0-9_]*)[[:space:]]*\([[:space:]]*BruterList ]]; then
                        func_name="${BASH_REMATCH[1]}"
                        clean_name="$func_name"
                        [[ "$clean_name" == feraw_* ]] && clean_name="${clean_name:6}"
                        functions+=("$func_name:$clean_name")
                    fi
                    embedded_lines+=("$line")
                done <<< "$preprocessed"
            else
                echo "Warning: BR file not found: $input_file" >&2
            fi
            ;;
        *)
            echo "Warning: unsupported file type: $input_file" >&2
            ;;
    esac
done

# Gera a saída no stdout
{
    echo '#include "bruter.h"'
    echo '#include <stdlib.h>'
    echo

    if (( ${#cc_blocks[@]} > 0 )); then
        echo "/* BEGIN injected C blocks (expanded) */"
        printf '%s\n' "${cc_blocks[@]}"
        echo "/* END injected C blocks */"
        echo
    fi

    echo "int main(int argc, char *argv[])"
    echo "{"
    echo "    BruterList *context = bruter_new(BRUTER_DEFAULT_SIZE, true, true);"
    echo
    echo "    BruterList *args = bruter_new(BRUTER_DEFAULT_SIZE, false, true);"
    echo "    for(int i = 0; i < argc; i++) { bruter_push_pointer(args, argv[i], NULL, BRUTER_TYPE_BUFFER); }"
    echo "    bruter_push_pointer(context, args, \"args\", BRUTER_TYPE_LIST);"
    echo

    if (( ${#functions[@]} > 0 )); then
        echo "    /* BEGIN function registrations */"
        for fn in "${functions[@]}"; do
            func="${fn%%:*}"
            clean="${fn##*:}"
            echo "    bruter_push_pointer(context, $func, \"$clean\", BRUTER_TYPE_FUNCTION);"
        done
        echo "    /* END function registrations */"
        echo
    fi

    if (( ${#embedded_lines[@]} > 0 )); then
        echo "    const char *embedded_code ="
        for line in "${embedded_lines[@]}"; do
            [[ -z "$line" ]] && continue
            esc="${line//\\/\\\\}"
            esc="${esc//\"/\\\"}"
            echo "    \"$esc\\n\""
        done
        echo "    ;"
        echo
        echo "    bruter_interpret(context, embedded_code, NULL, NULL);"
    fi

    echo "    bruter_free(args);"
    echo "    bruter_free(context);"
    echo "    return EXIT_SUCCESS;"
    echo "}"
}
