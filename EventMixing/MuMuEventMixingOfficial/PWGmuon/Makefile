# Makefile for PWGmuon -- automatically generated

# Include this file from the ROOT installed on the system where we are using the PARfile
include $(shell root-config --etcdir)/Makefile.arch

# Name of this module
PAR_MODULE := PWGmuon

# This module depends on the following libraries
PAR_LIBDEPS := libANALYSISalice libAOD libCORRFW libOADB libSTEERBase libEventMixing

# Current OS
OS := $(shell uname)

# ROOT major version
ROOT_MAJOR_VERSION := $(shell root-config --version | cut -f1 -d.)

# Include paths: take into consideration "extra includes" (manually entered by users) and includes
# from other previously enabled PARfiles
PAR_EXTRAINCLUDES = ../OADB/COMMON/MULTIPLICITY
PAR_INCLUDES = -I. $(addprefix -I../,$(ALICE_PARFILES_ENABLED)) $(addprefix -I,$(PAR_EXTRAINCLUDES)) -I$(ALICE_ROOT)/include -I$(ALICE_PHYSICS)/include -I$(shell root-config --incdir)
ifneq ($(FASTJET),)
	PAR_INCLUDES += -I$(FASTJET)/include
endif

# Get C++ flags form ROOT
PAR_CXXFLAGS = $(CXXFLAGS) $(PAR_INCLUDES)

# Get linker flags from ROOT
PAR_SOFLAGS = $(SOFLAGS) $(LDFLAGS)
ifeq ($(OS),Darwin)
# Only on OS X
	PAR_SOFLAGS += -undefined dynamic_lookup
endif

# Get some ROOT build flags from the current installation
ROOT_DEFINES += $(shell root-config --features | grep -q xml && echo '-DWITHXML')
ROOT_DEFINES += $(shell root-config --features | grep -q alien && echo '-DWITHALIEN')
ifneq ($(FASTJET),)
	ROOT_DEFINES += -DHAVE_FASTJET
endif

SRCS := AliAnalysisCountTriggers.cxx AliAnalysisMuMuBase.cxx AliAnalysisMuMuBinning.cxx AliAnalysisMuMuCutCombination.cxx AliAnalysisMuMuCutElement.cxx AliAnalysisMuMuCutRegistry.cxx AliAnalysisMuMuEventCutter.cxx AliAnalysisMuMuGlobal.cxx AliAnalysisMuMuMCGene.cxx AliAnalysisMuMuMinv.cxx AliAnalysisMuMuNch.cxx AliAnalysisMuMuSingle.cxx AliAnalysisMuonUtility.cxx AliAnalysisNonMuonTrackCuts.cxx AliAnalysisNonPrimaryVertices.cxx AliAnalysisTaskAOD2MuonAOD.cxx AliAnalysisTaskAOD2MuonAOD.cxx AliAnalysisTaskCreateMixedDimuons.cxx AliAnalysisTaskDimuonCFContainerBuilder.cxx AliAnalysisTaskESDMuonFilter.cxx AliAnalysisTaskFlowSingleMu.cxx AliAnalysisTaskFromStandardToMuonAOD.cxx AliAnalysisTaskLinkToMC.cxx AliAnalysisTaskLUT.cxx AliAnalysisTaskMTRResponse.cxx AliAnalysisTaskMuMu.cxx AliAnalysisTaskMuMu.cxx AliAnalysisTaskMuonAODCreation.cxx AliAnalysisTaskMuonAODfromGeneral.cxx AliAnalysisTaskMuonCollisionMultiplicity.cxx AliAnalysisTaskMuonDistributions.cxx AliAnalysisTaskMuonTreeBuilder.cxx AliAnalysisTaskNorm.cxx AliAnalysisTaskOnTheFlyAliAODDimuon.cxx AliAnalysisTaskSEMuonsHF.cxx AliAnalysisTaskSingleMu.cxx AliAnalysisTaskSingleMuESD.cxx AliAODEventInfo.cxx AliAODMuonReplicator.cxx AliCFMuonResTask1.cxx AliCFMuonResUpsilon.cxx AliCFMuonSingleTask1.cxx AliDimuInfoStoreMC.cxx AliDimuInfoStoreRD.cxx AliESDMuonTrackCuts.cxx AliEventPoolMuon.cxx AliHistogramCollection.cxx AliMTRParameterizedResponse.cxx AliMuonAnalysisOutput.cxx AliMuonEventCuts.cxx AliMuonInfoStoreMC.cxx AliMuonInfoStoreRD.cxx AliMuonPairCuts.cxx AliMuonsHFHeader.cxx AliMuonTrackCuts.cxx AliMuonTriggerCombo.cxx AliOADBMuonTrackCutsParam.cxx AliUtilityMuonAncestor.cxx AliVAnalysisMuon.cxx AliAnalysisTaskLMREventFilter.cxx AliLMREvent.cxx AliLMRMuon.cxx
HDRS := AliAnalysisCountTriggers.h AliAnalysisMuMuBase.h AliAnalysisMuMuBinning.h AliAnalysisMuMuCutCombination.h AliAnalysisMuMuCutElement.h AliAnalysisMuMuCutRegistry.h AliAnalysisMuMuEventCutter.h AliAnalysisMuMuGlobal.h AliAnalysisMuMuMCGene.h AliAnalysisMuMuMinv.h AliAnalysisMuMuNch.h AliAnalysisMuMuSingle.h AliAnalysisMuonUtility.h AliAnalysisNonMuonTrackCuts.h AliAnalysisNonPrimaryVertices.h AliAnalysisTaskAOD2MuonAOD.h AliAnalysisTaskAOD2MuonAOD.h AliAnalysisTaskCreateMixedDimuons.h AliAnalysisTaskDimuonCFContainerBuilder.h AliAnalysisTaskESDMuonFilter.h AliAnalysisTaskFlowSingleMu.h AliAnalysisTaskFromStandardToMuonAOD.h AliAnalysisTaskLinkToMC.h AliAnalysisTaskLUT.h AliAnalysisTaskMTRResponse.h AliAnalysisTaskMuMu.h AliAnalysisTaskMuMu.h AliAnalysisTaskMuonAODCreation.h AliAnalysisTaskMuonAODfromGeneral.h AliAnalysisTaskMuonCollisionMultiplicity.h AliAnalysisTaskMuonDistributions.h AliAnalysisTaskMuonTreeBuilder.h AliAnalysisTaskNorm.h AliAnalysisTaskOnTheFlyAliAODDimuon.h AliAnalysisTaskSEMuonsHF.h AliAnalysisTaskSingleMu.h AliAnalysisTaskSingleMuESD.h AliAODEventInfo.h AliAODMuonReplicator.h AliCFMuonResTask1.h AliCFMuonResUpsilon.h AliCFMuonSingleTask1.h AliDimuInfoStoreMC.h AliDimuInfoStoreRD.h AliESDMuonTrackCuts.h AliEventPoolMuon.h AliHistogramCollection.h AliMTRParameterizedResponse.h AliMuonAnalysisOutput.h AliMuonEventCuts.h AliMuonInfoStoreMC.h AliMuonInfoStoreRD.h AliMuonPairCuts.h AliMuonsHFHeader.h AliMuonTrackCuts.h AliMuonTriggerCombo.h AliOADBMuonTrackCutsParam.h AliUtilityMuonAncestor.h AliVAnalysisMuon.h AliAnalysisTaskLMREventFilter.h AliLMREvent.h AliLMRMuon.h

