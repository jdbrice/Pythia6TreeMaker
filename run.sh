#!/bin/bash

source /home/jdb12/work/pythia/set_env.sh
which root
root -b -q -l 'runPythia.C( "/data/star1/jdb12/dilepton/Run15_PP_200_Mtd/Pythia/'r2_out_$1.root'", 10000, '$1', 1201 )'