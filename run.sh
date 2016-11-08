#!/bin/bash

/data/star1/jdb12/vendor/pythia_root_6.06.02/root-6.06.02/bin/root -b -q -l 'runPythia.C+( "/data/star1/jdb12/dilepton/Run15_PP_200_Mtd/Pythia/'out_$1.root'", 10000, '$1' )'