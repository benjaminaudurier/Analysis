###########################################

###########################################
HOW MY CODE WORKS
###########################################

###########################################




-Open a terminal

-Go to the folder with command : cd somepath/My_First_Task/

-Run Fit.sh : bash Fit.sh




###########################################
Fit.sh 
###########################################

Fit.sh opens ROOT and calls “MyMacro.C”. Severals options can be added to the command :
	—> -q to quite after running MyMacro.C
	-> -b to run in bash mode (no ROOT window open)

Results are in MyMacroResult.txt




###########################################
MyMacro.C
###########################################

This program :
	-run “runProof.C” (can be commented after the first use)
	-Add fit functions written in FittingFunctions.C+ to library
	-Add the fit macro ( *Something*Macro.C) to the library
	-Calls the fitting macro




###########################################
runProof.C
###########################################

Program that
	- Load and set Aliroot (see macro inside)
	- Create an analysis train with all your tasks(a task :AliAnalysisTask*somename*.cxx)

Must be run manually once first. To to so :
	-> Open terminal
	-> Open root : “root”
	-> “.x runProof.C+” 

It is recommended to compile runLocal.C to make some debugging first.



###########################################
FittingFunctions.C
###########################################

Here is defined all functions needed in order to fit datas




###########################################
*somename*FitMacro.C
###########################################

Program that perform the fit. Look inside for details.



###########################################
AliAnalysisTask*somename*.cxx
###########################################

A c++ program which analysis your data set. Look inside for more details.


