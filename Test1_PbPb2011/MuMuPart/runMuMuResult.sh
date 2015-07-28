#!/bin/sh

#  Bash script to set and run MuMu framework automatically
#  
#PMy
#  Created by Benjamin Audurier on 03/12/14.
#

echo 
echo "Should i read runMuMu.C (y or n)?"
read response
echo "response=$response"
case  $response in 
	y)
		root -b 'runMuMu.C("datasetfull.txt")' > runMuMuOutputs.txt
		root -b -q '.x MyMacro.C("datasetfull.txt.saf.root","","","PbPb2011")' > runMuMuResultsOutputs.txt
		;; 
	n)
		echo "Running MyMacro.C only..."
		root -b  'MyMacro.C("datasetfull.txt.saf.root","","","PbPb2011",0)' > runMuMuResultsOutputs.txt
		;; 

	*) 
		echo "Error reading (y/n)"
		exit 1
		;;
esac
exit 0




