#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"

if [ ! -e "$1" ]; then
  echo "Usage: $0 <unicode database>"
  exit 1
fi

echo Making StandardUpperCase.hpp
python "$SCRIPT_DIR/make_case_convert_file.py" upper $1 >StandardUpperCase.hpp
echo Making StandardLowerCase.hpp
python "$SCRIPT_DIR/make_case_convert_file.py" lower $1 >StandardLowerCase.hpp
echo Making StandardTitleCase.hpp
python "$SCRIPT_DIR/make_case_convert_file.py" title $1 >StandardTitleCase.hpp
echo Making CharClass.cpp
python "$SCRIPT_DIR/make_char_class_file.py" $1 >CharClass.cpp
