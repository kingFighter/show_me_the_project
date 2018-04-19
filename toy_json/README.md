# TOY JSON

## Features
* parser
* generator
* C++
* support utf-8
* unit test
* docs

## Dependencies
* doxygen
* gtest
* premake5

## Build
1. cd toy_json/src
2. premake5 --file=toy_json.lua gmake
3. cd ../build
4. make

## Unit Test
1. After [Build](#build)
2. ./unit_test

## Doc
1. cd  toy_json
2. doxygen Doxyfile
3. check with toy_json/docs

## Reference
1. [RFC 7159](https://tools.ietf.org/html/rfc7159)
2. [ECMA-404](https://www.ecma-international.org/publications/files/ECMA-ST/ECMA-404.pdf)
