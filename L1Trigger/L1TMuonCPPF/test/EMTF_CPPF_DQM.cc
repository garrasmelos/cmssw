#include "EMTF_CPPF_DQM.h"
///////////////////////////////////////////////
//  u: unpacker
//  e: emulator
//  E: EMTF 
//  C: CPPF
///////////////////////////////////////////////  

EMTF_CPPF_DQM::EMTF_CPPF_DQM(const edm::ParameterSet& iConfig) :
  cppfDigiToken1_(consumes<l1t::CPPFDigiCollection>(iConfig.getParameter<edm::InputTag>("cppfdigiLabel1"))),
  cppfDigiToken2_(consumes<l1t::CPPFDigiCollection>(iConfig.getParameter<edm::InputTag>("cppfdigiLabel2"))),
  cppfDigiToken3_(consumes<l1t::CPPFDigiCollection>(iConfig.getParameter<edm::InputTag>("cppfdigiLabel3"))),
  cppfDigiToken4_(consumes<l1t::CPPFDigiCollection>(iConfig.getParameter<edm::InputTag>("cppfdigiLabel4"))) {
}

EMTF_CPPF_DQM::~EMTF_CPPF_DQM(){
}

void EMTF_CPPF_DQM::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){  
  
  //Get the CPPFDigi 
  edm::Handle<l1t::CPPFDigiCollection> CppfDigis1;
  edm::Handle<l1t::CPPFDigiCollection> CppfDigis2;
  edm::Handle<l1t::CPPFDigiCollection> CppfDigis3;
  edm::Handle<l1t::CPPFDigiCollection> CppfDigis4;
  iEvent.getByToken(cppfDigiToken1_, CppfDigis1);
  iEvent.getByToken(cppfDigiToken2_, CppfDigis2);
  iEvent.getByToken(cppfDigiToken3_, CppfDigis3);
  iEvent.getByToken(cppfDigiToken4_, CppfDigis4);
  
  //std::cout << "" << std::endl;
  
//  int matchesCe = 0;
//  int matchesCe_bx = 0;
//  int matchesCe_int = 0;
//  int matchesEe = 0;
//  int matchesEe_bx = 0;
//  int matchesEe_int = 0;
  
///////////////////////////////////////////////////////////
// Comparison CPPF emulator (Ce) vs CPPF unpacked (Cu), 
// EMTF unpacked (Eu) and EMTF emulator (Ee) 
///////////////////////////////////////////////////////////
  
  for(auto& cppf_digis : *CppfDigis1){
    
    RPCDetId rpcIdCe = cppf_digis.rpcId();
    int regionCe = rpcIdCe.region();
    int stationCe = rpcIdCe.station();
    int ringCe = rpcIdCe.ring();
    int sectorCe = rpcIdCe.sector();
    int subsectorCe = rpcIdCe.subsector();
    int phiIntCe = cppf_digis.phi_int();
    int thetaIntCe =  cppf_digis.theta_int();
    
    int emtfSectorCe = cppf_digis.emtf_sector();
    int emtfSubsectorCe = GetSubsector(emtfSectorCe, subsectorCe);
    
    //Displace by 0.5 to center it to the bin.
    int bxCe = cppf_digis.bx();
    //if(bxCe >= 0) bxCe = bxCe+0.5;  
    //else if(bxCe < 0) bxCe = bxCe-0.5;  
    
    int fillOccupancyCe = occupancy_value(regionCe, stationCe, ringCe);
    int fillBxCe = bx_value(regionCe, emtfSectorCe); 
    
    //Chamber ID
    int nsubCe = 6;
    (ringCe == 1 && stationCe > 1) ? nsubCe = 3 : nsubCe = 6;
    int chamberIDCe = subsectorCe + nsubCe * ( sectorCe - 1);
    
    
    int matchesCu = 0;  
    int matchesCu_bx = 0;
    int matchesCu_int = 0;
    int phiIntCu = 0;
    for(auto& cppf_digis_unpacker : *CppfDigis2){
      RPCDetId rpcIdCu = cppf_digis_unpacker.rpcId();
      int regionCu = rpcIdCu.region();
      int stationCu = rpcIdCu.station();
      int ringCu = rpcIdCu.ring();
      int sectorCu = rpcIdCu.sector();
      int subsectorCu = rpcIdCu.subsector();
      phiIntCu = cppf_digis_unpacker.phi_int();
      int thetaIntCu =  cppf_digis_unpacker.theta_int();
      int bxCu = cppf_digis_unpacker.bx();
      
      int emtfSectorCu = cppf_digis_unpacker.emtf_sector();
      int emtfSubsectorCu = GetSubsector(sectorCu, subsectorCu);

      //Chamber ID
      int nsubCu = 6;
      (ringCu == 1 && stationCu > 1) ? nsubCu = 3 : nsubCu = 6;
      int chamberIDCu = subsectorCu + nsubCu * ( sectorCu - 1);
      
      int fillOccupancyCu = occupancy_value(regionCu, stationCu, ringCu);
      int fillBxCu = bx_value(regionCu, emtfSectorCu);
      
      if((regionCe == regionCu) && 
	        (stationCe == stationCu) &&
	        (ringCe == ringCu) &&
	        (sectorCe == sectorCu) &&
	        (subsectorCe == subsectorCu)){
	
	      matchesCu++;
        h1CeVsCuMatches_ch->Fill(matchesCu);
	
        h2CeVsCuChamberCeChamberCu->Fill(chamberIDCe, chamberIDCu); 
	
        h2CeVsCuPhiCePhiCu->Fill(phiIntCe, phiIntCu);
        h2CeVsCuPhiCePhiCuDiff->Fill(phiIntCe - phiIntCu);
        h2CeVsCuThetaCeThetaCu->Fill(thetaIntCe, thetaIntCu);
        h2CeVsCuBxCeBxCu->Fill(bxCe, bxCu);
	
        if(bxCe ==  bxCu){
	        matchesCu_bx++;
	        h1CeVsCuMatches_bx->Fill(matchesCu_bx);	
	        h2CeVsCuPhiCePhiCu_bx->Fill(phiIntCe, phiIntCu);
	        h2CeVsCuThetaCeThetaCu_bx->Fill(thetaIntCe, thetaIntCu); 
	        h2CeVsCuBxCeBxCu_bx->Fill(bxCe, bxCu);
	  
	        if((phiIntCe == phiIntCu) && (thetaIntCe == thetaIntCu)){
	          matchesCu_int++;
	          h1CeVsCuMatches_int->Fill(matchesCu_int);	
	          h2CeVsCuPhiCePhiCu_int->Fill(phiIntCe, phiIntCu);
	          h2CeVsCuThetaCeThetaCu_int->Fill(thetaIntCe, thetaIntCu); 
            //Occupancy 
	          h2CeVsCuChamberCuZoneCu->Fill(emtfSubsectorCu, fillOccupancyCu);
	          h2CeVsCuChamberCeZoneCe->Fill(emtfSubsectorCe, fillOccupancyCe);
            //bx occupancy 
            h2CeVsCuBxCuZoneCuOccupancy->Fill(bxCu, fillOccupancyCu);
            h2CeVsCuBxCeZoneCeOccupancy->Fill(bxCe, fillOccupancyCe);
            //bx
            h2CeVsCuBxCuZoneCu->Fill(fillBxCu, bxCu);
            h2CeVsCuBxCeZoneCe->Fill(fillBxCe, bxCe);
	        } 
	      }
      }
    }
    //Matching information
    h1CeVsCuMatches->Fill(matchesCu);
	  //if (matchesCu == 1) h2CeVsCuPhiCePhiCuDiff->Fill(phiIntCe - phiIntCu);
    int matchesEu = 0;
    int matchesEu_bx = 0; 
    int matchesEu_int = 0; 
    
    for(auto& cppf_digis_emtf : *CppfDigis3){
      
      RPCDetId rpcIdEu = cppf_digis_emtf.rpcId();
      int regionEu = rpcIdEu.region();
      int stationEu = rpcIdEu.station();
      int ringEu = rpcIdEu.ring();
      int sectorEu = rpcIdEu.sector();
      int subsectorEu = rpcIdEu.subsector();
      int phiIntEu = cppf_digis_emtf.phi_int();
      int thetaIntEu =  cppf_digis_emtf.theta_int();
      int bxEu = cppf_digis_emtf.bx();
      
      //Chamber ID
      int nsubEu = 6;
      (ringEu == 1 && stationEu > 1) ? nsubEu = 3 : nsubEu = 6;
      int chamberIDEu = subsectorEu + nsubEu * ( sectorEu - 1);
      
      int emtfSectorEu = cppf_digis_emtf.emtf_sector();
      int emtfSubsectorEu = GetSubsector(sectorEu,subsectorEu);
      
      if((phiIntCe == phiIntEu) && (thetaIntCe == thetaIntEu) && (regionCe == regionEu)){
	      h2CeVsEuChamberCeChamberEu_test->Fill(chamberIDCe, chamberIDEu);
      }
      int fillOccupancyEu = occupancy_value(regionEu, stationEu, ringEu);
      int fillBxEu = bx_value(regionEu, emtfSectorEu);

      //G: if((chamberIDEu == 35) || (chamberIDEu == 36)) chamberIDEu -= 34;
      //G: else chamberIDEu += 2;

      if((chamberIDCe == chamberIDEu) && (regionCe == regionEu) 
         && (ringCe == ringEu) && (stationCe == stationEu) && (regionCe == regionEu) 
          ){
	      matchesEu++;
        h1CeVsEuMatches_ch->Fill(matchesEu);
	
        h2CeVsEuChamberCeChamberEu->Fill(chamberIDCe, chamberIDEu); 
	
        h2CeVsEuPhiCePhiEu->Fill(phiIntCe, phiIntEu);
        h2CeVsEuPhiCePhiEuDiff->Fill(phiIntCe - phiIntEu);
        h2CeVsEuThetaCeThetaEu->Fill(thetaIntCe, thetaIntEu);
        h2CeVsEuBxCeBxEu->Fill(bxCe, bxEu);
	
        if(bxCe ==  bxEu){
	        matchesEu_bx++;
	        h1CeVsEuMatches_bx->Fill(matchesEu_bx);	    
	        h2CeVsEuPhiCePhiEu_bx->Fill(phiIntCe, phiIntEu);
	        h2CeVsEuThetaCeThetaEu_bx->Fill(thetaIntCe, thetaIntEu); 
	        h2CeVsEuBxCeBxEu_bx->Fill(bxCe, bxEu);
	        if((phiIntCe == phiIntEu) && (thetaIntCe == thetaIntEu)){
	          matchesEu_int++;
	          h1CeVsEuMatches_int->Fill(matchesEu_int);
	          h2CeVsEuPhiCePhiEu_int->Fill(phiIntCe, phiIntCe);
	          h2CeVsEuThetaCeThetaEu_int->Fill(thetaIntCe, thetaIntEu);
 
            //Occupancy
	          h2CeVsEuChamberEuZoneEu->Fill(emtfSubsectorEu, fillOccupancyEu);
            //Setting up the subsector
            //if((EMTF_subsector == 1) || (EMTF_subsector == 2)) EMTF_subsector += 34;
            //else EMTF_subsector -= 2; 		 
            //G: if((emtfSubsectorEu== 35) || (emtfSubsectorEu == 36)) emtfSubsectorEu -= 34;
            //G: else emtfSubsectorEu += 2; 		 
	          h2CeVsEuChamberCeZoneCe->Fill(emtfSubsectorCe, fillOccupancyCe); 
            //bx occupancy 
            h2CeVsEuBxEuZoneEuOccupancy->Fill(bxEu, fillOccupancyEu);
            h2CeVsEuBxCeZoneCeOccupancy->Fill(bxCe, fillOccupancyCe);
            //bx
            h2CeVsEuBxEuZoneEu->Fill(fillBxEu,bxEu);
            h2CeVsEuBxCeZoneCe->Fill(fillBxCe,bxCe);
	        }	    
        }
      }
    }
    //Matching information
    h1CeVsEuMatches->Fill(matchesEu);
    int matchesEe = 0;
    int matchesEe_bx = 0; 
    int matchesEe_int = 0; 
    
    for(auto& cppf_digis_emtf : *CppfDigis4){
      
      RPCDetId rpcIdEe = cppf_digis_emtf.rpcId();
      int regionEe = rpcIdEe.region();
      int stationEe = rpcIdEe.station();
      int ringEe = rpcIdEe.ring();
      int sectorEe = rpcIdEe.sector();
      int subsectorEe = rpcIdEe.subsector();
      int phiIntEe = cppf_digis_emtf.phi_int();
      int thetaIntEe =  cppf_digis_emtf.theta_int();
      int bxEe = cppf_digis_emtf.bx();
      
      //Chamber ID
      int nsubEe = 6;
      (ringEe == 1 && stationEe > 1) ? nsubEe = 3 : nsubEe = 6;
      int chamberIDEe = subsectorEe + nsubEe * ( sectorEe - 1);
      
      int emtfSectorEe = cppf_digis_emtf.emtf_sector();
      int emtfSubsectorEe = GetSubsector(sectorEe,subsectorEe);
      
      if((phiIntCe == phiIntEe) && (thetaIntCe == thetaIntEe) && (regionCe == regionEe)){
	      h2CeVsEeChamberCeChamberEe_test->Fill(chamberIDCe, chamberIDEe);
      }
      int fillOccupancyEe = occupancy_value(regionEe, stationEe, ringEe);
      int fillBxEe = bx_value(regionEe, emtfSectorEe);

      //if((chamberIDEe == 35) || (chamberIDEe == 36)) chamberIDEe -= 34;
      //else chamberIDEe += 2;

      if((chamberIDCe == chamberIDEe) && (regionCe == regionEe) 
         && (ringCe == ringEe) && (stationCe == stationEe) && (regionCe == regionEe) 
          ){
	      matchesEe++;
        h1CeVsEeMatches_ch->Fill(matchesEe);
	
        h2CeVsEeChamberCeChamberEe->Fill(chamberIDCe, chamberIDEe); 
	
        h2CeVsEePhiCePhiEe->Fill(phiIntCe, phiIntEe);
        h2CeVsEePhiCePhiEeDiff->Fill(phiIntCe - phiIntEe);
        h2CeVsEeThetaCeThetaEe->Fill(thetaIntCe, thetaIntEe);
        h2CeVsEeBxCeBxEe->Fill(bxCe, bxEe);
	
        if(bxCe ==  bxEe){
	        matchesEe_bx++;
	        h1CeVsEeMatches_bx->Fill(matchesEe_bx);	    
	        h2CeVsEePhiCePhiEe_bx->Fill(phiIntCe, phiIntEe);
	        h2CeVsEeThetaCeThetaEe_bx->Fill(thetaIntCe, thetaIntEe); 
	        h2CeVsEeBxCeBxEe_bx->Fill(bxCe, bxEe);
	        if((phiIntCe == phiIntEe) && (thetaIntCe == thetaIntEe)){
	          matchesEe_int++;
	          h1CeVsEeMatches_int->Fill(matchesEe_int);
	          h2CeVsEePhiCePhiEe_int->Fill(phiIntCe, phiIntCe);
	          h2CeVsEeThetaCeThetaEe_int->Fill(thetaIntCe, thetaIntEe);
 
            //Occupancy
	          h2CeVsEeChamberEeZoneEe->Fill(emtfSubsectorEe, fillOccupancyEe);
            //Setting up the subsector
            //if((EMTF_subsector == 1) || (EMTF_subsector == 2)) EMTF_subsector += 34;
            //else EMTF_subsector -= 2; 		 
            //G: if((emtfSubsectorEe== 35) || (emtfSubsectorEe == 36)) emtfSubsectorEe -= 34;
            //G: else emtfSubsectorEe += 2; 		 
	          h2CeVsEeChamberCeZoneCe->Fill(emtfSubsectorCe, fillOccupancyCe); 
            //bx occupancy 
            h2CeVsEeBxEeZoneEeOccupancy->Fill(bxEe, fillOccupancyEe);
            h2CeVsEeBxCeZoneCeOccupancy->Fill(bxCe, fillOccupancyCe);
            //bx
            h2CeVsEeBxEeZoneEe->Fill(fillBxEe,bxEe);
            h2CeVsEeBxCeZoneCe->Fill(fillBxCe,bxCe);
	        }	    
        }
      }
    }
    //Matching information
    h1CeVsEeMatches->Fill(matchesEe);
  } // loop over CPPFDigis
  
  //Comparison between CPPF and EMTF
    
  for(auto& cppf_digis_unpacker : *CppfDigis2){
    RPCDetId rpcIdCu = cppf_digis_unpacker.rpcId();
    int regionCu = rpcIdCu.region();
    int stationCu = rpcIdCu.station();
    int ringCu = rpcIdCu.ring();
    int sectorCu = rpcIdCu.sector();
    int subsectorCu = rpcIdCu.subsector();
    int phiIntCu = cppf_digis_unpacker.phi_int();
    int thetaIntCu =  cppf_digis_unpacker.theta_int();
    int bxCu = cppf_digis_unpacker.bx();
    
    //std::cout << "Subsector CPPF unpacker" << subsector_unpacker << std::endl;
    int emtfSectorCu = cppf_digis_unpacker.emtf_sector();
    int emtfSubsectorCu = GetSubsector(sectorCu, subsectorCu);

    //Chamber ID
    int nsubCu = 6;
    (ringCu == 1 && stationCu > 1) ? nsubCu = 3 : nsubCu = 6;
    int chamberIDCu = subsectorCu + nsubCu * ( sectorCu - 1);
    
    int fillOccupancyCu = occupancy_value(regionCu, stationCu, ringCu);
    int fillBxCu = bx_value(regionCu, emtfSectorCu);
    
    for(auto& cppf_digis_emtf : *CppfDigis3){
      
      RPCDetId rpcIdEu = cppf_digis_emtf.rpcId();
      int regionEu = rpcIdEu.region();
      int stationEu = rpcIdEu.station();
      int ringEu = rpcIdEu.ring();
      int sectorEu = rpcIdEu.sector();
      int subsectorEu = rpcIdEu.subsector();
      int phiIntEu = cppf_digis_emtf.phi_int();
      int thetaIntEu =  cppf_digis_emtf.theta_int();
      int bxEu = cppf_digis_emtf.bx();
      
      //Chamber ID
      int nsubEu = 6;
      (ringEu == 1 && stationEu > 1) ? nsubEu = 3 : nsubEu = 6;
      int chamberIDEu = subsectorEu + nsubEu * ( sectorEu - 1);
      
      if((phiIntCu == phiIntEu) && (thetaIntCu == thetaIntCu) && (regionCu == regionEu)){
	      h2CuVsEuChamberCuChamberEu_test->Fill(chamberIDCu, chamberIDEu);
      }
      int fillOccupancyEu = occupancy_value(regionEu, stationEu, ringEu);
      int emtfSectorEu = cppf_digis_emtf.emtf_sector();
      int emtfSubsectorEu = GetSubsector(sectorEu, subsectorEu);

      int fillBxEu = bx_value(regionEu, emtfSectorEu);

      //G: if((chamberIDEu == 35) || (chamberIDEu == 36)) chamberIDEu -= 34;
      //G: else chamberIDEu += 2;

      if((chamberIDCu == chamberIDEu)  
         && (ringCu == ringEu) && (stationCu == stationEu) && (regionCu == regionCu) 
          ){
	
        h2CuVsEuChamberCuChamberEu->Fill(chamberIDCu, chamberIDEu); 
        
        if (abs(phiIntCu - phiIntEu) > 500.)  {
          h2CuVsEuChamberCuChamberEuBox->Fill(chamberIDCu, chamberIDEu); 
	        h2CuVsEuChamberCuZoneCuBox->Fill(emtfSubsectorCu, fillOccupancyCu); 
	        h2CuVsEuChamberEuZoneEuBox->Fill(emtfSubsectorEu, fillOccupancyEu); 
        }
	
        h2CuVsEuPhiCuPhiEu->Fill(phiIntCu, phiIntEu);
        h2CuVsEuPhiCuPhiEuDiff->Fill(phiIntCu - phiIntEu);
        h2CuVsEuThetaCuThetaEu->Fill(thetaIntCu, thetaIntEu);
        h2CuVsEuBxCuBxEu->Fill(bxCu,bxEu);
	
        if(bxEu ==  bxEu){
	        h2CuVsEuPhiCuPhiEu_bx->Fill(phiIntCu, phiIntEu);
	        h2CuVsEuThetaCuThetaEu_bx->Fill(thetaIntCu, thetaIntEu); 
	        h2CuVsEuBxCuBxEu_bx->Fill(bxCu,bxEu);
	        if((phiIntCu == phiIntEu) && (thetaIntCu == thetaIntEu)){
	          h2CuVsEuPhiCuPhiEu_int->Fill(phiIntCu, phiIntEu);
	          h2CuVsEuThetaCuThetaEu_int->Fill(thetaIntCu, thetaIntEu);
 
            //Setting up the subsector
            //if((EMTF_subsector_u == 1) || (EMTF_subsector_u == 2)) EMTF_subsector_u += 34;
            //else EMTF_subsector_u -= 2; 		 
            //G: if((emtfSubsectorEu== 35) || (emtfSubsectorEu== 36)) emtfSubsectorEu-= 34;
            //G: else emtfSubsectorEu+=2; 		 
	          h2CuVsEuChamberCuZoneCu->Fill(emtfSubsectorCu, fillOccupancyCu); 
	          h2CuVsEuChamberEuZoneEu->Fill(emtfSubsectorEu, fillOccupancyEu); 
            //bx occupancy 
            h2CuVsEuBxCuZoneCuOccupancy->Fill(bxCu, fillOccupancyEu);
            h2CuVsEuBxEuZoneEuOccupancy->Fill(bxEu, fillOccupancyEu);
            //bx
            h2CuVsEuBxCuZoneCu->Fill(fillBxCu,bxCu);
            h2CuVsEuBxEuZoneEu->Fill(fillBxEu,bxEu);
	        }	    
        }
      }
    }
  } // loop over CPPFDigis





  for(auto& cppf_digis_unpacker : *CppfDigis4){
    RPCDetId rpcIdEe = cppf_digis_unpacker.rpcId();
    int regionEe = rpcIdEe.region();
    int stationEe = rpcIdEe.station();
    int ringEe = rpcIdEe.ring();
    int sectorEe = rpcIdEe.sector();
    int subsectorEe = rpcIdEe.subsector();
    int phiIntEe = cppf_digis_unpacker.phi_int();
    int thetaIntEe =  cppf_digis_unpacker.theta_int();
    int bxEe = cppf_digis_unpacker.bx();
    
    //std::cout << "Subsector CPPF unpacker" << subsector_unpacker << std::endl;
    int emtfSectorEe = cppf_digis_unpacker.emtf_sector();
    int emtfSubsectorEe = GetSubsector(sectorEe, subsectorEe);

    //Chamber ID
    int nsubEe = 6;
    (ringEe == 1 && stationEe > 1) ? nsubEe = 3 : nsubEe = 6;
    int chamberIDEe = subsectorEe + nsubEe * ( sectorEe - 1);
    
    int fillOccupancyEe = occupancy_value(regionEe, stationEe, ringEe);
    int fillBxEe = bx_value(regionEe, emtfSectorEe);
    
    for(auto& cppf_digis_emtf : *CppfDigis3){
      
      RPCDetId rpcIdEu = cppf_digis_emtf.rpcId();
      int regionEu = rpcIdEu.region();
      int stationEu = rpcIdEu.station();
      int ringEu = rpcIdEu.ring();
      int sectorEu = rpcIdEu.sector();
      int subsectorEu = rpcIdEu.subsector();
      int phiIntEu = cppf_digis_emtf.phi_int();
      int thetaIntEu =  cppf_digis_emtf.theta_int();
      int bxEu = cppf_digis_emtf.bx();
      
      //Chamber ID
      int nsubEu = 6;
      (ringEu == 1 && stationEu > 1) ? nsubEu = 3 : nsubEu = 6;
      int chamberIDEu = subsectorEu + nsubEu * ( sectorEu - 1);
      
      if((phiIntEe == phiIntEu) && (thetaIntEe == thetaIntEe) && (regionEe == regionEu)){
	      h2EeVsEuChamberEeChamberEu_test->Fill(chamberIDEe, chamberIDEu);
      }
      int fillOccupancyEu = occupancy_value(regionEu, stationEu, ringEu);
      int emtfSectorEu = cppf_digis_emtf.emtf_sector();
      int emtfSubsectorEu = GetSubsector(sectorEu, subsectorEu);

      int fillBxEu = bx_value(regionEu, emtfSectorEu);

      //G: if((chamberIDEu == 35) || (chamberIDEu == 36)) chamberIDEu -= 34;
      //G: else chamberIDEu += 2;

      if((chamberIDEe == chamberIDEu)  
         && (ringEe == ringEu) && (stationEe == stationEu) && (regionEe == regionEe) 
          ){
	
        h2EeVsEuChamberEeChamberEu->Fill(chamberIDEe, chamberIDEu); 
	
        h2EeVsEuPhiEePhiEu->Fill(phiIntEe, phiIntEu);
        h2EeVsEuPhiEePhiEuDiff->Fill(phiIntEe - phiIntEu);
        h2EeVsEuThetaEeThetaEu->Fill(thetaIntEe, thetaIntEu);
        h2EeVsEuBxEeBxEu->Fill(bxEe,bxEu);
	
        if(bxEu ==  bxEu){
	        h2EeVsEuPhiEePhiEu_bx->Fill(phiIntEe, phiIntEu);
	        h2EeVsEuThetaEeThetaEu_bx->Fill(thetaIntEe, thetaIntEu); 
	        h2EeVsEuBxEeBxEu_bx->Fill(bxEe,bxEu);
	        if((phiIntEe == phiIntEu) && (thetaIntEe == thetaIntEu)){
	          h2EeVsEuPhiEePhiEu_int->Fill(phiIntEe, phiIntEu);
	          h2EeVsEuThetaEeThetaEu_int->Fill(thetaIntEe, thetaIntEu);
 
            //Setting up the subsector
            //if((EMTF_subsector_u == 1) || (EMTF_subsector_u == 2)) EMTF_subsector_u += 34;
            //else EMTF_subsector_u -= 2; 		 
            //G: if((emtfSubsectorEu== 35) || (emtfSubsectorEu== 36)) emtfSubsectorEu-= 34;
            //G: else emtfSubsectorEu+=2; 		 
	          h2EeVsEuChamberEeZoneEe->Fill(emtfSubsectorEe, fillOccupancyEe); 
	          h2EeVsEuChamberEuZoneEu->Fill(emtfSubsectorEu, fillOccupancyEu); 
            //bx occupancy 
            h2EeVsEuBxEeZoneEeOccupancy->Fill(bxEe, fillOccupancyEu);
            h2EeVsEuBxEuZoneEuOccupancy->Fill(bxEu, fillOccupancyEu);
            //bx
            h2EeVsEuBxEeZoneEe->Fill(fillBxEe,bxEe);
            h2EeVsEuBxEuZoneEu->Fill(fillBxEu,bxEu);
	        }	    
        }
      }
    }
  } // loop over CPPFDigis
} //End class