DHDR := $(PAR_MODULE)LinkDef.h

SRCS += G__$(PAR_MODULE).cxx
OBJS := $(SRCS:.cxx=.o)

COLM := \033[35m
COLC := \033[36m
COLG := \033[32m
COLZ := \033[m

# Multiplatform echo. Interprets escapes. Use it with $(call ECHO,"<message>")
ECHO = printf $(1)"\n"

# Default rule (it's the first one, "all" is just a convention)
all: lib$(PAR_MODULE).rootmap lib$(PAR_MODULE).so
	@$(call ECHO,"[$(COLM)$(PAR_MODULE)$(COLZ)] $(COLG)PARfile built successfully$(COLZ)")

# Abstract rule to compile classes to objects
%.o: %.cxx %.h
	@$(call ECHO,"[$(COLM)$(PAR_MODULE)$(COLZ)] compiling $(COLC)$<$(COLZ)")
	@$(CXX) $(PAR_CXXFLAGS) -c $< -o $@

# Generate library from objects
lib$(PAR_MODULE).so: $(OBJS)
	@$(call ECHO,"[$(COLM)$(PAR_MODULE)$(COLZ)] building shared library")
	@$(LD) $(PAR_SOFLAGS) $^ -o $@ $(EXPLLINKLIBS)

# Generate dictionary (rule broken up to accommodate parallel builds)
G__$(PAR_MODULE).h: G__$(PAR_MODULE).cxx

G__$(PAR_MODULE).cxx: $(HDRS) $(DHDR)
	@$(call ECHO,"[$(COLM)$(PAR_MODULE)$(COLZ)] generating dictionary")
ifeq ($(ROOT_MAJOR_VERSION),5)
	@rootcint -f $@ -c -p $(ROOT_DEFINES) $(PAR_INCLUDES) $^
endif
ifeq ($(ROOT_MAJOR_VERSION),6)
	@rootcint -f $@ -rmf lib${PAR_MODULE}.rootmap -rml lib${PAR_MODULE} $(ROOT_DEFINES) $(PAR_INCLUDES) $^
endif

# Generate rootmap (for automatic loading of libraries)
lib$(PAR_MODULE).rootmap: lib$(PAR_MODULE).so
	@$(call ECHO,"[$(COLM)$(PAR_MODULE)$(COLZ)] generating rootmap")
ifeq ($(ROOT_MAJOR_VERSION),5)
	@rlibmap -o $@ -l lib$(PAR_MODULE) -d $(PAR_LIBDEPS) -c $(DHDR)
endif

# Delete all
clean:
	rm -f $(OBJS) *.so G__$(PAR_MODULE).* *.rootmap
