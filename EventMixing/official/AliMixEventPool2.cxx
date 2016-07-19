//
// Class AliMixEventPool
//
// AliMixEventPool is used to find
// similar events
//
// author:
//        Martin Vala (martin.vala@cern.ch)
//

#include <TEntryList.h>

#include "AliLog.h"
#include "AliMixEventCutObj2.h"
#include "AliMixEventPool2.h"

ClassImp(AliMixEventPool2)

//_________________________________________________________________________________________________
AliMixEventPool2::AliMixEventPool2(const char *name, const char *title) : TNamed(name, title),
   fListOfEntryList(),
   fListOfEventCuts(),
   fBinNumber(0),
   fBufferSize(0),
   fMixNumber(0)
{
   //
   // Default constructor.
   //
   AliDebug(AliLog::kDebug + 5, "<-");
   AliDebug(AliLog::kDebug + 5, "->");
}
//_________________________________________________________________________________________________
AliMixEventPool2::AliMixEventPool2(const AliMixEventPool2 &obj) : TNamed(obj),
   fListOfEntryList(obj.fListOfEntryList),
   fListOfEventCuts(obj.fListOfEventCuts),
   fBinNumber(obj.fBinNumber),
   fBufferSize(obj.fBufferSize),
   fMixNumber(obj.fMixNumber)
{
   //
   // Copy constructor
   //
   AliDebug(AliLog::kDebug + 5, "<-");
   AliDebug(AliLog::kDebug + 5, "->");
}

//_________________________________________________________________________________________________
AliMixEventPool2 &AliMixEventPool2::operator=(const AliMixEventPool2 &obj)
{
   //
   // Assigned operator
   //
   if (&obj != this) {
      TNamed::operator=(obj);
      fListOfEntryList = obj.fListOfEntryList;
      fListOfEventCuts = obj.fListOfEventCuts;
      fBinNumber = obj.fBinNumber;
      fBufferSize = obj.fBufferSize;
      fMixNumber = obj.fMixNumber;
   }
   return *this;
}


//_________________________________________________________________________________________________
AliMixEventPool2::~AliMixEventPool2()
{
   //
   // Destructor
   //
   AliDebug(AliLog::kDebug + 5, "<-");
   AliDebug(AliLog::kDebug + 5, "->");
}
//_________________________________________________________________________________________________
void AliMixEventPool2::AddCut(AliMixEventCutObj2 *cut)
{
   //
   // Adds cut
   //

   if (cut && cut->IsValid()) fListOfEventCuts.Add(new AliMixEventCutObj2(*cut));
}
//_________________________________________________________________________________________________
void AliMixEventPool2::Print(const Option_t *option) const
{
   //
   // Prints usefull information
   //
   TObjArrayIter next(&fListOfEventCuts);
   //   Int_t c=0;
   AliMixEventCutObj2 *cut;
   while ((cut = (AliMixEventCutObj2 *) next())) {
      cut->Print(option);
   }
   AliDebug(AliLog::kDebug, Form("NumOfEntryList %d", fListOfEntryList.GetEntries()));
   TEntryList *el;
   for (Int_t i = 0; i < fListOfEntryList.GetEntries(); i++) {
      el = (TEntryList *) fListOfEntryList.At(i);
      AliDebug(AliLog::kDebug, Form("EntryList[%d] %lld", i, el->GetN()));
   }
}
//_________________________________________________________________________________________________
Int_t AliMixEventPool2::Init()
{
   //
   // Init event pool
   //
   AliDebug(AliLog::kDebug + 5, "<-");
   CreateEntryListsRecursivly(fListOfEventCuts.GetEntries() - 1);
   fBinNumber++;
   AliDebug(AliLog::kDebug, Form("fBinnumber = %d", fBinNumber));
   AddEntryList();
   AliDebug(AliLog::kDebug + 5, "->");
   return 0;
}

//_________________________________________________________________________________________________
void AliMixEventPool2::CreateEntryListsRecursivly(Int_t index)
{
   //
   // Helper function which create entrylist recursivly
   //
   AliDebug(AliLog::kDebug + 5, "<-");
   AliMixEventCutObj2 *cut;
   if (index >= 0) {
      AliDebug(AliLog::kDebug + 1, Form("index = %d", index));
      cut = dynamic_cast<AliMixEventCutObj2 *>(fListOfEventCuts.At(index));
      if (!cut) return;
      cut->Reset();
      while (cut->HasMore()) {
         cut->AddStep();
         CreateEntryListsRecursivly(index - 1);
         if (cut->HasMore()) {
            fBinNumber++;
            AliDebug(AliLog::kDebug + 1, Form("fBinnumber = %d", fBinNumber));
            AddEntryList();
            //                 PrintCurrentCutIntervals();
         }
      }
   }
   AliDebug(AliLog::kDebug + 5, "->");
}

//_________________________________________________________________________________________________
TEntryList *AliMixEventPool2::AddEntryList()
{
   //
   // Adds endtry list
   //
   AliDebug(AliLog::kDebug + 5, "<-");
   TObjArrayIter next(&fListOfEventCuts);
   AliMixEventCutObj2 *cut;
   while ((cut = (AliMixEventCutObj2 *) next())) {
      if (cut) cut->PrintCurrentInterval();
   }
   TEntryList *el = new TEntryList;
   fListOfEntryList.Add(el);
   AliDebug(AliLog::kDebug + 1, Form("Number in Entry list -> %lld", el->GetN()));
   AliDebug(AliLog::kDebug + 5, "->");
   return el;
}

