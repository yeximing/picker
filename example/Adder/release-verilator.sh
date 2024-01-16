#!/bin/bash


if ! command -v verible-verilog-syntax &> /dev/null
then
    echo "verible could not be found"
    echo "please add verible-verilog-syntax into path first"
    echo "https://chipsalliance.github.io/verible/verilog_syntax.html"
    echo "https://github.com/chipsalliance/verible/releases/tag/v0.0-3428-gcfcbb82b"
    exit
fi

rm -rf picker_out_adder/
picker example/Adder/Adder.v -w Adder.fst -S Adder $@ -t picker_out_adder
cp example/Adder/example.cpp picker_out_adder/cpp/

cd picker_out_adder && make
