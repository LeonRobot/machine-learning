#!/bin/bash

#########################################################################
# This program is free software: you can redistribute it and/or modify  #
# it under the terms of the version 3 of the GNU General Public License #
# as published by the Free Software Foundation.                         #
#                                                                       #
# This program is distributed in the hope that it will be useful, but   #
# WITHOUT ANY WARRANTY; without even the implied warranty of            #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU      #
# General Public License for more details.                              #
#                                                                       #
# You should have received a copy of the GNU General Public License     #
# along with this program. If not, see <http://www.gnu.org/licenses/>.  #
#                                                                       #
# Written by and Copyright (C) Francois Fleuret                         #
# Contact <francois.fleuret@idiap.ch> for comments & bug reports        #
#########################################################################

set -e

function check_mnist () {
    for f in train-images-idx3-ubyte train-labels-idx1-ubyte t10k-images-idx3-ubyte t10k-labels-idx1-ubyte; do
        if [[ -f "./$f" ]]; then
            echo "Found $f, good."
        else
            echo "File $f is missing."
            exit 1
        fi
    done
}

function draw_graph () {
    gnuplot <<EOF
# set terminal pdfcairo
set terminal postscript
set output "/tmp/example.eps"
set xlabel "Maximum depth"
set ylabel "Error rate"
plot 'train_error.dat' w l title "Training error",\
     'test_error.dat' w l title "Test error"
EOF

    if [[ ${VIEW} ]]; then
        ${PDF_VIEWER} /tmp/example.eps
    else
        echo "Graph saved in /tmp/example.eps"
    fi
}

if [[ $(which xpdf) ]]; then
    PDF_VIEWER=xpdf
else
    # PDF_VIEWER=evince
	PDF_VIEWER=open
fi

[[ ${MAIN_EXECUTABLE} ]] || MAIN_EXECUTABLE=./pw1

if [[ ! "$1" ]]; then
    echo "$0 [-v] <demo|question1|question2|question3|question4>" >&2
    exit 1
fi

while [[ $1 ]]; do

    if [[ "$1" == "-v" ]]; then

        VIEW=yes

    elif [[ $1 == "graph" ]]; then

        draw_graph

    else

        make -j -k

        check_mnist

        ./main \
            --train-data-files train-images-idx3-ubyte train-labels-idx1-ubyte \
            --test-data-files t10k-images-idx3-ubyte t10k-labels-idx1-ubyte \
            $1 \
            | tee log

    fi

    shift

done
