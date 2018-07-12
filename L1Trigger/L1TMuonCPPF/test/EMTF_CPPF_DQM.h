#ifndef L1Trigger_DQMCPPF_h
#define L1Trigger_DQMCPPF_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "Geometry/RPCGeometry/interface/RPCRoll.h"
#include "Geometry/RPCGeometry/interface/RPCGeometry.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"

#include "DataFormats/MuonDetId/interface/RPCDetId.h"
#include "DataFormats/L1TMuon/interface/CPPFDigi.h"
#include "DataFormats/Common/interface/Handle.h"

#include "CondFormats/RPCObjects/interface/RPCMaskedStrips.h"
#include "CondFormats/RPCObjects/interface/RPCDeadStrips.h"
#include "CondFormats/Serialization/interface/Serializable.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "L1Trigger/L1TMuonEndCap/interface/TrackTools.h"

#include<boost/cstdint.hpp>
#include "TH1.h"
#include "TH2.h"
#include <memory>
#include <iostream>
#include <vector>
#include "TLorentzVector.h"
#include <ios>
#include <map>


class EMTF_CPPF_DQM : public edm::EDAnalyzer {
  
 public:
  explicit EMTF_CPPF_DQM(const edm::ParameterSet&);
  ~EMTF_CPPF_DQM() override;
  void beginRun(const edm::Run&, const edm::EventSetup&) override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  int occupancy_value(int region_, int station_, int ring_);
  int bx_value(int region_, int emtfsector_);
  int GetSubsector(int emtfsector_, int lsubsector_);
  edm::ESHandle <RPCGeometry> rpcGeom;
  
 private:
  
  void beginJob() override;
  const edm::EDGetTokenT<l1t::CPPFDigiCollection> cppfDigiToken1_;
  const edm::EDGetTokenT<l1t::CPPFDigiCollection> cppfDigiToken2_;
  const edm::EDGetTokenT<l1t::CPPFDigiCollection> cppfDigiToken3_;
  const edm::EDGetTokenT<l1t::CPPFDigiCollection> cppfDigiToken4_;


  
  TH1D* h1CeVsCuMatches;

  TH1D* h1CeVsCuMatches_ch;
  TH2D* h2CeVsCuChamberCeChamberCu;
  TH2D* h2CeVsCuPhiCePhiCu;
  TH1D* h2CeVsCuPhiCePhiCuDiff;
  TH2D* h2CeVsCuThetaCeThetaCu;
  TH2D* h2CeVsCuBxCeBxCu;

  TH1D* h1CeVsCuMatches_bx;
  TH2D* h2CeVsCuPhiCePhiCu_bx;
  TH2D* h2CeVsCuThetaCeThetaCu_bx;
  TH2D* h2CeVsCuBxCeBxCu_bx;

  TH1D* h1CeVsCuMatches_int;
  TH2D* h2CeVsCuPhiCePhiCu_int;
  TH2D* h2CeVsCuThetaCeThetaCu_int;
  TH2D* h2CeVsCuChamberCuZoneCu;
  TH2D* h2CeVsCuChamberCeZoneCe;
  TH2D* h2CeVsCuBxCuZoneCuOccupancy;
  TH2D* h2CeVsCuBxCeZoneCeOccupancy;
  TH2D* h2CeVsCuBxCuZoneCu;
  TH2D* h2CeVsCuBxCeZoneCe;


  TH1D* h1CeVsEuMatches;

  TH1D* h1CeVsEuMatches_ch;
  TH2D* h2CeVsEuChamberCeChamberEu_test;
  TH2D* h2CeVsEuChamberCeChamberEu;
  TH2D* h2CeVsEuPhiCePhiEu;
  TH1D* h2CeVsEuPhiCePhiEuDiff;
  TH2D* h2CeVsEuThetaCeThetaEu;
  TH2D* h2CeVsEuBxCeBxEu;

  TH1D* h1CeVsEuMatches_bx;
  TH2D* h2CeVsEuPhiCePhiEu_bx;
  TH2D* h2CeVsEuThetaCeThetaEu_bx;
  TH2D* h2CeVsEuBxCeBxEu_bx;

  TH1D* h1CeVsEuMatches_int;
  TH2D* h2CeVsEuPhiCePhiEu_int;
  TH2D* h2CeVsEuThetaCeThetaEu_int;
  TH2D* h2CeVsEuChamberEuZoneEu;
  TH2D* h2CeVsEuChamberCeZoneCe;


