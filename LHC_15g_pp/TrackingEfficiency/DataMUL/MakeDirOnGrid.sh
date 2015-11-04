
#include "Riostream.h"
#include "TUrl.h"
#include "TSystem.h"
#include "TString.h"
#include "TFile.h"
#include "TGrid.h"
void CopyFromRemote(const char* txtfile="grid.esd.txt")
{
}

for line in $(cat runList.txt)
	do 
		root -b -l -q 'gSystem->Exec("alien_mkdir /alice/cern.ch/user/b/baudurie/Analysis/LHC15g/TrackingEfficiency/ResultatPhilippe/Data/results/000$line")'  
	done

exit 0