int EMTF_CPPF_DQM::occupancy_value(int region_, int station_, int ring_){
  int fill_val = 0;

  if(region_ == -1){
    if((station_ == 4) && (ring_ == 3)) fill_val = 1;
    else if ((station_ == 4) && (ring_ == 2)) fill_val = 2;
    else if ((station_ == 3) && (ring_ == 3)) fill_val = 3;
    else if ((station_ == 3) && (ring_ == 2)) fill_val = 4;
    else if ((station_ == 2) && (ring_ == 2)) fill_val = 5;
    else if ((station_ == 1) && (ring_ == 2)) fill_val = 6;
  }
  else if(region_ == +1){
    if((station_ == 1) && (ring_ == 2)) fill_val = 7; 
    else if((station_ == 2) && (ring_ == 2)) fill_val = 8;
    else if((station_ == 3) && (ring_ == 2)) fill_val = 9;
    else if((station_ == 3) && (ring_ == 3)) fill_val = 10;
    else if((station_ == 4) && (ring_ == 2)) fill_val = 11;
    else if((station_ == 4) && (ring_ == 3)) fill_val = 12;
  }
  return fill_val;
}

int EMTF_CPPF_DQM::GetSubsector(int emtfsector_, int lsubsector_){
  const int nsectors = 6;
  int gsubsector = 0;
  if ((emtfsector_ != -99) and (lsubsector_ != 0)){
    gsubsector = (emtfsector_ - 1)*nsectors + lsubsector_;
  }
  return gsubsector;
}

