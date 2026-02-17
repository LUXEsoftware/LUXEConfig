#!/usr/bin/env python3

from Configurables import GeoSvc, MarlinProcessorWrapper, EventDataSvc
from Gaudi.Configuration import INFO
from k4FWCore import ApplicationMgr, IOSvc
from k4FWCore.parseArgs import parser
from k4MarlinWrapper.io_helpers import IOHandlerHelper

parser.add_argument(
    "--inputFile", help="Input file from simulation", type=str, default=""
)
parser.add_argument(
    "-o",
    "--outputFile",
    help="The name of the output file",
    type=str,
    default="luxe_reconstruction.edm4hep.root",
)
parser.add_argument(
    "--compactFile", help="Compact detector file to use", type=str, default=""
)

reco_args = parser.parse_known_args()[0]

svc_list = [EventDataSvc("EventDataSvc")]
# Setup inputs and outputs
io_svc = IOSvc(
    "IOSvc",
    OutputLevel=INFO,
)
# Only load a geometry if it is passed
if reco_args.compactFile:
    svc_list.append(
        GeoSvc(
            "GeoSvc",
            detectors=[reco_args.compactFile],
            OutputLevel=INFO,
            EnableGeant4Geo=False,
        )
    )

alg_list = []
io_handler = IOHandlerHelper(alg_list, io_svc)
io_handler.add_reader(reco_args.inputFile)

MyEcalpReco = MarlinProcessorWrapper("MyEcalpReco")
MyEcalpReco.ProcessorType = "RealisticCaloRecoSilicon"
MyEcalpReco.Parameters = {
    "CellIDLayerString": ["layer"],
    "calibration_factorsMipGev": ["0.00616736103247","0.0125274552256"],
    "calibration_layergroups": ["20", "1"],
    "inputHitCollections": ["PixelSiEcalCollectionDigi"],
    "inputRelationCollections": ["EcalpRelationsSimDigi"],
    "outputHitCollections": ["PixelSiEcalCollectionRec"],
    "outputRelationCollections": ["EcalpRelationsSimRec"],
}

MyEcalpDigi.ProcessorType = "RealisticCaloDigiSilicon"
MyEcalpDigi.Parameters = {
    "CellIDLayerString": ["layer"],
    "calibration_mip": ["0.0001525"],
    "inputHitCollections": ["PixelSiEcalCollection"],
    "outputHitCollections": ["PixelSiEcalCollectionDigi"],
    "outputRelationCollections": ["EcalpRelationsSimDigi"],
    "threshold": ["0"],
    "timingCut": ["1"],
}

alg_list.extend([
    MyEcalpDigi,
    MyEcalpReco    
])


io_handler.add_edm4hep_writer(reco_args.outputFileBase)
io_handler.finalize_converters()

app_mgr = ApplicationMgr(
    TopAlg=alg_list, EvtSel="NONE", EvtMax=10, ExtSvc=svc_list, OutputLevel=INFO
)
