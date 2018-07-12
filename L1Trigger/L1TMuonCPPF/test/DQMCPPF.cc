#include "DQMCPPF.h"

DQM_CPPF::DQM_CPPF(const edm::ParameterSet& iConfig) :
  cppfDigiToken1_(consumes<l1t::CPPFDigiCollection>(iConfig.getParameter<edm::InputTag>("cppfdigiLabel1"))),
  cppfDigiToken2_(consumes<l1t::CPPFDigiCollection>(iConfig.getParameter<edm::InputTag>("cppfdigiLabel2"))),
  cppfDigiToken3_(consumes<l1t::CPPFDigiCollection>(iConfig.getParameter<edm::InputTag>("cppfdigiLabel3"))),
  EMTF_sector(0),    
  EMTF_subsector(0),
  EMTF_subsector_unpacker(0),
  EMTF_subsector_EMTF(0),
  EMTF_bx(0) {
}

DQM_CPPF::~DQM_CPPF(){
}

void DQM_CPPF::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){  
  
  //Get the CPPFDigi 
  edm::Handle<l1t::CPPFDigiCollection> CppfDigis1;
  edm::Handle<l1t::CPPFDigiCollection> CppfDigis2;
  edm::Handle<l1t::CPPFDigiCollection> CppfDigis3;
  iEvent.getByToken(cppfDigiToken1_, CppfDigis1);
  iEvent.getByToken(cppfDigiToken2_, CppfDigis2);
  iEvent.getByToken(cppfDigiToken3_, CppfDigis3);
  
  //Fill the specific bin for each EMTF sector 
  for(int i = 1; i < 7; i++ ){
    EMTFsector0bins.push_back(0);
    EMTFsector1bins.push_back(i);
    EMTFsector2bins.push_back(i+6);
    EMTFsector3bins.push_back(i+12);
    EMTFsector4bins.push_back(i+18);
    EMTFsector5bins.push_back(i+24);
    EMTFsector6bins.push_back(i+30);
  }
  //FIll the map for each EMTF sector 
  fill_info[1] = EMTFsector1bins;
  fill_info[2] = EMTFsector2bins;
  fill_info[3] = EMTFsector3bins;
  fill_info[4] = EMTFsector4bins;
  fill_info[5] = EMTFsector5bins;
  fill_info[6] = EMTFsector6bins;
  fill_info[-99] = EMTFsector0bins;
  
  
  int matches_unpacker = 0;
  int matches_unpacker_bx = 0;
  int matches_unpacker_int = 0;
  int matches_emtf = 0;
  int matches_emtf_bx = 0;
  int matches_emtf_int = 0;
  
  for(auto& cppf_digis : *CppfDigis1){
    
    RPCDetId rpcId = cppf_digis.rpcId();
    int region = rpcId.region();
    int station = rpcId.station();
    int ring = rpcId.ring();
    int sector = rpcId.sector();
    int subsector = rpcId.subsector();
    int phi_int = cppf_digis.phi_int();
    int theta_int =  cppf_digis.theta_int();
    
    EMTF_sector = cppf_digis.emtf_sector();
    EMTF_subsector = fill_info[EMTF_sector][subsector-1];
    
    EMTF_bx = cppf_digis.bx();
    if(EMTF_bx >= 0) EMTF_bx = EMTF_bx+0.5;  
    else if(EMTF_bx < 0) EMTF_bx = EMTF_bx-0.5;  
    
    int fill_occupancy = occupancy_value(region, station, ring);
    int fill_bx = bx_value(region, EMTF_sector); 
    
    //Chamber ID
    int nsub_e = 6;
    (ring == 1 && station > 1) ? nsub_e = 3 : nsub_e = 6;
    int chamberID = subsector + nsub_e * ( sector - 1);
    
    
    matches_unpacker = 0;  
    matches_unpacker_bx = 0;
    matches_unpacker_int = 0;
    for(auto& cppf_digis_unpacker : *CppfDigis2){
      RPCDetId rpcId_unpacker = cppf_digis_unpacker.rpcId();
      int region_unpacker = rpcId_unpacker.region();
      int station_unpacker = rpcId_unpacker.station();
      int ring_unpacker = rpcId_unpacker.ring();
      int sector_unpacker = rpcId_unpacker.sector();
      int subsector_unpacker = rpcId_unpacker.subsector();
      int phi_int_unpacker = cppf_digis_unpacker.phi_int();
      int theta_int_unpacker =  cppf_digis_unpacker.theta_int();
      int bx_unpacker = cppf_digis_unpacker.bx();
      
      int EMTF_sector_u = cppf_digis_unpacker.emtf_sector();
      //int EMTF_subsector_u = fill_info[EMTF_sector_u][subsector_unpacker-1];
      int EMTF_subsector_u = fill_info[sector_unpacker][subsector_unpacker-1];

      //Chamber ID
      int nsub_un = 6;
      (ring_unpacker == 1 && station_unpacker > 1) ? nsub_un = 3 : nsub_un = 6;
      int chamberID_unpacker = subsector_unpacker + nsub_un * ( sector_unpacker - 1);
      
      int fill_occupancy_unpacker = occupancy_value(region_unpacker, station_unpacker, ring_unpacker);
      int fill_bx_unpacker = bx_value(region_unpacker, EMTF_sector_u);
      
      if((region == region_unpacker) && 
	        (station == station_unpacker) &&
	        (ring == ring_unpacker) &&
	        (sector == sector_unpacker) &&
	        (subsector == subsector_unpacker)){
	
	      matches_unpacker++;
        Matches_unpacker_ch->Fill(matches_unpacker);
	
        chamber_emu_unpacker->Fill(chamberID, chamberID_unpacker); 
	
        phi_emu_unpacker->Fill(phi_int, phi_int_unpacker);
        theta_emu_unpacker->Fill(theta_int, theta_int_unpacker);
        bx_emu_unpacker->Fill(cppf_digis.bx(),bx_unpacker);
	
        if(cppf_digis.bx() ==  bx_unpacker){
	        matches_unpacker_bx++;
	        Matches_unpacker_bx->Fill(matches_unpacker_bx);	
	        phi_emu_unpacker_bx->Fill(phi_int, phi_int_unpacker);
	        theta_emu_unpacker_bx->Fill(theta_int, theta_int_unpacker); 
	        bx_emu_unpacker_bx->Fill(cppf_digis.bx(),bx_unpacker);
	  
	        if((phi_int == phi_int_unpacker) && (theta_int == theta_int_unpacker)){
	          matches_unpacker_int++;
	          Matches_unpacker_int->Fill(matches_unpacker_int);	
	          phi_emu_unpacker_int->Fill(phi_int, phi_int_unpacker);
	          theta_emu_unpacker_int->Fill(theta_int, theta_int_unpacker); 
            //Occupancy 
	          occupancy_unpacker->Fill(EMTF_subsector_u, fill_occupancy_unpacker);
	          //occupancy_unpacker->Fill(EMTF_subsector, fill_occupancy);
	          occupancy_emu_unpacker->Fill(EMTF_subsector, fill_occupancy);
            //bx occupancy 
            bx_occupancy_unpacker->Fill(bx_unpacker, fill_occupancy_unpacker);
            bx_occupancy_emu_unpacker->Fill(EMTF_bx, fill_occupancy);
            //bx
            bx_sector_unpacker->Fill(fill_bx_unpacker,bx_unpacker);
            bx_sector_emu_unpacker->Fill(fill_bx,EMTF_bx);
	        } 
	      }
      }
    }
    //Matching information
    Matches_unpacker->Fill(matches_unpacker);
    matches_emtf = 0;
    matches_emtf_bx = 0; 
    matches_emtf_int = 0; 
    
    for(auto& cppf_digis_emtf : *CppfDigis3){
      
      RPCDetId rpcId_emtf = cppf_digis_emtf.rpcId();
      int region_emtf = rpcId_emtf.region();
      int station_emtf = rpcId_emtf.station();
      int ring_emtf = rpcId_emtf.ring();
      int sector_emtf = rpcId_emtf.sector();
      int subsector_emtf = rpcId_emtf.subsector();
      int phi_int_emtf = cppf_digis_emtf.phi_int();
      int theta_int_emtf =  cppf_digis_emtf.theta_int();
      int bx_emtf = cppf_digis_emtf.bx();
      
      //Chamber ID
      int nsub_un = 6;
      (ring_emtf == 1 && station_emtf > 1) ? nsub_un = 3 : nsub_un = 6;
      int chamberID_emtf = subsector_emtf + nsub_un * ( sector_emtf - 1);
      
      int EMTF_sector_e = cppf_digis_emtf.emtf_sector();
      //int EMTF_subsector_e = fill_info[EMTF_sector_e][subsector_emtf-1];
      int EMTF_subsector_e = fill_info[sector_emtf][subsector_emtf-1];
      
      if((phi_int == phi_int_emtf) && (theta_int == theta_int_emtf) && (region == region_emtf)){
	      chamber_emu_emtf_test->Fill(chamberID, chamberID_emtf);
      }
      int fill_occupancy_emtf = occupancy_value(region_emtf, station_emtf, ring_emtf);
      int fill_bx_emtf = bx_value(region_emtf, EMTF_sector_e);

      if((chamberID_emtf == 35) || (chamberID_emtf == 36)) chamberID_emtf -= 34;
      else chamberID_emtf += 2;
      /*
      if((region == region_emtf) && 
	        (station == station_emtf) &&
	        (ring == ring_emtf) &&
	        (sector == sector_emtf) &&
	        (subsector == subsector_emtf)){
      */
      if((chamberID == chamberID_emtf) && (region == region_emtf) 
         && (ring == ring_emtf) && (station == station_emtf) && (region ==region_emtf) 
          ){
	      matches_emtf++;
        Matches_emtf_ch->Fill(matches_emtf);
	
        chamber_emu_emtf->Fill(chamberID, chamberID_emtf); 
	
        phi_emu_emtf->Fill(phi_int, phi_int_emtf);
        theta_emu_emtf->Fill(theta_int, theta_int_emtf);
        bx_emu_emtf->Fill(cppf_digis.bx(),bx_emtf);
	
        if(cppf_digis.bx() ==  bx_emtf){
	        matches_emtf_bx++;
	        Matches_emtf_bx->Fill(matches_emtf_bx);	    
	        phi_emu_emtf_bx->Fill(phi_int, phi_int_emtf);
	        theta_emu_emtf_bx->Fill(theta_int, theta_int_emtf); 
	        bx_emu_emtf_bx->Fill(cppf_digis.bx(),bx_emtf);
	        if((phi_int == phi_int_emtf) && (theta_int == theta_int_emtf)){
	          matches_emtf_int++;
	          Matches_emtf_int->Fill(matches_emtf_int);
	          phi_emu_emtf_int->Fill(phi_int, phi_int_emtf);
	          theta_emu_emtf_int->Fill(theta_int, theta_int_emtf);
 
            //Occupancy
	          occupancy_emtf->Fill(EMTF_subsector_e, fill_occupancy_emtf);
            //Setting up the subsector
            //if((EMTF_subsector == 1) || (EMTF_subsector == 2)) EMTF_subsector += 34;
            //else EMTF_subsector -= 2; 		 
            if((EMTF_subsector_e == 35) || (EMTF_subsector_e == 36)) EMTF_subsector_e -= 34;
            else EMTF_subsector_e += 2; 		 
	          occupancy_emu_emtf->Fill(EMTF_subsector, fill_occupancy); 
            //bx occupancy 
            bx_occupancy_emtf->Fill(bx_emtf, fill_occupancy_emtf);
            bx_occupancy_emu_emtf->Fill(EMTF_bx, fill_occupancy);
            //bx
            bx_sector_emtf->Fill(fill_bx_emtf,bx_emtf);
            bx_sector_emu_emtf->Fill(fill_bx,EMTF_bx);
	        }	    
        }
      }
    }
    //Matching information
    Matches_emtf->Fill(matches_emtf);
  } // loop over CPPFDigis
  
  //Comparison between CPPF and EMTF
    
  for(auto& cppf_digis_unpacker : *CppfDigis2){
    RPCDetId rpcId_unpacker = cppf_digis_unpacker.rpcId();
    int region_unpacker = rpcId_unpacker.region();
    int station_unpacker = rpcId_unpacker.station();
    int ring_unpacker = rpcId_unpacker.ring();
    int sector_unpacker = rpcId_unpacker.sector();
    int subsector_unpacker = rpcId_unpacker.subsector();
    int phi_int_unpacker = cppf_digis_unpacker.phi_int();
    int theta_int_unpacker =  cppf_digis_unpacker.theta_int();
    int bx_unpacker = cppf_digis_unpacker.bx();
    
    //std::cout << "Subsector CPPF unpacker" << subsector_unpacker << std::endl;
    int EMTF_sector_u = cppf_digis_unpacker.emtf_sector();
    //int EMTF_subsector_u = fill_info[EMTF_sector_u][subsector_unpacker-1];
    int EMTF_subsector_u = fill_info[sector_unpacker][subsector_unpacker-1];

    //Chamber ID
    int nsub_un = 6;
    (ring_unpacker == 1 && station_unpacker > 1) ? nsub_un = 3 : nsub_un = 6;
    int chamberID_unpacker = subsector_unpacker + nsub_un * ( sector_unpacker - 1);
    
    int fill_occupancy_unpacker = occupancy_value(region_unpacker, station_unpacker, ring_unpacker);
    int fill_bx_unpacker = bx_value(region_unpacker, EMTF_sector_u);
    
    for(auto& cppf_digis_emtf : *CppfDigis3){
      
      RPCDetId rpcId_emtf = cppf_digis_emtf.rpcId();
      int region_emtf = rpcId_emtf.region();
      int station_emtf = rpcId_emtf.station();
      int ring_emtf = rpcId_emtf.ring();
      int sector_emtf = rpcId_emtf.sector();
      int subsector_emtf = rpcId_emtf.subsector();
      int phi_int_emtf = cppf_digis_emtf.phi_int();
      int theta_int_emtf =  cppf_digis_emtf.theta_int();
      int bx_emtf = cppf_digis_emtf.bx();
      
      //Chamber ID
      int nsub_un = 6;
      (ring_emtf == 1 && station_emtf > 1) ? nsub_un = 3 : nsub_un = 6;
      int chamberID_emtf = subsector_emtf + nsub_un * ( sector_emtf - 1);
      
      if((phi_int_unpacker == phi_int_emtf) && (theta_int_unpacker == theta_int_emtf) && (region_unpacker == region_emtf)){
	      chamber_unpacker_emtf_test->Fill(chamberID_unpacker, chamberID_emtf);
      }
      int fill_occupancy_emtf = occupancy_value(region_emtf, station_emtf, ring_emtf);
      int EMTF_sector_e = cppf_digis_emtf.emtf_sector();
      int EMTF_subsector_e = fill_info[sector_emtf][subsector_emtf-1];

      int fill_bx_emtf = bx_value(region_emtf, EMTF_sector_e);

      if((chamberID_emtf == 35) || (chamberID_emtf == 36)) chamberID_emtf -= 34;
      else chamberID_emtf += 2;

      if((chamberID_unpacker == chamberID_emtf)  
         && (ring_unpacker == ring_emtf) && (station_unpacker == station_emtf) && (region_unpacker ==region_emtf) 
          ){
	
        chamber_unpacker_emtf->Fill(chamberID_unpacker, chamberID_emtf); 
	
        phi_unpacker_emtf->Fill(phi_int_unpacker, phi_int_emtf);
        theta_unpacker_emtf->Fill(theta_int_unpacker, theta_int_emtf);
        bx_unpacker_emtf->Fill(cppf_digis_unpacker.bx(),bx_emtf);
	
        if(bx_unpacker ==  bx_emtf){
	        phi_unpacker_emtf_bx->Fill(phi_int_unpacker, phi_int_emtf);
	        theta_unpacker_emtf_bx->Fill(theta_int_unpacker, theta_int_emtf); 
	        bx_unpacker_emtf_bx->Fill(bx_unpacker,bx_emtf);
	        if((phi_int_unpacker == phi_int_emtf) && (theta_int_unpacker == theta_int_emtf)){
	          phi_unpacker_emtf_int->Fill(phi_int_unpacker, phi_int_emtf);
	          theta_unpacker_emtf_int->Fill(theta_int_unpacker, theta_int_emtf);
 
            //Setting up the subsector
            //if((EMTF_subsector_u == 1) || (EMTF_subsector_u == 2)) EMTF_subsector_u += 34;
            //else EMTF_subsector_u -= 2; 		 
            if((EMTF_subsector_e == 35) || (EMTF_subsector_e == 36)) EMTF_subsector_e -= 34;
            else EMTF_subsector_e += 2; 		 
	          occupancy_unpacker_emtf_u->Fill(EMTF_subsector_u, fill_occupancy_unpacker); 
	          occupancy_unpacker_emtf_e->Fill(EMTF_subsector_e, fill_occupancy_emtf); 
            //bx occupancy 
            bx_occupancy_unpacker_emtf_u->Fill(bx_unpacker, fill_occupancy_unpacker);
            bx_occupancy_unpacker_emtf_e->Fill(bx_emtf, fill_occupancy_emtf);
            //bx
            bx_sector_unpacker_emtf_u->Fill(fill_bx_unpacker,bx_unpacker);
            bx_sector_unpacker_emtf_e->Fill(fill_bx_emtf,bx_emtf);
	        }	    
        }
      }
    }
  } // loop over CPPFDigis
  
} //End class

