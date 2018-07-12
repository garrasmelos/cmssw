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


class DQM_CPPF : public edm::EDAnalyzer {
  
 public:
  explicit DQM_CPPF(const edm::ParameterSet&);
  ~DQM_CPPF() override;
  void beginRun(const edm::Run&, const edm::EventSetup&) override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  int occupancy_value(int region_, int station_, int ring_);
  int bx_value(int region_, int emtfsector_);

  edm::ESHandle <RPCGeometry> rpcGeom;
  
 private:
  
  void beginJob() override;
  const edm::EDGetTokenT<l1t::CPPFDigiCollection> cppfDigiToken1_;
  const edm::EDGetTokenT<l1t::CPPFDigiCollection> cppfDigiToken2_;
  const edm::EDGetTokenT<l1t::CPPFDigiCollection> cppfDigiToken3_;

  int EMTF_sector;
  int EMTF_subsector;
  int EMTF_subsector_unpacker;
  int EMTF_subsector_EMTF;
  int EMTF_bx;

  std::vector<int> EMTFsector0bins;
  std::vector<int> EMTFsector1bins;
  std::vector<int> EMTFsector2bins;
  std::vector<int> EMTFsector3bins;
  std::vector<int> EMTFsector4bins;
  std::vector<int> EMTFsector5bins;
  std::vector<int> EMTFsector6bins;
  
  std::map<int , std::vector<int>> fill_info;
  
  TH1D* Matches_unpacker;

  TH1D* Matches_unpacker_ch;
  TH2D* chamber_emu_unpacker;
  TH2D* phi_emu_unpacker;
  TH2D* theta_emu_unpacker;
  TH2D* bx_emu_unpacker;

  TH1D* Matches_unpacker_bx;
  TH2D* phi_emu_unpacker_bx;
  TH2D* theta_emu_unpacker_bx;
  TH2D* bx_emu_unpacker_bx;

  TH1D* Matches_unpacker_int;
  TH2D* phi_emu_unpacker_int;
  TH2D* theta_emu_unpacker_int;
  TH2D* occupancy_unpacker;
  TH2D* occupancy_emu_unpacker;

  TH2D* bx_occupancy_unpacker;
  TH2D* bx_occupancy_emu_unpacker;
  TH2D* bx_sector_unpacker; 
  TH2D* bx_sector_emu_unpacker;


  TH1D* Matches_emtf;

  TH1D* Matches_emtf_ch;
  TH2D* chamber_emu_emtf_test;
  TH2D* chamber_emu_emtf;
  TH2D* phi_emu_emtf;
  TH2D* theta_emu_emtf;
  TH2D* bx_emu_emtf;

  TH1D* Matches_emtf_bx;
  TH2D* phi_emu_emtf_bx;
  TH2D* theta_emu_emtf_bx;
  TH2D* bx_emu_emtf_bx;

  
  TH1D* Matches_emtf_int;
  TH2D* phi_emu_emtf_int;
  TH2D* theta_emu_emtf_int;
  TH2D* occupancy_emtf;
  TH2D* occupancy_emu_emtf;

  TH2D* bx_occupancy_emtf;
  TH2D* bx_occupancy_emu_emtf;
  TH2D* bx_sector_emtf; 
  TH2D* bx_sector_emu_emtf;

  TH2D* chamber_unpacker_emtf_test;
  TH2D* chamber_unpacker_emtf;
  TH2D* phi_unpacker_emtf;
  TH2D* theta_unpacker_emtf;
  TH2D* bx_unpacker_emtf;

  TH2D* phi_unpacker_emtf_bx;
  TH2D* theta_unpacker_emtf_bx;
  TH2D* bx_unpacker_emtf_bx;

  
  TH2D* phi_unpacker_emtf_int;
  TH2D* theta_unpacker_emtf_int;
  TH2D* occupancy_unpacker_emtf_u;
  TH2D* occupancy_unpacker_emtf_e;

  TH2D* bx_occupancy_unpacker_emtf_u;
  TH2D* bx_occupancy_unpacker_emtf_e;
  TH2D* bx_sector_unpacker_emtf_u;
  TH2D* bx_sector_unpacker_emtf_e;
 
  TH2D* bx;
  TH2D* bx_occupancy;

};


#endif


