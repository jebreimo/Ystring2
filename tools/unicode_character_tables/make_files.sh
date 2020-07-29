#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"

if [ ! -e "$1" ]; then
  echo "Usage: $0 <unicode database>"
  exit 1
fi

echo Making UpperCaseTables.hpp
python "$SCRIPT_DIR/make_case_convert_file.py" upper $1 >UpperCaseTables.hpp
echo Making LowerCaseTables.hpp
python "$SCRIPT_DIR/make_case_convert_file.py" lower $1 >LowerCaseTables.hpp
echo Making TitleCaseTables.hpp
python "$SCRIPT_DIR/make_case_convert_file.py" title $1 >TitleCaseTables.hpp
echo Making CharClassTables.cpp
python "$SCRIPT_DIR/make_char_class_file.py" $1 >CharClassTables.hpp
