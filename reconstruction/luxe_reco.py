#!/usr/bin/env python3

from Configurables import GeoSvc, MarlinProcessorWrapper, EventDataSvc
from Gaudi.Configuration import INFO
from k4FWCore import ApplicationMgr, IOSvc
from k4FWCore.parseArgs import parser

parser.add_argument(
    "--inputFiles", help="One or multiple input files", nargs="+", action="extend"
)

parser.add_argument(
    "--compactFile", help="Compact detector file to use", type=str, default=""
)

reco_args = parser.parse_known_args()[0]

svc_list = [EventDataSvc("EventDataSvc")]

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


app_mgr = ApplicationMgr(
    TopAlg=alg_list, EvtSel="NONE", EvtMax=3, ExtSvc=svc_list, OutputLevel=INFO
)
