The folder is dedicated to the study of the syst. uncertainty ot the mean pt. We generate maps with stat. flucutation and run the mumu framework to see how the mean pt change
.The workflow is :

- Generate the maps with CreateAccEffMap.C
- Copy them on the saf with runMuMu.C, AddTaskMuMu.C, runJPsiAccEffCorr2.sh
- go to saf 3 and launch the script.
- Copy back the results and make the fit.
- Print the results