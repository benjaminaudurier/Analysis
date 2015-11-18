#! /bin/bash
##
##Script to Compare Efficiency for two EffComparison.root 


#Select file one
echo 
echo "What trigger for file 1 (MSL/MUL)?"
read trigg1
case  "$trigg1" in
	MSL)
		echo "trigger=$trigg1" 
		;; 
	MUL)
		echo "trigger=$trigg1"
		;; 
	*) 
		echo "trigger not defined"
		exit 1
		;;
esac

echo "What ptMin for file 1 (0/2/4)?"
read ptMin1
case  "$ptMin1" in
	0)
		echo "ptMin for file 1=$ptMin1" 
		;; 
	2)
		echo "ptMin for file 1=$ptMin1"
		;; 
	4)
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
echo "What trigger for file 2 (MSL/MUL)?"
read trigg2
case  "$trigg2" in
	MSL)
		echo "trigger=$trigg2" 
		;; 
	MUL)
		echo "trigger=$trigg2"
		;; 
	*) 
		echo "trigger not defined"
		exit 1
		;;
esac

echo "What ptMin for file 2 (0/2/4)?"
read ptMin2
case  "$ptMin2" in
	0)
		echo "ptMin for file 2=$ptMin2" 
		;; 
	2)
		echo "ptMin for file 2=$ptMin2"
		;; 
	4)
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
echo 
echo "Compute Comparison"
cd /Users/audurier/Documents/Analysis/LHC_15g_pp/TrackingEfficiency #move to directory
root -b -l -q 'ComparisonEfficiency.C++("'$file1'","'$file2'")'
cp ComparisonEfficiency.root Resultats/ComparisonEfficiency${trigg1}-${ptMin1}-Over${trigg2}-${ptMin2}.root


# #Compute Comparisonception
# echo 
# echo "Some Comparisonception maybe (y/n)?"
# read Comparisonception
# if [ "%Comparisonception" = "y" ]
# 	#Select file one
# 	echo 
# 	echo "What trigger 1 (MSL/MUL)?"
# 	read trigg3
# 	case  "$trigg3" in
# 		MSL)
# 			echo "trigger=$trigg3" 
# 			;; 
# 		nMUL)
# 			echo "trigger=$trigg3"
# 			;; 
# 		*) 
# 			echo "trigger not defined"
# 			exit 1
# 			;;
# 	esac

# 	echo "What ptMin 1 (0/2/4)?"
# 	read ptMin3
# 	case  "$ptMin3" in
# 		0)
# 			echo "ptMin for file 1=$ptMin3" 
# 			;; 
# 		2)
# 			echo "ptMin for file 1=$ptMin3"
# 			;; 
# 		4)
# 			echo "ptMin for file 1=$ptMin3"
# 			;; 
# 		*) 
# 			echo "ptMin for file 1 not defined"
# 			exit 1
# 			;;
# 	esac


# 	#Select part
# 	echo 
# 	echo "What trigger for file 2 (MSL/MUL)?"
# 	read trigg2
# 	case  "$trigg2" in
# 		MSL)
# 			echo "trigger=$trigg2" 
# 			;; 
# 		nMUL)
# 			echo "trigger=$trigg2"
# 			;; 
# 		*) 
# 			echo "trigger not defined"
# 			exit 1
# 			;;
# 	esac

# 	echo "What ptMin for file 2 (0/2/4)?"
# 	read ptMin2
# 	case  "$ptMin2" in
# 		0)
# 			echo "ptMin for file 2=$ptMin2" 
# 			;; 
# 		2)
# 			echo "ptMin for file 2=$ptMin2"
# 			;; 
# 		4)
# 			echo "ptMin for file 2=$ptMin2"
# 			;; 
# 		*) 
# 			echo "ptMin for file 2 not defined"
# 			exit 1
# 			;;
# 	esac

# 	echo
# 	echo "According to your choice, file 2 = EffComparison'${trigg2}'Pt='${ptMin2}'.root"
# 	echo
# 	export file2="Resultats/ComparisonEfficiency${trigg1}-${ptMin1}-Over${trigg2}-${ptMin2}.root"
# 	echo ${file2}

# 	root -b -l -q 'ComparisonEfficiency.C++("'$file1'","'$file2'")'
# 	cp ComparisonEfficiency.root Resultats/ComparisonEfficiency${trigg1}-${ptMin1}-Over${trigg2}-${ptMin2}.root

# fi






exit 0