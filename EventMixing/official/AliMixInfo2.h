//
// Class AliMixInfo
//
// AliMixInfo object contains information about one cut on for event mixing
// available for users containing mixing information
//
// authors:
//          Martin Vala (martin.vala@cern.ch)
//

#ifndef ALIMIXINFO2_H
#define ALIMIXINFO2_H

#include <TNamed.h>
#include <Rtypes.h>

class AliMixEventPool2;
class TH1I;
class TList;
class TCollection;
class AliMixInfo2 : public TNamed {
public:
   enum EInfoHistorgramType { kMainEvents = 0, kMixedEvents = 1, kNumTypes };

   AliMixInfo2(const char *name = "mix", const char *title = "MixInfo");
   AliMixInfo2(const AliMixInfo2 &obj);
   virtual ~AliMixInfo2();

   void Reset(){;} // Not implemented
   virtual void Print(Option_t *option = "") const;
   virtual void Draw(Option_t *option = "");
   virtual Long64_t Merge(TCollection *list);

   void Add(AliMixInfo2 *mi);

   void SetOutputList(TList *const list) { fHistogramList = list; }
   void CreateHistogram(EInfoHistorgramType type, Int_t nbins, Int_t min, Int_t max);
   void FillHistogram(AliMixInfo2::EInfoHistorgramType type, Int_t value);
   const char *GetNameHistogramByType(Int_t index) const;
   const char *GetTitleHistogramByType(Int_t index) const;
   TH1I  *GetHistogramByType(Int_t index) const;

   void    SetEventPool(AliMixEventPool2 *evPool);
   AliMixEventPool2 *GetEventPool(const char *name);


   static void DynamicExec(AliMixInfo2 *const mixInfo);
private:

   TList     *fHistogramList;  // histogram list

   AliMixInfo2 &operator=(const AliMixInfo2 &) { return *this; }

   ClassDef(AliMixInfo2, 1)

};

#endif