int EMTF_CPPF_DQM::bx_value(int region_, int emtfsector_){
  int fill_val = 0;

  if(region_ == -1){
    if(emtfsector_ == 1) fill_val = 6;
    else if(emtfsector_ == 2) fill_val = 5;
    else if(emtfsector_ == 3) fill_val = 4;
    else if(emtfsector_ == 4) fill_val = 3;
    else if(emtfsector_ == 5) fill_val = 2;
    else if(emtfsector_ == 6) fill_val = 1;
  }
  else if(region_ == +1){
    if(emtfsector_ == 1) fill_val = 7;
    else if(emtfsector_ == 2) fill_val = 8;
    else if(emtfsector_ == 3) fill_val = 9;
    else if(emtfsector_ == 4) fill_val = 10;
    else if(emtfsector_ == 5) fill_val = 11;
    else if(emtfsector_ == 6) fill_val = 12;
  }
  return fill_val;
}


void EMTF_CPPF_DQM::beginRun(const edm::Run& run, const edm::EventSetup& iSetup){
  iSetup.get<MuonGeometryRecord>().get(rpcGeom);
}

void EMTF_CPPF_DQM::beginJob(){
  edm::Service<TFileService> fs;
  
  h1CeVsCuMatches = fs->make<TH1D>("h1CeVsCuMatches", "CPPFDigis_h1CeVsCuMatches" , 5, 0. , 5.);
  
  h1CeVsCuMatches_ch = fs->make<TH1D>("h1CeVsCuMatches_ch", "CPPFDigis_h1CeVsCuMatches_ch" , 5, 0. , 5.);
  h2CeVsCuChamberCeChamberCu = fs->make<TH2D>("h2CeVsCuChamberCeChamberCu","h2CeVsCuChamberCeChamberCu", 36, 1.,37.,36,1.,37.);
  h2CeVsCuPhiCePhiCu=fs->make<TH2D>("h2CeVsCuPhiCePhiCu", "h2CeVsCuPhiCePhiCu",  124, 0., 1240., 124, 0., 1240.);
  h2CeVsCuPhiCePhiCuDiff=fs->make<TH1D>("h2CeVsCuPhiCePhiCuDiff", "h2CeVsCuPhiCePhiCuDiff",  100, -100., 100.);
  h2CeVsCuThetaCeThetaCu=fs->make<TH2D>("h2CeVsCuThetaCeThetaCu", "h2CeVsCuThetaCeThetaCu",  32, 0., 32., 32, 0., 32.);
  h2CeVsCuBxCeBxCu = fs->make<TH2D>("h2CeVsCuBxCeBxCu","h2CeVsCuBxCeBxCu", 8, -3.5,4.5, 8, -3.5, 4.5);
  
  h1CeVsCuMatches_bx = fs->make<TH1D>("h1CeVsCuMatches_bx", "CPPFDigis_Matches_bx" , 5, 0. , 5.);
  h2CeVsCuPhiCePhiCu_bx=fs->make<TH2D>("h2CeVsCuPhiCePhiCu_bx", "h2CeVsCuPhiCePhiCu_bx",  124, 0., 1240., 124, 0., 1240.);
  h2CeVsCuThetaCeThetaCu_bx=fs->make<TH2D>("h2CeVsCuThetaCeThetaCu_bx", "h2CeVsCuThetaCeThetaCu_bx",  32, 0., 32., 32, 0., 32.);
  h2CeVsCuBxCeBxCu_bx = fs->make<TH2D>("h2CeVsCuBxCeBxCu_bx","h2CeVsCuBxCeBxCu_bx", 8, -3.5,4.5, 8, -3.5, 4.5);
  
  h1CeVsCuMatches_int = fs->make<TH1D>("h1CeVsCuMatches_int", "CPPFDigis_Matches_int" , 5, 0. , 5.);
  h2CeVsCuPhiCePhiCu_int=fs->make<TH2D>("h2CeVsCuPhiCePhiCu_int", "h2CeVsCuPhiCePhiCu_int",  124, 0., 1240., 124, 0., 1240.);
  h2CeVsCuThetaCeThetaCu_int=fs->make<TH2D>("h2CeVsCuThetaCeThetaCu_int", "h2CeVsCuThetaCeThetaCu_int",  32, 0., 32., 32, 0., 32.);
  h2CeVsCuChamberCuZoneCu = fs->make<TH2D>("h2CeVsCuChamberCuZoneCu", "CPPFDigis h2CeVsCuChamberCuZoneCu", 36, 0.5, 36.5, 12, 0.5,12.5); 
  h2CeVsCuChamberCeZoneCe = fs->make<TH2D>("h2CeVsCuChamberCeZoneCe", "CPPFDigis h2CeVsCuChamberCeZoneCe", 36, 0.5, 36.5, 12, 0.5,12.5); 
  h2CeVsCuBxCuZoneCuOccupancy = fs->make<TH2D>("h2CeVsCuBxCuZoneCuOccupancy","CPPFDigis Bx_Occupancy_unpacker", 8, -3.5, 4.5, 12, 0.5, 12.5);
  h2CeVsCuBxCeZoneCeOccupancy = fs->make<TH2D>("h2CeVsCuBxCeZoneCe","CPPFDigis Bx_Occupancy_emu_unpacker", 8, -3.5, 4.5, 12, 0.5, 12.5);
  h2CeVsCuBxCuZoneCu = fs->make<TH2D>("h2CeVsCuBxCuZoneCu","CPPFDigis Bx_unpacker", 12, 0.5, 12.5, 8,-3.5,4.5);
  h2CeVsCuBxCeZoneCe = fs->make<TH2D>("h2CeVsCuBxCeZoneCe","CPPFDigis Bx_emu_unpacker", 12, 0.5, 12.5, 8,-3.5,4.5);


  h1CeVsEuMatches = fs->make<TH1D>("h1CeVsEuMatches", "CPPFDigis_h1CeVsEuMatches" , 5, 0. , 5.);
  
  h1CeVsEuMatches_ch = fs->make<TH1D>("h1CeVsEuMatches_ch", "CPPFDigis_Matches_ch" , 5, 0. , 5.);
  h2CeVsEuChamberCeChamberEu_test = fs->make<TH2D>("h2CeVsEuChamberCeChamberEu_test","h2CeVsEuChamberCeChamberEu_test", 36, 1.,37.,36,1.,37.);
  h2CeVsEuChamberCeChamberEu = fs->make<TH2D>("h2CeVsEuChamberCeChamberEu","h2CeVsEuChamberCeChamberEu", 36, 1.,37.,36,1.,37.);
  h2CeVsEuPhiCePhiEu=fs->make<TH2D>("h2CeVsEuPhiCePhiEu", "h2CeVsEuPhiCePhiEu",  124, 0., 1240., 124, 0., 1240.);
  h2CeVsEuPhiCePhiEuDiff=fs->make<TH1D>("h2CeVsEuPhiCePhiEuDiff", "h2CeVsEuPhiCePhiEuDiff",  100, -100., 100.);
  h2CeVsEuThetaCeThetaEu=fs->make<TH2D>("h2CeVsEuThetaCeThetaEu", "h2CeVsEuThetaCeThetaEu",  32, 0., 32., 32, 0., 32.);
  h2CeVsEuBxCeBxEu = fs->make<TH2D>("h2CeVsEuBxCeBxEu","h2CeVsEuBxCeBxEu", 8, -3.5,4.5, 8, -3.5, 4.5);
  
  h1CeVsEuMatches_bx = fs->make<TH1D>("h1CeVsEuMatches_bx", "CPPFDigis_Matches_bx" , 5, 0. , 5.);
  h2CeVsEuPhiCePhiEu_bx=fs->make<TH2D>("h2CeVsEuPhiCePhiEu_bx", "h2CeVsEuPhiCePhiEu_bx",  124, 0., 1240., 124, 0., 1240.);
  h2CeVsEuThetaCeThetaEu_bx=fs->make<TH2D>("h2CeVsEuThetaCeThetaEu_bx", "h2CeVsEuThetaCeThetaEu_bx",  32, 0., 32., 32, 0., 32.);
  h2CeVsEuBxCeBxEu_bx = fs->make<TH2D>("h2CeVsEuBxCeBxEu_bx","h2CeVsEuBxCeBxEu_bx", 8, -3.5,4.5, 8, -3.5, 4.5);
  
  h1CeVsEuMatches_int = fs->make<TH1D>("h1CeVsEuMatches_int", "CPPFDigis_Matches_int" , 5, 0. , 5.);
  h2CeVsEuPhiCePhiEu_int=fs->make<TH2D>("h2CeVsEuPhiCePhiEu_int", "h2CeVsEuPhiCePhiEu_int",  124, 0., 1240., 124, 0., 1240.);
  h2CeVsEuThetaCeThetaEu_int=fs->make<TH2D>("h2CeVsEuThetaCeThetaEu_int", "h2CeVsEuThetaCeThetaEu_int",  32, 0., 32., 32, 0., 32.);
  h2CeVsEuChamberEuZoneEu = fs->make<TH2D>("h2CeVsEuChamberEuZoneEu", "CPPFDigis h2CeVsEuChamberEuZoneEu", 36, 0.5, 36.5, 12, 0.5,12.5); 
  h2CeVsEuChamberCeZoneCe = fs->make<TH2D>("h2CeVsEuChamberCeZoneCe", "CPPFDigis h2CeVsEuChamberCeZoneCe", 36, 0.5, 36.5, 12, 0.5,12.5); 
  
  
  h2CeVsEuBxEuZoneEuOccupancy = fs->make<TH2D>("h2CeVsEuBxEuZoneEuOccupancy","CPPFDigis Bx_Occupancy_emtf", 8, -3.5, 4.5, 12, 0.5, 12.5);
  h2CeVsEuBxCeZoneCeOccupancy = fs->make<TH2D>("h2CeVsEuBxCeZoneCeOccupancy","CPPFDigis Bx_Occupancy_emu_emtf", 8, -3.5, 4.5, 12, 0.5, 12.5);
  h2CeVsEuBxEuZoneEu = fs->make<TH2D>("h2CeVsEuBxEuZoneEu","CPPFDigis Bx_emtf", 12, 0.5, 12.5, 8,-3.5,4.5);
  h2CeVsEuBxCeZoneCe = fs->make<TH2D>("h2CeVsEuBxCeZoneCe","CPPFDigis Bx_emu_emtf", 12, 0.5, 12.5, 8,-3.5,4.5);
 
                                    
  h1CeVsEeMatches = fs->make<TH1D>("h1CeVsEeMatches", "CPPFDigis_h1CeVsEeMatches" , 5, 0. , 5.);
  
  h1CeVsEeMatches_ch = fs->make<TH1D>("h1CeVsEeMatches_ch", "CPPFDigis_Matches_ch" , 5, 0. , 5.);
  h2CeVsEeChamberCeChamberEe_test = fs->make<TH2D>("h2CeVsEeChamberCeChamberEe_test","h2CeVsEeChamberCeChamberEe_test", 36, 1.,37.,36,1.,37.);
  h2CeVsEeChamberCeChamberEe = fs->make<TH2D>("h2CeVsEeChamberCeChamberEe","h2CeVsEeChamberCeChamberEe", 36, 1.,37.,36,1.,37.);
  h2CeVsEePhiCePhiEe=fs->make<TH2D>("h2CeVsEePhiCePhiEe", "h2CeVsEePhiCePhiEe",  124, 0., 1240., 124, 0., 1240.);
  h2CeVsEePhiCePhiEeDiff=fs->make<TH1D>("h2CeVsEePhiCePhiEeDiff", "h2CeVsEePhiCePhiEeDiff",  100, -100., 100.);
  h2CeVsEeThetaCeThetaEe=fs->make<TH2D>("h2CeVsEeThetaCeThetaEe", "h2CeVsEeThetaCeThetaEe",  32, 0., 32., 32, 0., 32.);
  h2CeVsEeBxCeBxEe = fs->make<TH2D>("h2CeVsEeBxCeBxEe","h2CeVsEeBxCeBxEe", 8, -3.5,4.5, 8, -3.5, 4.5);
  
  h1CeVsEeMatches_bx = fs->make<TH1D>("h1CeVsEeMatches_bx", "CPPFDigis_Matches_bx" , 5, 0. , 5.);
  h2CeVsEePhiCePhiEe_bx=fs->make<TH2D>("h2CeVsEePhiCePhiEe_bx", "h2CeVsEePhiCePhiEe_bx",  124, 0., 1240., 124, 0., 1240.);
  h2CeVsEeThetaCeThetaEe_bx=fs->make<TH2D>("h2CeVsEeThetaCeThetaEe_bx", "h2CeVsEeThetaCeThetaEe_bx",  32, 0., 32., 32, 0., 32.);
  h2CeVsEeBxCeBxEe_bx = fs->make<TH2D>("h2CeVsEeBxCeBxEe_bx","h2CeVsEeBxCeBxEe_bx", 8, -3.5,4.5, 8, -3.5, 4.5);
  
  h1CeVsEeMatches_int = fs->make<TH1D>("h1CeVsEeMatches_int", "CPPFDigis_Matches_int" , 5, 0. , 5.);
  h2CeVsEePhiCePhiEe_int=fs->make<TH2D>("h2CeVsEePhiCePhiEe_int", "h2CeVsEePhiCePhiEe_int",  124, 0., 1240., 124, 0., 1240.);
  h2CeVsEeThetaCeThetaEe_int=fs->make<TH2D>("h2CeVsEeThetaCeThetaEe_int", "h2CeVsEeThetaCeThetaEe_int",  32, 0., 32., 32, 0., 32.);
  h2CeVsEeChamberEeZoneEe = fs->make<TH2D>("h2CeVsEeChamberEeZoneEe", "CPPFDigis h2CeVsEeChamberEeZoneEe", 36, 0.5, 36.5, 12, 0.5,12.5); 
  h2CeVsEeChamberCeZoneCe = fs->make<TH2D>("h2CeVsEeChamberCeZoneCe", "CPPFDigis h2CeVsEeChamberCeZoneCe", 36, 0.5, 36.5, 12, 0.5,12.5); 
  
  
  h2CeVsEeBxEeZoneEeOccupancy = fs->make<TH2D>("h2CeVsEeBxEeZoneEeOccupancy","CPPFDigis Bx_Occupancy_emtf", 8, -3.5, 4.5, 12, 0.5, 12.5);
  h2CeVsEeBxCeZoneCeOccupancy = fs->make<TH2D>("h2CeVsEeBxCeZoneCeOccupancy","CPPFDigis Bx_Occupancy_emu_emtf", 8, -3.5, 4.5, 12, 0.5, 12.5);
  h2CeVsEeBxEeZoneEe = fs->make<TH2D>("h2CeVsEeBxEeZoneEe","CPPFDigis Bx_emtf", 12, 0.5, 12.5, 8,-3.5,4.5);
  h2CeVsEeBxCeZoneCe = fs->make<TH2D>("h2CeVsEeBxCeZoneCe","CPPFDigis Bx_emu_emtf", 12, 0.5, 12.5, 8,-3.5,4.5);
 
  h2CuVsEuChamberCuChamberEu_test = fs->make<TH2D>("h2CuVsEuChamberCuChamberEu_test","h2CuVsEuChamberCuChamberEu_test", 36, 1.,37.,36,1.,37.);
  h2CuVsEuChamberCuChamberEu = fs->make<TH2D>("h2CuVsEuChamberCuChamberEu","h2CuVsEuChamberCuChamberEu", 36, 1.,37.,36,1.,37.);
  h2CuVsEuChamberCuChamberEuBox = fs->make<TH2D>("h2CuVsEuChamberCuChamberEuBox","h2CuVsEuChamberCuChamberEuBox", 36, 1.,37.,36,1.,37.);
  h2CuVsEuPhiCuPhiEu=fs->make<TH2D>("h2CuVsEuPhiCuPhiEu", "h2CuVsEuPhiCuPhiEu",  124, 0., 1240., 124, 0., 1240.);
  h2CuVsEuPhiCuPhiEuDiff=fs->make<TH1D>("h2CuVsEuPhiCuPhiEuDiff", "h2CuVsEuPhiCuPhiEuDiff",  100, -100., 100.);
  h2CuVsEuThetaCuThetaEu=fs->make<TH2D>("h2CuVsEuThetaCuThetaEu", "h2CuVsEuThetaCuThetaEu",  32, 0., 32., 32, 0., 32.);
  h2CuVsEuBxCuBxEu = fs->make<TH2D>("h2CuVsEuBxCuBxEu","h2CuVsEuBxCuBxEu", 8, -3.5,4.5, 8, -3.5, 4.5);
  
  h2CuVsEuPhiCuPhiEu_bx=fs->make<TH2D>("h2CuVsEuPhiCuPhiEu_bx", "h2CuVsEuPhiCuPhiEu_bx",  124, 0., 1240., 124, 0., 1240.);
  h2CuVsEuThetaCuThetaEu_bx=fs->make<TH2D>("h2CuVsEuThetaCuThetaEu_bx", "h2CuVsEuThetaCuThetaEu_bx",  32, 0., 32., 32, 0., 32.);
  h2CuVsEuBxCuBxEu_bx = fs->make<TH2D>("h2CuVsEuBxCuBxEu_bx","h2CuVsEuBxCuBxEu_bx", 8, -3.5,4.5, 8, -3.5, 4.5);
  
  h2CuVsEuPhiCuPhiEu_int=fs->make<TH2D>("h2CuVsEuPhiCuPhiEu_int", "h2CuVsEuPhiCuPhiEu_int",  124, 0., 1240., 124, 0., 1240.);
  h2CuVsEuThetaCuThetaEu_int=fs->make<TH2D>("h2CuVsEuThetaCuThetaEu_int", "h2CuVsEuThetaCuThetaEu_int",  32, 0., 32., 32, 0., 32.);
  h2CuVsEuChamberCuZoneCu = fs->make<TH2D>("h2CuVsEuChamberCuZoneCu", "CPPFDigis h2CuVsEuChamberCuZoneCu", 36, 0.5, 36.5, 12, 0.5,12.5); 
  h2CuVsEuChamberCuZoneCuBox = fs->make<TH2D>("h2CuVsEuChamberCuZoneCuBox", "CPPFDigis h2CuVsEuChamberCuZoneCuBox", 36, 0.5, 36.5, 12, 0.5,12.5); 
  h2CuVsEuChamberEuZoneEu = fs->make<TH2D>("h2CuVsEuChamberEuZoneEu", "CPPFDigis h2CuVsEuChamberEuZoneEu", 36, 0.5, 36.5, 12, 0.5,12.5); 
  h2CuVsEuChamberEuZoneEuBox = fs->make<TH2D>("h2CuVsEuChamberEuZoneEuBox", "CPPFDigis h2CuVsEuChamberEuZoneEuBox", 36, 0.5, 36.5, 12, 0.5,12.5); 
  
  h2CuVsEuBxCuZoneCuOccupancy = fs->make<TH2D>("h2CuVsEuBxCuZoneCuOccupancy","CPPFDigis Bx_Occupancy_unpacker_emtf_u", 8, -3.5, 4.5, 12, 0.5, 12.5);
  h2CuVsEuBxEuZoneEuOccupancy = fs->make<TH2D>("h2CuVsEuBxEuZoneEuOccupancy","CPPFDigis Bx_Occupancy_unpacker_emtf_e", 8, -3.5, 4.5, 12, 0.5, 12.5);
  h2CuVsEuBxCuZoneCu = fs->make<TH2D>("h2CuVsEuBxCuZoneCu","CPPFDigis Bx_unpacker_emtf_u", 12, 0.5, 12.5, 8,-3.5,4.5);
  h2CuVsEuBxEuZoneEu = fs->make<TH2D>("h2CuVsEuBxEuZoneEu","CPPFDigis Bx_unpacker_emtf_e", 12, 0.5, 12.5, 8,-3.5,4.5);




  h2EeVsEuChamberEeChamberEu_test = fs->make<TH2D>("h2EeVsEuChamberEeChamberEu_test","h2EeVsEuChamberEeChamberEu_test", 36, 1.,37.,36,1.,37.);
  h2EeVsEuChamberEeChamberEu = fs->make<TH2D>("h2EeVsEuChamberEeChamberEu","h2EeVsEuChamberEeChamberEu", 36, 1.,37.,36,1.,37.);
  h2EeVsEuPhiEePhiEu=fs->make<TH2D>("h2EeVsEuPhiEePhiEu", "h2EeVsEuPhiEePhiEu",  124, 0., 1240., 124, 0., 1240.);
  h2EeVsEuPhiEePhiEuDiff=fs->make<TH1D>("h2EeVsEuPhiEePhiEuDiff", "h2EeVsEuPhiEePhiEuDiff",  100, -100., 100.);
  h2EeVsEuThetaEeThetaEu=fs->make<TH2D>("h2EeVsEuThetaEeThetaEu", "h2EeVsEuThetaEeThetaEu",  32, 0., 32., 32, 0., 32.);
  h2EeVsEuBxEeBxEu = fs->make<TH2D>("h2EeVsEuBxEeBxEu","h2EeVsEuBxEeBxEu", 8, -3.5,4.5, 8, -3.5, 4.5);
  
  h2EeVsEuPhiEePhiEu_bx=fs->make<TH2D>("h2EeVsEuPhiEePhiEu_bx", "h2EeVsEuPhiEePhiEu_bx",  124, 0., 1240., 124, 0., 1240.);
  h2EeVsEuThetaEeThetaEu_bx=fs->make<TH2D>("h2EeVsEuThetaEeThetaEu_bx", "h2EeVsEuThetaEeThetaEu_bx",  32, 0., 32., 32, 0., 32.);
  h2EeVsEuBxEeBxEu_bx = fs->make<TH2D>("h2EeVsEuBxEeBxEu_bx","h2EeVsEuBxEeBxEu_bx", 8, -3.5,4.5, 8, -3.5, 4.5);
  
  h2EeVsEuPhiEePhiEu_int=fs->make<TH2D>("h2EeVsEuPhiEePhiEu_int", "h2EeVsEuPhiEePhiEu_int",  124, 0., 1240., 124, 0., 1240.);
  h2EeVsEuThetaEeThetaEu_int=fs->make<TH2D>("h2EeVsEuThetaEeThetaEu_int", "h2EeVsEuThetaEeThetaEu_int",  32, 0., 32., 32, 0., 32.);
  h2EeVsEuChamberEeZoneEe = fs->make<TH2D>("h2EeVsEuChamberEeZoneEe", "CPPFDigis h2EeVsEuChamberEeZoneEe", 36, 0.5, 36.5, 12, 0.5,12.5); 
  h2EeVsEuChamberEuZoneEu = fs->make<TH2D>("h2EeVsEuChamberEuZoneEu", "CPPFDigis h2EeVsEuChamberEuZoneEu", 36, 0.5, 36.5, 12, 0.5,12.5); 
  
  h2EeVsEuBxEeZoneEeOccupancy = fs->make<TH2D>("h2EeVsEuBxEeZoneEeOccupancy","CPPFDigis Bx_Occupancy_unpacker_emtf_u", 8, -3.5, 4.5, 12, 0.5, 12.5);
  h2EeVsEuBxEuZoneEuOccupancy = fs->make<TH2D>("h2EeVsEuBxEuZoneEuOccupancy","CPPFDigis Bx_Occupancy_unpacker_emtf_e", 8, -3.5, 4.5, 12, 0.5, 12.5);
  h2EeVsEuBxEeZoneEe = fs->make<TH2D>("h2EeVsEuBxEeZoneEe","CPPFDigis Bx_unpacker_emtf_u", 12, 0.5, 12.5, 8,-3.5,4.5);
  h2EeVsEuBxEuZoneEu = fs->make<TH2D>("h2EeVsEuBxEuZoneEu","CPPFDigis Bx_unpacker_emtf_e", 12, 0.5, 12.5, 8,-3.5,4.5);
  return;
}
//define this as a plug-in
DEFINE_FWK_MODULE(EMTF_CPPF_DQM);

