/********************************************************************
* /global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Source/GarfieldDict.h
* CAUTION: DON'T CHANGE THIS FILE. THIS FILE IS AUTOMATICALLY GENERATED
*          FROM HEADER FILES LISTED IN G__setup_cpp_environmentXXX().
*          CHANGE THOSE HEADER FILES AND REGENERATE THIS FILE.
********************************************************************/
#ifdef __CINT__
#error /global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Source/GarfieldDict.h/C is only for compilation. Abort cint.
#endif
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define G__ANSIHEADER
#define G__DICTIONARY
#define G__PRIVATE_GVALUE
#include "G__ci.h"
#include "FastAllocString.h"
extern "C" {
extern void G__cpp_setup_tagtableGarfieldDict();
extern void G__cpp_setup_inheritanceGarfieldDict();
extern void G__cpp_setup_typetableGarfieldDict();
extern void G__cpp_setup_memvarGarfieldDict();
extern void G__cpp_setup_globalGarfieldDict();
extern void G__cpp_setup_memfuncGarfieldDict();
extern void G__cpp_setup_funcGarfieldDict();
extern void G__set_cpp_environmentGarfieldDict();
}


#include "TObject.h"
#include "TMemberInspector.h"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/AvalancheMC.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/AvalancheMicroscopic.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/AvalancheMicroscopic_josh.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/ComponentAnalyticField.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/ComponentAnsys121.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/ComponentAnsys123.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/ComponentBase.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/ComponentCST.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/ComponentConstant.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/ComponentElmer.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/ComponentFieldMap.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/ComponentNeBem2d.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/ComponentTcad2d.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/ComponentTcad3d.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/ComponentUser.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/DriftLineRKF.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/FundamentalConstants.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/GarfieldConstants.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/GeometryBase.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/GeometryRoot.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/GeometrySimple.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/MagboltzInterface.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/Medium.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/MediumConductor.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/MediumGas.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/MediumMagboltz.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/MediumPlastic.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/MediumSilicon.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/Numerics.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/OpticalData.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/Plotting.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/PlottingEngine.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/PlottingEngineRoot.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/Random.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/RandomEngine.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/RandomEngineRoot.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/Sensor.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/Solid.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/SolidBox.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/SolidTube.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/Track.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/TrackBichsel.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/TrackElectron.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/TrackHeed.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/TrackPAI.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/TrackSimple.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/ViewCell.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/ViewDrift.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/ViewFEMesh.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/ViewField.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/ViewGeometry.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/ViewMedium.hh"
#include "/global/homes/y/ynakajim/mywork/hpxe/garfpp_recomb//Include/ViewSignal.hh"
#include <algorithm>
namespace std { }
using namespace std;

#ifndef G__MEMFUNCBODY
#endif