  TH2D* h2CeVsEuBxEuZoneEuOccupancy;
  TH2D* h2CeVsEuBxCeZoneCeOccupancy;
  TH2D* h2CeVsEuBxEuZoneEu;
  TH2D* h2CeVsEuBxCeZoneCe;

  TH1D* h1CeVsEeMatches;

  TH1D* h1CeVsEeMatches_ch;
  TH2D* h2CeVsEeChamberCeChamberEe_test;
  TH2D* h2CeVsEeChamberCeChamberEe;
  TH2D* h2CeVsEePhiCePhiEe;
  TH1D* h2CeVsEePhiCePhiEeDiff;
  TH2D* h2CeVsEeThetaCeThetaEe;
  TH2D* h2CeVsEeBxCeBxEe;

  TH1D* h1CeVsEeMatches_bx;
  TH2D* h2CeVsEePhiCePhiEe_bx;
  TH2D* h2CeVsEeThetaCeThetaEe_bx;
  TH2D* h2CeVsEeBxCeBxEe_bx;

  TH1D* h1CeVsEeMatches_int;
  TH2D* h2CeVsEePhiCePhiEe_int;
  TH2D* h2CeVsEeThetaCeThetaEe_int;
  TH2D* h2CeVsEeChamberEeZoneEe;
  TH2D* h2CeVsEeChamberCeZoneCe;


  TH2D* h2CeVsEeBxEeZoneEeOccupancy;
  TH2D* h2CeVsEeBxCeZoneCeOccupancy;
  TH2D* h2CeVsEeBxEeZoneEe;
  TH2D* h2CeVsEeBxCeZoneCe;


  TH2D* h2CuVsEuChamberCuChamberEu_test;
  TH2D* h2CuVsEuChamberCuChamberEu;
  TH2D* h2CuVsEuChamberCuChamberEuBox;
  TH2D* h2CuVsEuPhiCuPhiEu;
  TH1D* h2CuVsEuPhiCuPhiEuDiff;
  TH2D* h2CuVsEuThetaCuThetaEu;
  TH2D* h2CuVsEuBxCuBxEu;

  TH2D* h2CuVsEuPhiCuPhiEu_bx;
  TH2D* h2CuVsEuThetaCuThetaEu_bx;
  TH2D* h2CuVsEuBxCuBxEu_bx;

  TH2D* h2CuVsEuPhiCuPhiEu_int;
  TH2D* h2CuVsEuThetaCuThetaEu_int;
  TH2D* h2CuVsEuChamberCuZoneCu;
  TH2D* h2CuVsEuChamberCuZoneCuBox;
  TH2D* h2CuVsEuChamberEuZoneEu;
  TH2D* h2CuVsEuChamberEuZoneEuBox;
  TH2D* h2CuVsEuBxCuZoneCuOccupancy;
  TH2D* h2CuVsEuBxEuZoneEuOccupancy;
  TH2D* h2CuVsEuBxCuZoneCu;
  TH2D* h2CuVsEuBxEuZoneEu;


  TH2D* h2EeVsEuChamberEeChamberEu_test;
  TH2D* h2EeVsEuChamberEeChamberEu;
  TH2D* h2EeVsEuPhiEePhiEu;
  TH1D* h2EeVsEuPhiEePhiEuDiff;
  TH2D* h2EeVsEuThetaEeThetaEu;
  TH2D* h2EeVsEuBxEeBxEu;

  TH2D* h2EeVsEuPhiEePhiEu_bx;
  TH2D* h2EeVsEuThetaEeThetaEu_bx;
  TH2D* h2EeVsEuBxEeBxEu_bx;

  TH2D* h2EeVsEuPhiEePhiEu_int;
  TH2D* h2EeVsEuThetaEeThetaEu_int;
  TH2D* h2EeVsEuChamberEeZoneEe;
  TH2D* h2EeVsEuChamberEuZoneEu;
  TH2D* h2EeVsEuBxEeZoneEeOccupancy;
  TH2D* h2EeVsEuBxEuZoneEuOccupancy;
  TH2D* h2EeVsEuBxEeZoneEe;
  TH2D* h2EeVsEuBxEuZoneEu;
};


#endif