int DQM_CPPF::occupancy_value(int region_, int station_, int ring_){
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

int DQM_CPPF::bx_value(int region_, int emtfsector_){
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


void DQM_CPPF::beginRun(const edm::Run& run, const edm::EventSetup& iSetup){
  iSetup.get<MuonGeometryRecord>().get(rpcGeom);
}

void DQM_CPPF::beginJob(){
  edm::Service<TFileService> fs;
  
  Matches_unpacker = fs->make<TH1D>("Matches_unpacker", "CPPFDigis_Matches_unpacker" , 5, 0. , 5.);
  
  Matches_unpacker_ch = fs->make<TH1D>("Matches_unpacker_ch", "CPPFDigis_Matches_unpacker_ch" , 5, 0. , 5.);
  chamber_emu_unpacker = fs->make<TH2D>("chamber_emu_unpacker","chamber_emu_unpacker", 36, 1.,37.,36,1.,37.);
  phi_emu_unpacker=fs->make<TH2D>("phi_emu_unpacker", "phi_emu_unpacker",  124, 0., 1240., 124, 0., 1240.);
  theta_emu_unpacker=fs->make<TH2D>("theta_emu_unpacker", "theta_emu_unpacker",  32, 0., 32., 32, 0., 32.);
  bx_emu_unpacker = fs->make<TH2D>("bx_emu_unpacker","bx_emu_unpacker", 8, -3.5,4.5, 8, -3.5, 4.5);
  
  Matches_unpacker_bx = fs->make<TH1D>("Matches_unpacker_bx", "CPPFDigis_Matches_bx" , 5, 0. , 5.);
  phi_emu_unpacker_bx=fs->make<TH2D>("phi_emu_unpacker_bx", "phi_emu_unpacker_bx",  124, 0., 1240., 124, 0., 1240.);
  theta_emu_unpacker_bx=fs->make<TH2D>("theta_emu_unpacker_bx", "theta_emu_unpacker_bx",  32, 0., 32., 32, 0., 32.);
  bx_emu_unpacker_bx = fs->make<TH2D>("bx_emu_unpacker_bx","bx_emu_unpacker_bx", 8, -3.5,4.5, 8, -3.5, 4.5);
  
  Matches_unpacker_int = fs->make<TH1D>("Matches_unpacker_int", "CPPFDigis_Matches_int" , 5, 0. , 5.);
  phi_emu_unpacker_int=fs->make<TH2D>("phi_emu_unpacker_int", "phi_emu_unpacker_int",  124, 0., 1240., 124, 0., 1240.);
  theta_emu_unpacker_int=fs->make<TH2D>("theta_emu_unpacker_int", "theta_emu_unpacker_int",  32, 0., 32., 32, 0., 32.);
  occupancy_unpacker = fs->make<TH2D>("occupancy_unpacker", "CPPFDigis occupancy_unpacker", 36, 0.5, 36.5, 12, 0.5,12.5); 
  occupancy_emu_unpacker = fs->make<TH2D>("occupancy_emu_unpacker", "CPPFDigis occupancy_emu_unpacker", 36, 0.5, 36.5, 12, 0.5,12.5); 
  bx_occupancy_unpacker = fs->make<TH2D>("bx_occupancy_unpacker","CPPFDigis Bx_Occupancy_unpacker", 8, -3.5, 4.5, 12, 0.5, 12.5);
  bx_occupancy_emu_unpacker = fs->make<TH2D>("bx_occupancy_emu_unpacker","CPPFDigis Bx_Occupancy_emu_unpacker", 8, -3.5, 4.5, 12, 0.5, 12.5);
  bx_sector_unpacker = fs->make<TH2D>("bx_sector_unpacker","CPPFDigis Bx_unpacker", 12, 0.5, 12.5, 8,-3.5,4.5);
  bx_sector_emu_unpacker = fs->make<TH2D>("bx_sector_emu_unpacker","CPPFDigis Bx_emu_unpacker", 12, 0.5, 12.5, 8,-3.5,4.5);


  Matches_emtf = fs->make<TH1D>("Matches_emtf", "CPPFDigis_Matches_emtf" , 5, 0. , 5.);
  
  Matches_emtf_ch = fs->make<TH1D>("Matches_emtf_ch", "CPPFDigis_Matches_ch" , 5, 0. , 5.);
  chamber_emu_emtf_test = fs->make<TH2D>("chamber_emu_emtf_test","chamber_emu_emtf_test", 36, 1.,37.,36,1.,37.);
  chamber_emu_emtf = fs->make<TH2D>("chamber_emu_emtf","chamber_emu_emtf", 36, 1.,37.,36,1.,37.);
  phi_emu_emtf=fs->make<TH2D>("phi_emu_emtf", "phi_emu_emtf",  124, 0., 1240., 124, 0., 1240.);
  theta_emu_emtf=fs->make<TH2D>("theta_emu_emtf", "theta_emu_emtf",  32, 0., 32., 32, 0., 32.);
  bx_emu_emtf = fs->make<TH2D>("bx_emu_emtf","bx_emu_emtf", 8, -3.5,4.5, 8, -3.5, 4.5);
  
  Matches_emtf_bx = fs->make<TH1D>("Matches_emtf_bx", "CPPFDigis_Matches_bx" , 5, 0. , 5.);
  phi_emu_emtf_bx=fs->make<TH2D>("phi_emu_emtf_bx", "phi_emu_emtf_bx",  124, 0., 1240., 124, 0., 1240.);
  theta_emu_emtf_bx=fs->make<TH2D>("theta_emu_emtf_bx", "theta_emu_emtf_bx",  32, 0., 32., 32, 0., 32.);
  bx_emu_emtf_bx = fs->make<TH2D>("bx_emu_emtf_bx","bx_emu_emtf_bx", 8, -3.5,4.5, 8, -3.5, 4.5);
  
  Matches_emtf_int = fs->make<TH1D>("Matches_emtf_int", "CPPFDigis_Matches_int" , 5, 0. , 5.);
  phi_emu_emtf_int=fs->make<TH2D>("phi_emu_emtf_int", "phi_emu_emtf_int",  124, 0., 1240., 124, 0., 1240.);
  theta_emu_emtf_int=fs->make<TH2D>("theta_emu_emtf_int", "theta_emu_emtf_int",  32, 0., 32., 32, 0., 32.);
  occupancy_emtf = fs->make<TH2D>("occupancy_emtf", "CPPFDigis occupancy_emtf", 36, 0.5, 36.5, 12, 0.5,12.5); 
  occupancy_emu_emtf = fs->make<TH2D>("occupancy_emu_emtf", "CPPFDigis occupancy_emu_emtf", 36, 0.5, 36.5, 12, 0.5,12.5); 
  
  
  bx_occupancy_emtf = fs->make<TH2D>("bx_occupancy_emtf","CPPFDigis Bx_Occupancy_emtf", 8, -3.5, 4.5, 12, 0.5, 12.5);
  bx_occupancy_emu_emtf = fs->make<TH2D>("bx_occupancy_emu_emtf","CPPFDigis Bx_Occupancy_emu_emtf", 8, -3.5, 4.5, 12, 0.5, 12.5);
  bx_sector_emtf = fs->make<TH2D>("bx_sector_emtf","CPPFDigis Bx_emtf", 12, 0.5, 12.5, 8,-3.5,4.5);
  bx_sector_emu_emtf = fs->make<TH2D>("bx_sector_emu_emtf","CPPFDigis Bx_emu_emtf", 12, 0.5, 12.5, 8,-3.5,4.5);
 
                                    
 
  chamber_unpacker_emtf_test = fs->make<TH2D>("chamber_unpacker_emtf_test","chamber_unpacker_emtf_test", 36, 1.,37.,36,1.,37.);
  chamber_unpacker_emtf = fs->make<TH2D>("chamber_unpacker_emtf","chamber_unpacker_emtf", 36, 1.,37.,36,1.,37.);
  phi_unpacker_emtf=fs->make<TH2D>("phi_unpacker_emtf", "phi_unpacker_emtf",  124, 0., 1240., 124, 0., 1240.);
  theta_unpacker_emtf=fs->make<TH2D>("theta_unpacker_emtf", "theta_unpacker_emtf",  32, 0., 32., 32, 0., 32.);
  bx_unpacker_emtf = fs->make<TH2D>("bx_unpacker_emtf","bx_unpacker_emtf", 8, -3.5,4.5, 8, -3.5, 4.5);
  
  phi_unpacker_emtf_bx=fs->make<TH2D>("phi_unpacker_emtf_bx", "phi_unpacker_emtf_bx",  124, 0., 1240., 124, 0., 1240.);
  theta_unpacker_emtf_bx=fs->make<TH2D>("theta_unpacker_emtf_bx", "theta_unpacker_emtf_bx",  32, 0., 32., 32, 0., 32.);
  bx_unpacker_emtf_bx = fs->make<TH2D>("bx_unpacker_emtf_bx","bx_unpacker_emtf_bx", 8, -3.5,4.5, 8, -3.5, 4.5);
  
  phi_unpacker_emtf_int=fs->make<TH2D>("phi_unpacker_emtf_int", "phi_unpacker_emtf_int",  124, 0., 1240., 124, 0., 1240.);
  theta_unpacker_emtf_int=fs->make<TH2D>("theta_unpacker_emtf_int", "theta_unpacker_emtf_int",  32, 0., 32., 32, 0., 32.);
  occupancy_unpacker_emtf_u = fs->make<TH2D>("occupancy_unpacker_emtf_u", "CPPFDigis occupancy_unpacker_emtf_u", 36, 0.5, 36.5, 12, 0.5,12.5); 
  occupancy_unpacker_emtf_e = fs->make<TH2D>("occupancy_unpacker_emtf_e", "CPPFDigis occupancy_unpacker_emtf_e", 36, 0.5, 36.5, 12, 0.5,12.5); 
  
  bx_occupancy_unpacker_emtf_u = fs->make<TH2D>("bx_occupancy_unpacker_emtf_u","CPPFDigis Bx_Occupancy_unpacker_emtf_u", 8, -3.5, 4.5, 12, 0.5, 12.5);
  bx_occupancy_unpacker_emtf_e = fs->make<TH2D>("bx_occupancy_unpacker_emtf_e","CPPFDigis Bx_Occupancy_unpacker_emtf_e", 8, -3.5, 4.5, 12, 0.5, 12.5);
  bx_sector_unpacker_emtf_u = fs->make<TH2D>("bx_sector_unpacker_emtf_u","CPPFDigis Bx_unpacker_emtf_u", 12, 0.5, 12.5, 8,-3.5,4.5);
  bx_sector_unpacker_emtf_e = fs->make<TH2D>("bx_sector_unpacker_emtf_e","CPPFDigis Bx_unpacker_emtf_e", 12, 0.5, 12.5, 8,-3.5,4.5);
  return;
}
//define this as a plug-in
DEFINE_FWK_MODULE(DQM_CPPF);

