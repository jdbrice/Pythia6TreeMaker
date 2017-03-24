#!/bin/bash

source /home/jdb12/work/pythia/set_env.sh
let seed=$1+0
echo "seed = $seed"
./GENERATOR 1 /data/star1/jdb12/dilepton/pythia/muon_filter/seed_ 1000000 $seed 13
# ./GENERATOR 1 1000000 $seed
# root -b -q -l 'runPythia.C( "/data/star1/jdb12/dilepton/Run15_PP_200_Mtd/ccbar/'pythia_ccbar_mumu_$seed.root'", 10000, '$seed', 2500 )'