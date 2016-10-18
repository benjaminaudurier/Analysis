
// Macro to merge centrality bins

#include <AliLog.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <AliAnalysisMuMu.h>
#include <AliAnalysisMuMuConfig.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TFile.h>

#include <AliMergeableCollection.h>

// TString sfile ="/Users/audurier/Documents/Analysis/LHC_15o_PbPb/MuMu/Data_Full_pool=50/AnalysisResults.root";
TString sfile ="/Users/audurier/Documents/Analysis/LHC_15o_PbPb/MuMu/Data_Full_pool=50/AnalysisResultsMergeAtSafLevel.root";
TString sasso ="";
TString sasso2 ="";
TString config = "/Users/audurier/Documents/Analysis/LHC_15o_PbPb/MuMu/Data_Full_pool=50/mumu.PbPb2015.full.config";

const char* EVENTSELECTIONMIX    =  "PSMSL";
const char* MIXTRIGGER           =  "CMSL7-B-NOPF-MUFAST";
const char* PairSelection        =  "pALLPAIRYPAIRPTIN0.0-12.0RABSMATCHLOWETA/";
const char* particle             =  "psi";

// For the loop comming
TString signFlagMinv[3] ={"","PlusPlus","MinusMinus"};
TString signFlagDist[3] ={"","PP","MM"};

Bool_t corrected = kFALSE;

const char* binToMerge      =  /* /V0M_00.00_10.00,V0M_10.00_20.00",V0M_20.00_30.00,V0M_30.00_40.00,*/"/V0M_40.00_50.00,V0M_50.00_60.00,V0M_60.00_70.00,V0M_70.00_80.00,V0M_80.00_90.00";
TString mergeCentralityName =  "V0M_40.00_90.00";

