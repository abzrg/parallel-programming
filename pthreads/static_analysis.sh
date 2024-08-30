#!/bin/sh -e

clang -fsyntax-only -Xclang -analyze "$1"
