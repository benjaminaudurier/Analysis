//
// Class AliMixEventPool2
//
// AliMixEventPool2 is used to find
// similar events
//
// author:
//        Martin Vala (martin.vala@cern.ch)
//

#ifndef ALIMIXEVENTPOOL2_H
#define ALIMIXEVENTPOOL2_H

#include <TObjArray.h>
#include <TNamed.h>

class TEntryList;
class AliMixEventCutObj2;
class AliVEvent;
class AliMixEventPool2 : public TNamed {
public:
   AliMixEventPool2(const char *name = "mixEventPool", const char *title = "Mix event pool");
   AliMixEventPool2(const AliMixEventPool2 &obj);
   AliMixEventPool2 &operator= (const AliMixEventPool2 &obj);
   virtual ~AliMixEventPool2();

   // prints object info
   virtual void      Print(const Option_t *option = "") const;

   // inits correctly object
   Int_t       Init();

   void        CreateEntryListsRecursivly(Int_t index);
   void        SearchIndexRecursive(Int_t num, Int_t *i, Int_t *d, Int_t &index);
   TEntryList *AddEntryList();

   Bool_t      AddEntry(Long64_t entry, AliVEvent *ev);
   TEntryList *FindEntryList(AliVEvent *ev, Int_t &idEntryList);

   void        AddCut(AliMixEventCutObj2 *cut);

   Bool_t      NeedInit() { return (fListOfEntryList.GetEntries() == 0); }
   TObjArray  *GetListOfEntryLists() { return &fListOfEntryList; }
   TObjArray  *GetListOfEventCuts() { return &fListOfEventCuts; }

   Bool_t      SetCutValuesFromBinIndex(Int_t index);
   void        SetBufferSize(Int_t buffer) { fBufferSize = buffer; }
   void        SetMixNumber(Int_t numMix) { fMixNumber = numMix; }
   Int_t       GetBufferSize() const { return fBufferSize; }
   Int_t       GetMixNumber() const { return fMixNumber; }

private:

   TObjArray   fListOfEntryList;       // list of entry lists
   TObjArray   fListOfEventCuts;       // list of entry lists

   Int_t       fBinNumber;             // bin number
   Int_t       fBufferSize;            // buffer size
   Int_t       fMixNumber;             // mixing number

   ClassDef(AliMixEventPool2, 1)
};

#endif
