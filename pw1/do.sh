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

if [[ $(which xpdf) ]]; then
    PDF_VIEWER=xpdf
else
    PDF_VIEWER=evince
fi

[[ ${MAIN_EXECUTABLE} ]] || MAIN_EXECUTABLE=./pw1

if [[ "$1" == "-v" ]]; then
    VIEW=yes
    shift
fi

if [[ ! "$1" ]]; then
    echo "$0 [-v] <example|question1|question2|question3|question4>" >&2
    exit 1
fi

case $1 in

######################################################################

    example)

        if [[ ${MAIN_EXECUTABLE} -nt /tmp/true.dat ]]; then
            echo -n "Computing ... "
            ${MAIN_EXECUTABLE} example
            echo "done."
        fi

        gnuplot <<EOF
set terminal pdfcairo
set output "/tmp/example.pdf"
set key top left
set grid
set grid xtics ytics lt 1 lw 1 lc rgb "#e0e0e0"

plot [:][-1.25:1.25] \
     "/tmp/example_train.dat" w p pt 6 lc rgb "#8080ff" notitle,\
     sin(x) w l lw 2 lc rgb "#8080ff",\
     "/tmp/example.dat" using 1:3 w l lw 4 lc rgb "#ff0000" title "Prediction"
EOF

        if [[ ${VIEW} ]]; then
            ${PDF_VIEWER} /tmp/example.pdf
        else
            echo "Graph saved in /tmp/example.pdf"
        fi

        ;;

######################################################################

    question1)

        if [[ ${MAIN_EXECUTABLE} -nt /tmp/question1.dat ]]; then
            echo -n "Computing ... "
            ${MAIN_EXECUTABLE} question1
            echo "done."
        fi

        d_values=$(awk '{print $1}' < /tmp/question1.dat | uniq)

        for d in ${d_values}; do
            if [[ -n "${plot_command}" ]]; then plot_command+=","; fi
            plot_command+="'< grep ^\"$d \" /tmp/question1.dat' using 2:3 with line lw 3 title \"$d\""
        done

        if [[ -f "/tmp/question1_train.dat" ]]; then
            if [[ -n "${plot_command}" ]]; then plot_command+=","; fi
            plot_command+="'/tmp/question1_train.dat' w p pt 6 lc rgb \"#8080ff\" notitle'"
        fi

        gnuplot <<EOF
set terminal pdfcairo
set output "/tmp/question1.pdf"
set key top left
set grid
set grid xtics ytics lt 1 lw 1 lc rgb "#e0e0e0"
set xlabel "Degree"

plot [0:1][-0.2:1.2] ${plot_command}
EOF

        if [[ "${VIEW}" ]]; then
            ${PDF_VIEWER} /tmp/question1.pdf
        else
            echo "Graph saved in /tmp/question1.pdf"
        fi

        ;;

######################################################################

    question2)

        if [[ ${MAIN_EXECUTABLE} -nt /tmp/question2.dat ]]; then
            echo -n "Computing ... "
            ${MAIN_EXECUTABLE} question2
            echo "done."
        fi

        n_values=$(awk '{print $1}' < /tmp/question2.dat | uniq)

        for n in ${n_values}; do
            if [[ -n "${plot_command}" ]]; then plot_command+=","; fi
            plot_command+="'< grep ^\"$n \" /tmp/question2.dat' using 2:3 with line lw 3 title \"$n samples\""
        done

        gnuplot <<EOF
set terminal pdfcairo
set output "/tmp/question2.pdf"
set key top left
set grid
set grid xtics ytics lt 1 lw 1 lc rgb "#e0e0e0"
set xlabel "Degree"
set ylabel "Error (log)"
set logscale y

plot ${plot_command}
EOF

        if [[ "${VIEW}" ]]; then
            ${PDF_VIEWER} /tmp/question2.pdf
        else
            echo "Graph saved in /tmp/question2.pdf"
        fi

        ;;

######################################################################

    question3|question4)

        if [[ ${MAIN_EXECUTABLE} -nt /tmp/$1.dat ]]; then
            echo -n "Computing ... "
            ${MAIN_EXECUTABLE} $1
            echo "done."
        fi

        plot_command="'/tmp/$1.dat' w l lw 3 notitle"

        if [[ -f "/tmp/$1_train.dat" ]]; then
            if [[ -n "${plot_command}" ]]; then plot_command+=","; fi
            plot_command+="'/tmp/$1_train.dat' w p pt 6 lc rgb \"#8080ff\" notitle'"
        fi


        gnuplot <<EOF
set terminal pdfcairo
set output "/tmp/$1.pdf"
set key top left
set grid
set grid xtics ytics lt 1 lw 1 lc rgb "#e0e0e0"
set xlabel "Degree"
set ylabel "Error (log)"

plot ${plot_command}
EOF

        if [[ "${VIEW}" ]]; then
            ${PDF_VIEWER} /tmp/$1.pdf
        else
            echo "Graph saved in /tmp/$1.pdf"
        fi

        ;;

######################################################################

    *)
        echo "Unknown argument $1" >&2
        exit 1
        ;;
esac
