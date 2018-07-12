import FWCore.ParameterSet.Config as cms
import subprocess

from FWCore.PythonUtilities.LumiList import LumiList
from FWCore.ParameterSet.VarParsing import VarParsing

options = VarParsing("analysis")
options.register("runList"
                 , []
                 , VarParsing.multiplicity.list
                 , VarParsing.varType.int
                 , "Run selection")
options.register("lumiList"
                 #, "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions18/13TeV/DCSOnly/json_DCSONLY.txt"
                 , "/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/DCSOnly/json_DCSONLY.txt"
                 , VarParsing.multiplicity.singleton
                 , VarParsing.varType.string
                 , "JSON file")
options.parseArguments()

lumilist = LumiList(filename = options.lumiList)
if len(options.runList) :
    runlist = LumiList(runs = options.runList)
    lumilist = lumilist & runlist
    if not len(lumilist) :
        raise RuntimeError, "The resulting LumiList is empty"

process = cms.Process("testRPCCPPF")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
#process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
#process.load('Configuration.Geometry.GeometryDB_cff')
#process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.Geometry.GeometryExtended2016_cff')
process.load('Configuration.Geometry.GeometryExtended2016Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedRealistic50ns13TeVCollision_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = "92X_dataRun2_Express_v7"
process.GlobalTag.globaltag = "101X_dataRun2_Express_v7"

#process.SimpleMemoryCheck = cms.Service("SimpleMemoryCheck",
#    ignoreTotal = cms.untracked.int32(1)
#)

process.load("EventFilter.RPCRawToDigi.RPCCPPFRawToDigi_sqlite_cff")
process.load("EventFilter.RPCRawToDigi.rpcPacker_cfi")
process.rpcpacker.InputLabel = cms.InputTag("rpcCPPFRawToDigi")
process.load("EventFilter.RPCRawToDigi.rpcUnpackingModule_cfi")
process.rpcUnpackingModulePacked = process.rpcUnpackingModule.clone()
process.rpcUnpackingModulePacked.InputLabel = cms.InputTag("rpcpacker")

process.load("EventFilter.RPCRawToDigi.rpcUnpacker_cfi")
import EventFilter.RPCRawToDigi.rpcUnpacker_cfi
muonRPCDigis = EventFilter.RPCRawToDigi.rpcUnpacker_cfi.rpcunpacker.clone()
muonRPCDigis.InputLabel = 'rawDataCollector'

process.load("EventFilter.RPCRawToDigi.rpcUnpacker_cfi")
import EventFilter.RPCRawToDigi.rpcUnpacker_cfi
muonRPCDigis = EventFilter.RPCRawToDigi.rpcUnpacker_cfi.rpcunpacker.clone()
muonRPCDigis.InputLabel = 'rawDataCollector'

process.load('RecoLocalMuon.RPCRecHit.rpcRecHits_cfi')
from RecoLocalMuon.RPCRecHit.rpcRecHits_cfi import *
process.rpcRecHits.rpcDigiLabel = 'rpcunpacker'

process.load('L1Trigger.L1TMuonCPPF.emulatorCppfDigis_cfi')
from L1Trigger.L1TMuonCPPF.emulatorCppfDigis_cfi import *
process.emulatorCppfDigis.recHitLabel = 'rpcRecHits'

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )


# Input source
eos_cmd = '/afs/cern.ch/project/eos/installation/ams/bin/eos.select'
readFiles = cms.untracked.vstring()
process.source = cms.Source("PoolSource",
        fileNames = readFiles,
)
#in_dir_name = '/eos/cms/store/express/Commissioning2018/ExpressPhysics/FEVT/Express-v1/000/314/574/00000/'
#in_dir_name = '/eos/cms/store/data/Run2017F/SingleMuon/RAW/v1/000/306/121/00000/'
#in_dir_name = '/eos/cms/store/data/Commissioning2018/ZeroBias/RAW/v1/000/314/663/00000/'
#in_dir_name = '/eos/cms/store/data/Run2018A/ZeroBias/RAW/v1/000/315/420/00000/'
#in_dir_name = '/eos/cms/store/data/Run2018A/SingleMuon/RAW/v1/000/315/764/00000/'
#in_dir_name = '/eos/cms/store/data/Run2018A/SingleMuon/RAW-RECO/ZMu-PromptReco-v1/000/315/800/00000/'
in_dir_name = '/eos/cms/store/data/Run2018A/SingleMuon/RAW-RECO/ZMu-PromptReco-v1/000/315/257/00000/'
#in_dir_name = '/eos/ems/store/data/Run2018A/SingleMuon/RAW-RECO/ZMu-PromptReco-v3/000/316/995/00000/'

#readFiles.extend( cms.untracked.vstring('file:'+in_dir_name+'4CD446F8-8643-E811-ADE1-FA163E24B9CF.root') )
#readFiles.extend( cms.untracked.vstring('file:'+in_dir_name+'AADB01BE-474C-E811-A57C-02163E013D6F.root') )

iFile = 0
for in_file_name in subprocess.check_output([eos_cmd, 'ls', in_dir_name]).splitlines():
    if not ('.root' in in_file_name): continue
    iFile += 1
    readFiles.extend( cms.untracked.vstring('file:'+in_dir_name+in_file_name) )


process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )
#process.maxLuminosityBlocks = cms.untracked.PSet(input = cms.untracked.int32(10))

process.p = cms.Path( process.rpcUnpackingModule 
			* process.rpcCPPFRawToDigi * process.rpcpacker * process.rpcUnpackingModulePacked + 
			 
			process.rpcunpacker * process.rpcRecHits * process.emulatorCppfDigis
)

# Output
process.out = cms.OutputModule("PoolOutputModule"
                               , outputCommands = cms.untracked.vstring("drop *",
									"keep *_rpcunpacker_*_*",
                                                                        "keep *_emulatorCppfDigis_*_*",
                                                                        "keep *_rpcRecHits_*_*",
                                                                        "keep *_rpcCPPFRawToDigi_*_*")
                               #, fileName = cms.untracked.string(options.outputFile)
                               , fileName = cms.untracked.string("test_cppf_unpacker_emulator.root")
                               , SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring("p"))
)

process.e = cms.EndPath(process.out)