//_____________________________________________________________________________
void MergeCentralityBins(int debug =0)
{
    AliLog::SetGlobalDebugLevel(debug);

    // Main object
    AliAnalysisMuMu analysis(sfile.Data(),sasso.Data(),sasso2.Data(),config.Data());
    if(!analysis.OC() || !analysis.Config() || !analysis.BIN() ) return;
    analysis.CleanAllSpectra();

    // Get configuration settings
    TObjArray* eventTypeArray       = analysis.Config()->GetListElements(analysis.Config()->EventSelectionKey(),analysis.IsSimulation());
    TObjArray* triggerArray         = analysis.Config()->GetTriggersList(analysis.IsSimulation());
    TObjArray* eventTypeArrayMix    = analysis.Config()->GetListElements(analysis.Config()->RefMixEventSelectionKey(),analysis.IsSimulation());
    TObjArray* triggerArrayMix      = analysis.Config()->GetListElements(analysis.Config()->RefMixTriggerKey(),analysis.IsSimulation());

    TIter nexteventTypeArray(eventTypeArray);
    TIter nexttriggerArray(triggerArray);
    TIter nexteventTypeArrayMix(eventTypeArrayMix);
    TIter nexttriggerArrayMix(triggerArrayMix);

    TObjString* seventTypeArray;
    TObjString* striggerArray;
    TObjString* seventTypeArrayMix;
    TObjString* striggerArrayMix;

    TList* list(0x0);

    TObject* p =0x0;

    // Merge centrality bin for all 'classical' histo
    while ( ( seventTypeArray = static_cast<TObjString*>(nexteventTypeArray())) )
    {
        // Loop on each trigger (see MuMuConfig)
        nexttriggerArray.Reset();
        while ( ( striggerArray = static_cast<TObjString*>(nexttriggerArray())) )
        {
            // Identifier to give to AliMergeableCollection::GetSum()
            TString identifier = Form("/%s/%s",seventTypeArray->String().Data(),striggerArray->String().Data());
            identifier += binToMerge;
            identifier += PairSelection;

            TList* list = analysis.OC()->CreateListOfObjectNames(Form("/%s/%s/V0M_00.00_10.00/%s",seventTypeArray->String().Data(),striggerArray->String().Data(),PairSelection));
            if(list->IsEmpty()) continue;

            // printf("\n ---- \n ---- \n preparing storage \n ---- \n ---- \n");
            // Store Minv histo
            for (int j = 0; j < list->GetEntries(); ++j) {

                const char* objName =list->At(j)->GetName();

                TObject * o = analysis.OC()->GetSum(Form("%s/%s",identifier.Data(),objName));
                if ( o ) {
                   p = analysis.OC()->GetObject(Form("/%s/%s/%s/%s",seventTypeArray->String().Data(),striggerArray->String().Data(),mergeCentralityName.Data(),PairSelection),o->GetName());

                    if (p) analysis.OC()->Remove(Form("/%s/%s/%s/%s/%s",seventTypeArray->String().Data(),striggerArray->String().Data(),mergeCentralityName.Data(),PairSelection,o->GetName()));

                    Bool_t adoptOK = analysis.OC()->Adopt(Form("/%s/%s/%s/%s",seventTypeArray->String().Data(),striggerArray->String().Data(),mergeCentralityName.Data(),PairSelection),o->Clone());

                    if ( adoptOK ) printf(" --- Merge done for %s/%s/%s/%s/%s \n",seventTypeArray->String().Data(),striggerArray->String().Data(),mergeCentralityName.Data(),PairSelection,o->GetName());
                    else printf(" --- A problem occurs for %s/%s/%s/%s/%s \n",seventTypeArray->String().Data(),striggerArray->String().Data(),mergeCentralityName.Data(),PairSelection,o->GetName());
                } else continue;
            }

            delete list;
        }
    }

  //   // Merge centrality bin for all Mix
    while ( ( seventTypeArrayMix = static_cast<TObjString*>(nexteventTypeArrayMix())) )
    {
        // Loop on each trigger (see MuMuConfig)
        nexttriggerArrayMix.Reset();
        while ( ( striggerArrayMix = static_cast<TObjString*>(nexttriggerArrayMix())) )
        {
            // Identifier to give to AliMergeableCollection::GetSum()
            TString identifier = Form("/MIX/%s_%s/%s/%s",seventTypeArrayMix->String().Data(),striggerArrayMix->String().Data(),EVENTSELECTIONMIX,MIXTRIGGER);
            identifier += binToMerge;
            identifier += PairSelection;

            TList* list = analysis.OC()->CreateListOfObjectNames(Form("/MIX/%s_%s/%s/%s/V0M_00.00_10.00/%s",seventTypeArrayMix->String().Data(),striggerArrayMix->String().Data(),EVENTSELECTIONMIX,MIXTRIGGER,PairSelection));
            if(list->IsEmpty()) continue;

            // Store Minv histo
            for (int j = 0; j < list->GetEntries(); ++j) {

                const char* objName =list->At(j)->GetName();

                TObject * o = analysis.OC()->GetSum(Form("%s/%s",identifier.Data(),objName));
                if ( o ) {
                   p = analysis.OC()->GetObject(Form("/MIX/%s_%s/%s/%s/%s/%s",seventTypeArrayMix->String().Data(),striggerArrayMix->String().Data(),EVENTSELECTIONMIX,MIXTRIGGER,mergeCentralityName.Data(),PairSelection),o->GetName());

                    if (p) analysis.OC()->Remove(Form("/MIX/%s_%s/%s/%s/%s/%s/%s",seventTypeArrayMix->String().Data(),striggerArrayMix->String().Data(),EVENTSELECTIONMIX,MIXTRIGGER,mergeCentralityName.Data(),PairSelection,o->GetName()));

                    Bool_t adoptOK = analysis.OC()->Adopt(Form("/MIX/%s_%s/%s/%s/%s/%s",seventTypeArrayMix->String().Data(),striggerArrayMix->String().Data(),EVENTSELECTIONMIX,MIXTRIGGER,mergeCentralityName.Data(),PairSelection),o->Clone());

                    if ( adoptOK ) printf(" --- Merge done for /MIX/%s_%s/%s/%s/%s/%s/%s\n",seventTypeArrayMix->String().Data(),striggerArrayMix->String().Data(),EVENTSELECTIONMIX,MIXTRIGGER,mergeCentralityName.Data(),PairSelection,o->GetName());
                    else printf(" --- A problem occurs for /MIX/%s_%s/%s/%s/%s/%s/%s",seventTypeArrayMix->String().Data(),striggerArrayMix->String().Data(),EVENTSELECTIONMIX,MIXTRIGGER,mergeCentralityName.Data(),PairSelection,o->GetName());
                } else continue;
            }
        }
    }
}
