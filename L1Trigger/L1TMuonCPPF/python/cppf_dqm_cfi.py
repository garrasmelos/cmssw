import FWCore.ParameterSet.Config as cms

DQM_CPPF = cms.EDAnalyzer('DQM_CPPF',
                      cppfdigiLabel1 = cms.InputTag("emulatorCppfDigis","recHit"),   
		                  cppfdigiLabel2 = cms.InputTag("rpcCPPFRawToDigi"),
		                  cppfdigiLabel3 = cms.InputTag("emtfStage2Digis"),
		                  cppfdigiLabel4 = cms.InputTag("simEmtfDigisData")
)