extern G__linked_taginfo G__GarfieldDictLN_TClass;
extern G__linked_taginfo G__GarfieldDictLN_TBuffer;
extern G__linked_taginfo G__GarfieldDictLN_TMemberInspector;
extern G__linked_taginfo G__GarfieldDictLN_TObject;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEdoublecOallocatorlEdoublegRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEboolcOallocatorlEboolgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_string;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_TList;
extern G__linked_taginfo G__GarfieldDictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_Garfield;
extern G__linked_taginfo G__GarfieldDictLN_GarfieldcLcLMedium;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEvectorlEdoublecOallocatorlEdoublegRsPgRcOallocatorlEvectorlEdoublecOallocatorlEdoublegRsPgRsPgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEvectorlEdoublecOallocatorlEdoublegRsPgRcOallocatorlEvectorlEdoublecOallocatorlEdoublegRsPgRsPgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEvectorlEvectorlEdoublecOallocatorlEdoublegRsPgRcOallocatorlEvectorlEdoublecOallocatorlEdoublegRsPgRsPgRsPgRcOallocatorlEvectorlEvectorlEdoublecOallocatorlEdoublegRsPgRcOallocatorlEvectorlEdoublecOallocatorlEdoublegRsPgRsPgRsPgRsPgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEvectorlEvectorlEdoublecOallocatorlEdoublegRsPgRcOallocatorlEvectorlEdoublecOallocatorlEdoublegRsPgRsPgRsPgRcOallocatorlEvectorlEvectorlEdoublecOallocatorlEdoublegRsPgRcOallocatorlEvectorlEdoublecOallocatorlEdoublegRsPgRsPgRsPgRsPgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEvectorlEvectorlEvectorlEdoublecOallocatorlEdoublegRsPgRcOallocatorlEvectorlEdoublecOallocatorlEdoublegRsPgRsPgRsPgRcOallocatorlEvectorlEvectorlEdoublecOallocatorlEdoublegRsPgRcOallocatorlEvectorlEdoublecOallocatorlEdoublegRsPgRsPgRsPgRsPgRsPgRcOallocatorlEvectorlEvectorlEvectorlEdoublecOallocatorlEdoublegRsPgRcOallocatorlEvectorlEdoublecOallocatorlEdoublegRsPgRsPgRsPgRcOallocatorlEvectorlEvectorlEdoublecOallocatorlEdoublegRsPgRcOallocatorlEvectorlEdoublecOallocatorlEdoublegRsPgRsPgRsPgRsPgRsPgRsPgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEvectorlEvectorlEvectorlEdoublecOallocatorlEdoublegRsPgRcOallocatorlEvectorlEdoublecOallocatorlEdoublegRsPgRsPgRsPgRcOallocatorlEvectorlEvectorlEdoublecOallocatorlEdoublegRsPgRcOallocatorlEvectorlEdoublecOallocatorlEdoublegRsPgRsPgRsPgRsPgRsPgRcOallocatorlEvectorlEvectorlEvectorlEdoublecOallocatorlEdoublegRsPgRcOallocatorlEvectorlEdoublecOallocatorlEdoublegRsPgRsPgRsPgRcOallocatorlEvectorlEvectorlEdoublecOallocatorlEdoublegRsPgRcOallocatorlEvectorlEdoublecOallocatorlEdoublegRsPgRsPgRsPgRsPgRsPgRsPgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_GarfieldcLcLGeometryBase;
extern G__linked_taginfo G__GarfieldDictLN_GarfieldcLcLComponentBase;
extern G__linked_taginfo G__GarfieldDictLN_GarfieldcLcLSensor;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEGarfieldcLcLSensorcLcLcomponentcOallocatorlEGarfieldcLcLSensorcLcLcomponentgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEGarfieldcLcLSensorcLcLcomponentcOallocatorlEGarfieldcLcLSensorcLcLcomponentgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEGarfieldcLcLSensorcLcLelectrodecOallocatorlEGarfieldcLcLSensorcLcLelectrodegRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEGarfieldcLcLSensorcLcLelectrodecOallocatorlEGarfieldcLcLSensorcLcLelectrodegRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEGarfieldcLcLSensorcLcLthresholdCrossingcOallocatorlEGarfieldcLcLSensorcLcLthresholdCrossinggRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEGarfieldcLcLSensorcLcLthresholdCrossingcOallocatorlEGarfieldcLcLSensorcLcLthresholdCrossinggRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_TQObjSender;
extern G__linked_taginfo G__GarfieldDictLN_TView;
extern G__linked_taginfo G__GarfieldDictLN_TCanvas;
extern G__linked_taginfo G__GarfieldDictLN_iteratorlEbidirectional_iterator_tagcOTObjectmUcOlongcOconstsPTObjectmUmUcOconstsPTObjectmUaNgR;
extern G__linked_taginfo G__GarfieldDictLN_TH1;
extern G__linked_taginfo G__GarfieldDictLN_TPointSet3D;
extern G__linked_taginfo G__GarfieldDictLN_GarfieldcLcLViewDrift;
extern G__linked_taginfo G__GarfieldDictLN_vectorlETPolyLine3DcOallocatorlETPolyLine3DgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlETPolyLine3DcOallocatorlETPolyLine3DgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_vectorlETPointSet3DcOallocatorlETPointSet3DgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlETPointSet3DcOallocatorlETPointSet3DgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEGarfieldcLcLViewDriftcLcLmarkercOallocatorlEGarfieldcLcLViewDriftcLcLmarkergRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEGarfieldcLcLViewDriftcLcLmarkercOallocatorlEGarfieldcLcLViewDriftcLcLmarkergRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_GarfieldcLcLAvalancheMC;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEGarfieldcLcLAvalancheMCcLcLdriftPointcOallocatorlEGarfieldcLcLAvalancheMCcLcLdriftPointgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEGarfieldcLcLAvalancheMCcLcLdriftPointcOallocatorlEGarfieldcLcLAvalancheMCcLcLdriftPointgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEGarfieldcLcLAvalancheMCcLcLavalPointcOallocatorlEGarfieldcLcLAvalancheMCcLcLavalPointgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEGarfieldcLcLAvalancheMCcLcLavalPointcOallocatorlEGarfieldcLcLAvalancheMCcLcLavalPointgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEGarfieldcLcLAvalancheMCcLcLendpointcOallocatorlEGarfieldcLcLAvalancheMCcLcLendpointgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEGarfieldcLcLAvalancheMCcLcLendpointcOallocatorlEGarfieldcLcLAvalancheMCcLcLendpointgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_TVectorTlEfloatgR;
extern G__linked_taginfo G__GarfieldDictLN_TVectorTlEdoublegR;
extern G__linked_taginfo G__GarfieldDictLN_TF1;
extern G__linked_taginfo G__GarfieldDictLN_TH1D;
extern G__linked_taginfo G__GarfieldDictLN_GarfieldcLcLAvalancheMicroscopic;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEGarfieldcLcLAvalancheMicroscopiccLcLpointcOallocatorlEGarfieldcLcLAvalancheMicroscopiccLcLpointgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEGarfieldcLcLAvalancheMicroscopiccLcLpointcOallocatorlEGarfieldcLcLAvalancheMicroscopiccLcLpointgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEGarfieldcLcLAvalancheMicroscopiccLcLelectroncOallocatorlEGarfieldcLcLAvalancheMicroscopiccLcLelectrongRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEGarfieldcLcLAvalancheMicroscopiccLcLelectroncOallocatorlEGarfieldcLcLAvalancheMicroscopiccLcLelectrongRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEGarfieldcLcLAvalancheMicroscopiccLcLphotoncOallocatorlEGarfieldcLcLAvalancheMicroscopiccLcLphotongRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEGarfieldcLcLAvalancheMicroscopiccLcLphotoncOallocatorlEGarfieldcLcLAvalancheMicroscopiccLcLphotongRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEintcOallocatorlEintgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEintcOallocatorlEintgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_complexlEdoublegR;
extern G__linked_taginfo G__GarfieldDictLN_GarfieldcLcLComponentAnalyticField;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEstringcOallocatorlEstringgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEstringcOallocatorlEstringgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEGarfieldcLcLComponentAnalyticFieldcLcLwirecOallocatorlEGarfieldcLcLComponentAnalyticFieldcLcLwiregRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEGarfieldcLcLComponentAnalyticFieldcLcLwirecOallocatorlEGarfieldcLcLComponentAnalyticFieldcLcLwiregRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEcomplexlEdoublegRcOallocatorlEcomplexlEdoublegRsPgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEcomplexlEdoublegRcOallocatorlEcomplexlEdoublegRsPgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEGarfieldcLcLComponentAnalyticFieldcLcLstripcOallocatorlEGarfieldcLcLComponentAnalyticFieldcLcLstripgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEGarfieldcLcLComponentAnalyticFieldcLcLstripcOallocatorlEGarfieldcLcLComponentAnalyticFieldcLcLstripgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEGarfieldcLcLComponentAnalyticFieldcLcLplanecOallocatorlEGarfieldcLcLComponentAnalyticFieldcLcLplanegRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEGarfieldcLcLComponentAnalyticFieldcLcLplanecOallocatorlEGarfieldcLcLComponentAnalyticFieldcLcLplanegRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEvectorlEcomplexlEdoublegRcOallocatorlEcomplexlEdoublegRsPgRsPgRcOallocatorlEvectorlEcomplexlEdoublegRcOallocatorlEcomplexlEdoublegRsPgRsPgRsPgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEvectorlEcomplexlEdoublegRcOallocatorlEcomplexlEdoublegRsPgRsPgRcOallocatorlEvectorlEcomplexlEdoublegRcOallocatorlEcomplexlEdoublegRsPgRsPgRsPgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEGarfieldcLcLComponentAnalyticFieldcLcLcharge3dcOallocatorlEGarfieldcLcLComponentAnalyticFieldcLcLcharge3dgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEGarfieldcLcLComponentAnalyticFieldcLcLcharge3dcOallocatorlEGarfieldcLcLComponentAnalyticFieldcLcLcharge3dgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_TMatrixTBaselEfloatgR;
extern G__linked_taginfo G__GarfieldDictLN_TMatrixTBaselEdoublegR;
extern G__linked_taginfo G__GarfieldDictLN_TElementActionTlEdoublegR;
extern G__linked_taginfo G__GarfieldDictLN_TElementPosActionTlEdoublegR;
extern G__linked_taginfo G__GarfieldDictLN_TMatrixTlEdoublegR;
extern G__linked_taginfo G__GarfieldDictLN_TMatrixTRow_constlEdoublegR;
extern G__linked_taginfo G__GarfieldDictLN_TMatrixTRowlEdoublegR;
extern G__linked_taginfo G__GarfieldDictLN_TMatrixTDiag_constlEdoublegR;
extern G__linked_taginfo G__GarfieldDictLN_TMatrixTColumn_constlEdoublegR;
extern G__linked_taginfo G__GarfieldDictLN_TMatrixTFlat_constlEdoublegR;
extern G__linked_taginfo G__GarfieldDictLN_TMatrixTSub_constlEdoublegR;
extern G__linked_taginfo G__GarfieldDictLN_TMatrixTSparseRow_constlEdoublegR;
extern G__linked_taginfo G__GarfieldDictLN_TMatrixTSparseDiag_constlEdoublegR;
extern G__linked_taginfo G__GarfieldDictLN_TMatrixTColumnlEdoublegR;
extern G__linked_taginfo G__GarfieldDictLN_TMatrixTDiaglEdoublegR;
extern G__linked_taginfo G__GarfieldDictLN_TMatrixTFlatlEdoublegR;
extern G__linked_taginfo G__GarfieldDictLN_TMatrixTSublEdoublegR;
extern G__linked_taginfo G__GarfieldDictLN_TMatrixTSparseRowlEdoublegR;
extern G__linked_taginfo G__GarfieldDictLN_TMatrixTSparseDiaglEdoublegR;
extern G__linked_taginfo G__GarfieldDictLN_GarfieldcLcLComponentFieldMap;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEGarfieldcLcLComponentFieldMapcLcLelementcOallocatorlEGarfieldcLcLComponentFieldMapcLcLelementgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEGarfieldcLcLComponentFieldMapcLcLelementcOallocatorlEGarfieldcLcLComponentFieldMapcLcLelementgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEGarfieldcLcLComponentFieldMapcLcLnodecOallocatorlEGarfieldcLcLComponentFieldMapcLcLnodegRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEGarfieldcLcLComponentFieldMapcLcLnodecOallocatorlEGarfieldcLcLComponentFieldMapcLcLnodegRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEGarfieldcLcLComponentFieldMapcLcLmaterialcOallocatorlEGarfieldcLcLComponentFieldMapcLcLmaterialgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEGarfieldcLcLComponentFieldMapcLcLmaterialcOallocatorlEGarfieldcLcLComponentFieldMapcLcLmaterialgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_vectorlETMatrixTlEdoublegRmUcOallocatorlETMatrixTlEdoublegRmUgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlETMatrixTlEdoublegRmUcOallocatorlETMatrixTlEdoublegRmUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_GarfieldcLcLComponentAnsys121;
extern G__linked_taginfo G__GarfieldDictLN_GarfieldcLcLComponentAnsys123;
extern G__linked_taginfo G__GarfieldDictLN_GarfieldcLcLComponentConstant;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEGarfieldcLcLComponentNeBem2dcLcLpanelcOallocatorlEGarfieldcLcLComponentNeBem2dcLcLpanelgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEGarfieldcLcLComponentNeBem2dcLcLpanelcOallocatorlEGarfieldcLcLComponentNeBem2dcLcLpanelgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEGarfieldcLcLComponentTcad2dcLcLregioncOallocatorlEGarfieldcLcLComponentTcad2dcLcLregiongRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEGarfieldcLcLComponentTcad2dcLcLregioncOallocatorlEGarfieldcLcLComponentTcad2dcLcLregiongRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEGarfieldcLcLComponentTcad2dcLcLvertexcOallocatorlEGarfieldcLcLComponentTcad2dcLcLvertexgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEGarfieldcLcLComponentTcad2dcLcLvertexcOallocatorlEGarfieldcLcLComponentTcad2dcLcLvertexgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_GarfieldcLcLComponentUser;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEGarfieldcLcLDriftLineRKFcLcLstepcOallocatorlEGarfieldcLcLDriftLineRKFcLcLstepgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEGarfieldcLcLDriftLineRKFcLcLstepcOallocatorlEGarfieldcLcLDriftLineRKFcLcLstepgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_maplElongcOintcOlesslElonggRcOallocatorlEpairlEconstsPlongcOintgRsPgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_maplEintcOTGeoElementRNmUcOlesslEintgRcOallocatorlEpairlEconstsPintcOTGeoElementRNmUgRsPgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_TGeoMaterial;
extern G__linked_taginfo G__GarfieldDictLN_TGeoManager;
extern G__linked_taginfo G__GarfieldDictLN_vectorlETGeoVolumeAssemblycLcLThreadData_tmUcOallocatorlETGeoVolumeAssemblycLcLThreadData_tmUgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlETGeoVolumeAssemblycLcLThreadData_tmUcOallocatorlETGeoVolumeAssemblycLcLThreadData_tmUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_maplElongcOTGeoNavigatorArraymUcOlesslElonggRcOallocatorlEpairlEconstsPlongcOTGeoNavigatorArraymUgRsPgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_GarfieldcLcLGeometryRoot;
extern G__linked_taginfo G__GarfieldDictLN_GarfieldcLcLSolid;
extern G__linked_taginfo G__GarfieldDictLN_GarfieldcLcLGeometrySimple;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEGarfieldcLcLGeometrySimplecLcLmediumcOallocatorlEGarfieldcLcLGeometrySimplecLcLmediumgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEGarfieldcLcLGeometrySimplecLcLmediumcOallocatorlEGarfieldcLcLGeometrySimplecLcLmediumgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEGarfieldcLcLGeometrySimplecLcLsolidcOallocatorlEGarfieldcLcLGeometrySimplecLcLsolidgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEGarfieldcLcLGeometrySimplecLcLsolidcOallocatorlEGarfieldcLcLGeometrySimplecLcLsolidgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_GarfieldcLcLMediumGas;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEGarfieldcLcLMediumGascLcLexcListElementcOallocatorlEGarfieldcLcLMediumGascLcLexcListElementgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEGarfieldcLcLMediumGascLcLexcListElementcOallocatorlEGarfieldcLcLMediumGascLcLexcListElementgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEGarfieldcLcLMediumGascLcLionListElementcOallocatorlEGarfieldcLcLMediumGascLcLionListElementgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEGarfieldcLcLMediumGascLcLionListElementcOallocatorlEGarfieldcLcLMediumGascLcLionListElementgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_GarfieldcLcLMediumMagboltz;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEGarfieldcLcLMediumMagboltzcLcLdeexcitationcOallocatorlEGarfieldcLcLMediumMagboltzcLcLdeexcitationgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEGarfieldcLcLMediumMagboltzcLcLdeexcitationcOallocatorlEGarfieldcLcLMediumMagboltzcLcLdeexcitationgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEGarfieldcLcLMediumMagboltzcLcLionProdcOallocatorlEGarfieldcLcLMediumMagboltzcLcLionProdgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEGarfieldcLcLMediumMagboltzcLcLionProdcOallocatorlEGarfieldcLcLMediumMagboltzcLcLionProdgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEGarfieldcLcLMediumMagboltzcLcLdxcProdcOallocatorlEGarfieldcLcLMediumMagboltzcLcLdxcProdgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEGarfieldcLcLMediumMagboltzcLcLdxcProdcOallocatorlEGarfieldcLcLMediumMagboltzcLcLdxcProdgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_GarfieldcLcLMediumSilicon;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEGarfieldcLcLMediumSiliconcLcLopticalDatacOallocatorlEGarfieldcLcLMediumSiliconcLcLopticalDatagRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEGarfieldcLcLMediumSiliconcLcLopticalDatacOallocatorlEGarfieldcLcLMediumSiliconcLcLopticalDatagRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_GarfieldcLcLOpticalData;
extern G__linked_taginfo G__GarfieldDictLN_GarfieldcLcLPlottingEngineRoot;
extern G__linked_taginfo G__GarfieldDictLN_GarfieldcLcLRandomEngineRoot;
extern G__linked_taginfo G__GarfieldDictLN_GarfieldcLcLSolidBox;
extern G__linked_taginfo G__GarfieldDictLN_GarfieldcLcLSolidTube;
extern G__linked_taginfo G__GarfieldDictLN_GarfieldcLcLTrack;
extern G__linked_taginfo G__GarfieldDictLN_GarfieldcLcLTrackBichsel;
extern G__linked_taginfo G__GarfieldDictLN_GarfieldcLcLTrackElectron;
extern G__linked_taginfo G__GarfieldDictLN_HeedParticle;
extern G__linked_taginfo G__GarfieldDictLN_HeedMatterDef;
extern G__linked_taginfo G__GarfieldDictLN_GasDef;
extern G__linked_taginfo G__GarfieldDictLN_MatterDef;
extern G__linked_taginfo G__GarfieldDictLN_AtomPhotoAbsCS;
extern G__linked_taginfo G__GarfieldDictLN_MolecPhotoAbsCS;
extern G__linked_taginfo G__GarfieldDictLN_EnergyMesh;
extern G__linked_taginfo G__GarfieldDictLN_EnTransfCS;
extern G__linked_taginfo G__GarfieldDictLN_ElElasticScat;
extern G__linked_taginfo G__GarfieldDictLN_ElElasticScatLowSigma;
extern G__linked_taginfo G__GarfieldDictLN_PairProd;
extern G__linked_taginfo G__GarfieldDictLN_HeedDeltaElectronCS;
extern G__linked_taginfo G__GarfieldDictLN_GarfieldcLcLHeedChamber;
extern G__linked_taginfo G__GarfieldDictLN_GarfieldcLcLTrackHeed;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEGarfieldcLcLTrackHeedcLcLdeltaElectroncOallocatorlEGarfieldcLcLTrackHeedcLcLdeltaElectrongRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEGarfieldcLcLTrackHeedcLcLdeltaElectroncOallocatorlEGarfieldcLcLTrackHeedcLcLdeltaElectrongRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_GarfieldcLcLTrackPAI;
extern G__linked_taginfo G__GarfieldDictLN_GarfieldcLcLViewCell;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEGarfieldcLcLViewCellWirecOallocatorlEGarfieldcLcLViewCellWiregRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEGarfieldcLcLViewCellWirecOallocatorlEGarfieldcLcLViewCellWiregRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEGarfieldcLcLViewCellTubecOallocatorlEGarfieldcLcLViewCellTubegRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEGarfieldcLcLViewCellTubecOallocatorlEGarfieldcLcLViewCellTubegRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEGarfieldcLcLViewCellPlanecOallocatorlEGarfieldcLcLViewCellPlanegRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEGarfieldcLcLViewCellPlanecOallocatorlEGarfieldcLcLViewCellPlanegRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_vectorlETPolyLinemUcOallocatorlETPolyLinemUgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlETPolyLinemUcOallocatorlETPolyLinemUgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_maplEintcOboolcOlesslEintgRcOallocatorlEpairlEconstsPintcOboolgRsPgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_TF2;
extern G__linked_taginfo G__GarfieldDictLN_GarfieldcLcLViewField;
extern G__linked_taginfo G__GarfieldDictLN_GarfieldcLcLViewGeometry;
extern G__linked_taginfo G__GarfieldDictLN_vectorlEGarfieldcLcLViewGeometryShapecOallocatorlEGarfieldcLcLViewGeometryShapegRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlEGarfieldcLcLViewGeometryShapecOallocatorlEGarfieldcLcLViewGeometryShapegRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_TGraph;
extern G__linked_taginfo G__GarfieldDictLN_GarfieldcLcLViewMedium;
extern G__linked_taginfo G__GarfieldDictLN_vectorlETF1cOallocatorlETF1gRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlETF1cOallocatorlETF1gRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_vectorlETGraphcOallocatorlETGraphgRsPgR;
extern G__linked_taginfo G__GarfieldDictLN_reverse_iteratorlEvectorlETGraphcOallocatorlETGraphgRsPgRcLcLiteratorgR;
extern G__linked_taginfo G__GarfieldDictLN_GarfieldcLcLViewSignal;

/* STUB derived class for protected member access */