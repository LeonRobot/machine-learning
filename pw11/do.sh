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
# (C) Idiap Research Institute                                          #
# Written by Francois Fleuret                                           #
# Contact <francois.fleuret@idiap.ch> for comments & bug reports        #
#########################################################################


set -e
set -o pipefail

make -j -k

# PDF_VIEWER=xdg-open
# IMAGE_VIEWER=xdg-open
PDF_VIEWER=open
IMAGE_VIEWER=open

[[ ${MAIN_EXECUTABLE} ]] || MAIN_EXECUTABLE=./main

function compute () {
    if [[ ${MAIN_EXECUTABLE} -nt "$2" ]]; then
        echo "Computing."
        ${MAIN_EXECUTABLE} "$1"
    fi

    if [[ ${MAIN_EXECUTABLE} -nt "$2" ]]; then
        echo "Something did not work, \"$2\" was not updated." >&2
        return 1
    else
        return 0
    fi
}

if [[ ! "$1" ]]; then
    echo "$0 <example|question1|question2|question3|question4>" >&2
    exit 1
fi

while [[ "$1" ]]; do

    case "$1" in

        ######################################################################

        example)

            if compute example "example.jpg"; then

                ${IMAGE_VIEWER} "example.jpg"

            fi

            ;;

        ######################################################################

        question1)

            ${MAIN_EXECUTABLE} question1
			${IMAGE_VIEWER} "question1.jpg"

            ;;

        ######################################################################

        question2)

            if compute question2 "question2.dat"; then

                gnuplot <<EOF
set terminal pdfcairo
set output "question2.pdf"
set key top left
set grid
set grid xtics ytics lt 1 lw 1 lc rgb "#e0e0e0"

plot [-3.1415926:3.1415926][-1.25:1.25] \
     "question2.dat" w l lw 10 lc rgb "#8080ff" title "Prediction",\
     sin(x) w l lw 2 lc rgb "#000000" title "sin(x)
EOF

                ${PDF_VIEWER} "question2.pdf"

            fi

            ;;

        ######################################################################

        question3)

            if compute question3 "question3.jpg"; then

                ${IMAGE_VIEWER} "question3.jpg"

            fi

            ;;

        ######################################################################

        *)
            echo "Unknown argument $1" >&2
            exit 1
            ;;
    esac

    shift

done