//_________________________________________________________________________________________________
Bool_t AliMixEventPool2::AddEntry(Long64_t entry, AliVEvent *ev)
{
   //
   // Adds entry to correct entry list
   //
   AliDebug(AliLog::kDebug + 5, "<-");
   AliDebug(AliLog::kDebug + 5, Form("AddEntry(%lld,%p)", entry, (void *)ev));
   if (entry < 0) {
      AliDebug(AliLog::kDebug, Form("Entry %lld was NOT added !!!", entry));
      return kFALSE;
   }
   Int_t idEntryList = -1;
   TEntryList *el =  FindEntryList(ev, idEntryList);
   if (el) {
      el->Enter(entry);
      AliDebug(AliLog::kDebug, Form("Entry %lld was added with idEntryList %d !!!", entry, idEntryList));
      return kTRUE;
   }
   AliDebug(AliLog::kDebug, Form("Entry %lld was NOT added !!!", entry));
   AliDebug(AliLog::kDebug + 5, "->");
   return kFALSE;
}

//_________________________________________________________________________________________________
TEntryList *AliMixEventPool2::FindEntryList(AliVEvent *ev, Int_t &idEntryList)
{
   //
   // Find entrlist in list of entrlist
   //
   AliDebug(AliLog::kDebug + 5, "<-");
   Int_t num = fListOfEventCuts.GetEntriesFast();
   if (num < 1) return 0;
   Int_t *indexes = new Int_t[num] ;
   Int_t *lenght = new Int_t[num];
   Int_t i = 0;
   TObjArrayIter next(&fListOfEventCuts);
   AliMixEventCutObj2 *cut;
   while ((cut = (AliMixEventCutObj2 *) next())) {
      indexes[i] = cut->GetIndex(ev);
      if (indexes[i] < 0) {
         AliDebug(AliLog::kDebug, Form("idEntryList %d", -1));
         delete [] indexes;
         delete [] lenght;
         return 0;
      }
      lenght[i] = cut->GetNumberOfBins();
      AliDebug(AliLog::kDebug + 1, Form("indexes[%d] %d", i, indexes[i]));
      i++;
   }
   idEntryList = 0;
   SearchIndexRecursive(fListOfEventCuts.GetEntries() - 1, &indexes[0], &lenght[0], idEntryList);
   AliDebug(AliLog::kDebug, Form("idEntryList %d", idEntryList - 1));
   // index which start with 0 (idEntryList-1)
   delete [] indexes;
   delete [] lenght;
   AliDebug(AliLog::kDebug + 5, "->");
   return (TEntryList *) fListOfEntryList.At(idEntryList - 1);
}

//_________________________________________________________________________________________________
void AliMixEventPool2::SearchIndexRecursive(Int_t num, Int_t *i, Int_t *d, Int_t &index)
{
   //
   // Search for index of entrylist
   //
   AliDebug(AliLog::kDebug + 5, "<-");
   if (num > 0) {
      index += (i[num] - 1) * d[num - 1];
      SearchIndexRecursive(num - 1, i, d, index);
   } else {
      index += i[num];
   }
   AliDebug(AliLog::kDebug + 5, "->");
}

//_________________________________________________________________________________________________
Bool_t AliMixEventPool2::SetCutValuesFromBinIndex(Int_t index)
{
   //
   // Sets cut value from bin index
   //

   Int_t numCuts = fListOfEventCuts.GetEntriesFast();
   Int_t *lenght = new Int_t[numCuts];
   Int_t *indexes = new Int_t[numCuts];
   Long64_t timesNum = 1;
   AliMixEventCutObj2 *cut;
   Int_t i = 0, j = 0;
   for (i = 0; i < numCuts; i++) {
      cut = (AliMixEventCutObj2 *) fListOfEventCuts.At(i);
      cut->Reset();
      lenght[i] = cut->GetNumberOfBins();
      indexes[i] = 1;
      timesNum *= lenght[i];
   }

   if (index < 0 || index >= timesNum) {
//       AliError(Form("index=%d is out of range !!!", index));
      delete [] lenght;
      delete [] indexes;
      return kFALSE;
   }

   Long64_t indexNum = index;
   for (i = 0; i < numCuts; i++) {
      timesNum = 1;
      for (j = 0; j < numCuts - i - 1; j++) timesNum *= lenght[j];
      indexNum /= timesNum;
      indexes[numCuts - i - 1] = indexNum + 1;
      index -= indexNum * timesNum;
      indexNum = index;
   }

   for (i = 0; i < numCuts; i++) {
      cut = (AliMixEventCutObj2 *) fListOfEventCuts.At(i);
      for (j = 0; j < indexes[i]; j++) cut->AddStep();
      cut->PrintCurrentInterval();

   }

   for (i = 0; i < numCuts; i++) AliDebug(AliLog::kDebug, Form("indexes[%d]=%d", i, indexes[i]));

   delete [] lenght;
   delete [] indexes;

   return kTRUE;
}
