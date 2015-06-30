import FWCore.ParameterSet.Config as cms

from Configuration.Generator.Pythia8CommonSettings_cfi import *
from Configuration.Generator.Pythia8CUEP8M1Settings_cfi import *

generator = cms.EDFilter(
    "Pythia8PtGunTFunc",

    maxEventsToPrint = cms.untracked.int32(5),
    pythiaPylistVerbosity = cms.untracked.int32(1),
    pythiaHepMCVerbosity = cms.untracked.bool(True),
    
    PGunParameters = cms.PSet(
  #G      ParticleID = cms.vint32(5),
  #G      AddAntiParticle = cms.bool(True),
        ParticleID = cms.vint32(553),
        AddAntiParticle = cms.bool(False),
        MinPhi = cms.double(-3.14159265359),
        MaxPhi = cms.double(3.14159265359),
        MinPt = cms.double(0.0),
        MaxPt = cms.double(40.0),
        MinEta = cms.double(-2.4),
        MaxEta = cms.double(2.4),
        TFunction_string = cms.string("1/((1+[0]*x*x)**6)"),
        TFunction_min = cms.double("5"),
   	TFunction_max = cms.double("40")


        ),
    
    PythiaParameters = cms.PSet(
         pythiaUpsilonDecays = cms.vstring(
             '553:onMode = off',		#Turn off Upsilon decays
             '553:onIfMatch = 13 -13',		#Lets only Upsilon -> mu+ mu-
        ),
        parameterSets = cms.vstring('pythiaUpsilonDecays')
    ),
    parameterSets = cms.vstring('pythia8CommonSettings',
                                'pythia8CUEP8M1Settings',
                                'processParameters',
    )
      
)

ProductionFilterSequence = cms.Sequence(generator)
