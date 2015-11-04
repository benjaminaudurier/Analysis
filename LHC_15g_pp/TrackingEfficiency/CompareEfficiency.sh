#! /bin/bash
##
##Script to Compare Efficiency for two EffComparison.root 

local trigg1 trigg2 ptcutyn ptMin1 ptMin2 file1 file2

#Select file one
echo 
echo "What trigger for file 1 ?"
read trigg1
case  "$trigg1" in
	MSL)
		echo "trigger=$trigg1" 
		;; 
	nMUL)
		echo "trigger=$trigg1"
		;; 
	*) 
		echo "trigger not defined"
		exit 1
		;;
esac

echo "What ptMin for file 1 ?"
read ptMin1
case  "$ptMin1" in
	0)
		echo "ptMin for file 1=$ptMin1" 
		;; 
	1)
		echo "ptMin for file 1=$ptMin1"
		;; 
	2)
		echo "ptMin for file 1=$ptMin1"
		;; 
	*) 
		echo "ptMin for file 1 not defined"
		exit 1
		;;
esac

echo
echo "According to your choice, file 1 = EffComparison${trigg1}Pt=${ptMin1}.root"
echo
export file1='Resultats/EffComparison'${trigg1}'Pt='${ptMin1}'.root'
echo ${file1}

#Select file two
echo 
echo "What trigger for file 2 ?"
read trigg2
case  "$trigg2" in
	MSL)
		echo "trigger=$trigg2" 
		;; 
	nMUL)
		echo "trigger=$trigg2"
		;; 
	*) 
		echo "trigger not defined"
		exit 1
		;;
esac

echo "What ptMin for file 2 ?"
read ptMin2
case  "$ptMin2" in
	0)
		echo "ptMin for file 2=$ptMin2" 
		;; 
	1)
		echo "ptMin for file 2=$ptMin2"
		;; 
	2)
		echo "ptMin for file 2=$ptMin2"
		;; 
	*) 
		echo "ptMin for file 2 not defined"
		exit 1
		;;
esac

echo
echo "According to your choice, file 2 = EffComparison'${trigg2}'Pt='${ptMin2}'.root"
echo
export file2="Resultats/EffComparison${trigg2}Pt=${ptMin2}.root"
echo ${file2}

#ComputeComparison

#Eff comparison
echo 
echo "ComputeComparison"

root -b -l -q 'ComparisonEfficiency.C++("'$file1'","'$file2'")'
cp ComparisonEfficiency.root Resultats/ComparisonEfficiency${trigg1}-${ptMin1}-Over${trigg2}-${ptMin2}.root


#Copyfile in results dir.


exit 0