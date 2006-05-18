/**
 *  See header file for a description of this class.
 *  
 *  All the code is under revision
 *
 *  $Date: 2006/05/16 09:33:38 $
 *  $Revision: 1.2 $
 *
 *  \author A. Vitelli - INFN Torino, V.Palichik
 *  \author ported by: R. Bellan - INFN Torino
 */


#include "RecoMuon/MuonSeedGenerator/src/MuonSeedGenerator.h"

#include "RecoMuon/MuonSeedGenerator/src/MuonSeedFinder.h"
//was
//#include "Muon/MuonSeedGenerator/src/MuonSeedGeneratorByRecHits.h"
 
#include "DataFormats/TrajectorySeed/interface/TrajectorySeed.h"
#include "DataFormats/TrajectorySeed/interface/TrajectorySeedCollection.h"

#include "DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h"
#include "DataFormats/CSCRecHit/interface/CSCRecHit2D.h"

#include "DataFormats/DTRecHit/interface/DTRecSegment4DCollection.h"
#include "DataFormats/DTRecHit/interface/DTRecSegment4D.h"

#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
// Geometry
#include "TrackingTools/DetLayers/interface/DetLayer.h"
//was
//#include "CommonDet/DetLayout/interface/DetLayer.h"

// maybe not necessary
// #include "Geometry/Records/interface/MuonGeometryRecord.h"
// #include "Geometry/DTGeometry/interface/DTGeometry.h"
// #include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include "RecoMuon/DetLayers/interface/MuonDetLayerGeometry.h"
#include "RecoMuon/Records/interface/MuonRecoGeometryRecord.h"
//

// Framework
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Handle.h"

#include <vector>

#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHit.h"

#include "RecoMuon/MeasurementDet/interface/MuonDetLayerMeasurements.h"

using namespace std;

// Constructor
MuonSeedGenerator::MuonSeedGenerator(const edm::ParameterSet& pset){
  produces<TrajectorySeedCollection>(); 

  // the name of the DT rec hits collection
  theDTRecSegmentLabel = pset.getParameter<string>("DTRecSegmentLabel");
  // the name of the CSC rec hits collection
  theCSCRecSegmentLabel = pset.getParameter<string>("CSCRecSegmentLabel");
}

// Destructor
MuonSeedGenerator::~MuonSeedGenerator(){};


// reconstruct muon's seeds
void MuonSeedGenerator::produce(edm::Event& event, const edm::EventSetup& eSetup){

  theSeeds.clear();
  
  // create the pointer to the Seed container
  auto_ptr<TrajectorySeedCollection> output(new TrajectorySeedCollection());
  
  // divide the RecHits by DetLayer, in order to fill the
  // RecHitContainer like it was in ORCA. (part between [1] and [/1])
  
  //>> Muon Geometry - DT, CSC and RPC 
  edm::ESHandle<MuonDetLayerGeometry> muonLayers;
  eSetup.get<MuonRecoGeometryRecord>().get(muonLayers);

  // get the DT layers
  vector<DetLayer*> dtLayers = muonLayers->allDTLayers();

  // get the CSC layers
  vector<DetLayer*> cscForwardLayers = muonLayers->forwardCSCLayers();
  vector<DetLayer*> cscBackwardLayers = muonLayers->backwardCSCLayers();
  //<<
  
  // Backward (z<0) EndCap disk
  const DetLayer* ME4Bwd = cscBackwardLayers[4];
  const DetLayer* ME3Bwd = cscBackwardLayers[3];
  const DetLayer* ME2Bwd = cscBackwardLayers[2];
  const DetLayer* ME12Bwd = cscBackwardLayers[1];
  const DetLayer* ME11Bwd = cscBackwardLayers[0];
  
  // Forward (z>0) EndCap disk
  const DetLayer* ME11Fwd = cscForwardLayers[0];
  const DetLayer* ME12Fwd = cscForwardLayers[1];
  const DetLayer* ME2Fwd = cscForwardLayers[2];
  const DetLayer* ME3Fwd = cscForwardLayers[3];
  const DetLayer* ME4Fwd = cscForwardLayers[4];
     
  // barrel
  const DetLayer* MB4DL = dtLayers[3];
  const DetLayer* MB3DL = dtLayers[2];
  const DetLayer* MB2DL = dtLayers[1];
  const DetLayer* MB1DL = dtLayers[0];
  
  // instantiate the accessor
  //  MuonDetLayerMeasurements muonMeasurements(eSetup);
  MuonDetLayerMeasurements muonMeasurements;

  // ------------        EndCap disk z<0 + barrel

  RecHitContainer list24 = muonMeasurements.recHits(ME4Bwd,event);
  RecHitContainer list23 = muonMeasurements.recHits(ME3Bwd,event);
  
  RecHitContainer list12 = muonMeasurements.recHits(ME2Bwd,event);
  
  RecHitContainer list22 = muonMeasurements.recHits(ME12Bwd,event);
  RecHitContainer list21 = muonMeasurements.recHits(ME11Bwd,event);

  RecHitContainer list11 = list21; 
  RecHitContainer list5 = list22;
  RecHitContainer list13 = list23;  
  RecHitContainer list4 = list24; 
 
  if ( list21.size() == 0 )  { 
    list11 = list22; list5 = list21;
  }

  if ( list24.size() < list23.size() && list24.size() > 0 )  { 
    list13 = list24; list4 = list23;
  }

  if ( list23.size() == 0 )  { 
    list13 = list24; list4 = list23;
  }

  RecHitContainer list1 = list11;
  RecHitContainer list2 = list12;
  RecHitContainer list3 = list13;


  if ( list12.size() == 0 )  { 
    list3 = list12;
    if ( list11.size() <= list13.size() && list11.size() > 0 ) {
      list1 = list11; list2 = list13;}
    else { list1 = list13; list2 = list11;}
  }

  if ( list13.size() == 0 )  { 
    if ( list11.size() <= list12.size() && list11.size() > 0 ) {
      list1 = list11; list2 = list12;}
    else { list1 = list12; list2 = list11;}
  }
   
  if ( list12.size() != 0 &&  list13.size() != 0 )  { 
    if ( list11.size()<=list12.size() && list11.size()<=list13.size() && list11.size()>0 ) {   // ME 1
      if ( list12.size() > list13.size() ) {
	list2 = list13; list3 = list12;}
    }
    else if ( list12.size() <= list13.size() ) {                                   //  start with ME 2
      list1 = list12;
      if ( list11.size() <= list13.size() && list11.size() > 0 ) {
	list2 = list11; list3 = list13;}
      else { list2 = list13; list3 = list11;}
    } 
    else {                                                                         //  start with ME 3
      list1 = list13;
      if ( list11.size() <= list12.size() && list11.size() > 0 ) {
	list2 = list11; list3 = list12;}
      else { list2 = list12; list3 = list11;}
    }
  }

  RecHitContainer list6 = muonMeasurements.recHits(MB3DL,event);
  RecHitContainer list7 = muonMeasurements.recHits(MB2DL,event);
  RecHitContainer list8 = muonMeasurements.recHits(MB1DL,event);
  
  bool* MB1 = 0;
  if (list8.size()) {
    MB1 = new bool[list8.size()];
    for ( size_t i=0; i<list8.size(); i++ ) MB1[i]=false;
  }
  bool* MB2 = 0;
  if (list7.size()) {
    MB2 = new bool[list7.size()];
    for ( size_t i=0; i<list7.size(); i++ ) MB2[i]=false;
  }
  bool* MB3 = 0;
  if (list6.size()) {
    MB3 = new bool[list6.size()];
    for ( size_t i=0; i<list6.size(); i++ ) MB3[i]=false;
  }

  // +v ME-flags:
  bool* ME2 = 0;
  if (list2.size()) {
    ME2 = new bool[list2.size()];
    for ( size_t i=0; i<list2.size(); i++ ) ME2[i]=false;
  }
  bool* ME3 = 0;
  if (list3.size()) {
    ME3 = new bool[list3.size()];
    for ( size_t i=0; i<list3.size(); i++ ) ME3[i]=false;
  }
  bool* ME4 = 0;
  if (list4.size()) {
    ME4 = new bool[list4.size()];
    for ( size_t i=0; i<list4.size(); i++ ) ME4[i]=false;
  }
  bool* ME5 = 0;
  if (list5.size()) {
    ME5 = new bool[list5.size()];
    for ( size_t i=0; i<list5.size(); i++ ) ME5[i]=false;
  }

  // creates list of compatible track segments

  RecHitIterator iter;

  for ( iter = list1.begin(); iter!=list1.end(); iter++ ){
    if ( (*iter)->recHits().size() < 4 && list3.size() > 0 ) continue; // 3p.tr-seg. are not so good for starting
    MuonSeedFinder Theseed;
    Theseed.add(*iter);
    complete(Theseed, list2, ME2);
    complete(Theseed, list3, ME3);
    complete(Theseed, list4, ME4);
    complete(Theseed, list5, ME5);
    complete(Theseed, list6, MB3);
    complete(Theseed, list7, MB2);
    complete(Theseed, list8, MB1);

    checkAndFill(Theseed);
  }


  unsigned int counter;

  for ( counter = 0; counter<list2.size(); counter++ ){
    if ( !ME2[counter] ) {
      MuonSeedFinder Theseed;
      Theseed.add(list2[counter]);
      complete(Theseed, list3, ME3);
      complete(Theseed, list4, ME4);
      complete(Theseed, list5, ME5);
      complete(Theseed, list6, MB3);
      complete(Theseed, list7, MB2);
      complete(Theseed, list8, MB1);

      checkAndFill(Theseed);
    }
  }


  if ( list3.size() < 20 ) {   // +v
    for ( counter = 0; counter<list3.size(); counter++ ){
      if ( !ME3[counter] ) { 
	MuonSeedFinder Theseed;
	Theseed.add(list3[counter]);
	complete(Theseed, list4, ME4);
	complete(Theseed, list5, ME5);
	complete(Theseed, list6, MB3);
	complete(Theseed, list7, MB2);
	complete(Theseed, list8, MB1);
	
	checkAndFill(Theseed);
      }
    }
  }

  if ( list4.size() < 20 ) {   // +v
    for ( counter = 0; counter<list4.size(); counter++ ){
      if ( !ME4[counter] ) {
	MuonSeedFinder Theseed;
	Theseed.add(list4[counter]);
	complete(Theseed, list5, ME5);
	complete(Theseed, list6, MB3);
	complete(Theseed, list7, MB2);
	complete(Theseed, list8, MB1);

	checkAndFill(Theseed);
      }   
    }          
  } 

  // ------------        EndCap disk z>0

  list24 = muonMeasurements.recHits(ME4Fwd,event);
  list23 = muonMeasurements.recHits(ME3Fwd,event);
  
  list12 = muonMeasurements.recHits(ME2Fwd,event);
  
  list22 = muonMeasurements.recHits(ME12Fwd,event);
  list21 = muonMeasurements.recHits(ME11Fwd,event);
  
 
  list11 = list21; 
  list5 = list22;
  list13 = list23;  
  list4 = list24; 

  if ( list21.size() == 0 )  { 
    list11 = list22; list5 = list21;
  }

  if ( list24.size() < list23.size() && list24.size() > 0 )  { 
    list13 = list24; list4 = list23;
  }

  if ( list23.size() == 0 )  { 
    list13 = list24; list4 = list23;
  }

  list1 = list11;
  list2 = list12;
  list3 = list13;


  if ( list12.size() == 0 )  { 
    list3 = list12;
    if ( list11.size() <= list13.size() && list11.size() > 0 ) {
      list1 = list11; list2 = list13;}
    else { list1 = list13; list2 = list11;}
  }

  if ( list13.size() == 0 )  { 
    if ( list11.size() <= list12.size() && list11.size() > 0 ) {
      list1 = list11; list2 = list12;}
    else { list1 = list12; list2 = list11;}
  }
   
  if ( list12.size() != 0 &&  list13.size() != 0 )  { 
    if ( list11.size()<=list12.size() && list11.size()<=list13.size() && list11.size()>0 ) {  // ME 1
      if ( list12.size() > list13.size() ) {
	list2 = list13; list3 = list12;}
    }
    else if ( list12.size() <= list13.size() ) {                                  //  start with ME 2
      list1 = list12;
      if ( list11.size() <= list13.size() && list11.size() > 0 ) {
	list2 = list11; list3 = list13;}
      else { list2 = list13; list3 = list11;}
    } 
    else {                                                                        //  start with ME 3
      list1 = list13;
      if ( list11.size() <= list12.size() && list11.size() > 0 ) {
	list2 = list11; list3 = list12;}
      else { list2 = list12; list3 = list11;}
    }
  }


  if ( ME5 ) delete [] ME5;
  if ( ME4 ) delete [] ME4;
  if ( ME3 ) delete [] ME3;
  if ( ME2 ) delete [] ME2;
  ME5 = ME4 = ME3 = ME2 = 0;

  if (list2.size()) {
    ME2 = new bool[list2.size()];
    for ( size_t i=0; i<list2.size(); i++ ) ME2[i]=false;
  }
  if (list3.size()) {
    ME3 = new bool[list3.size()];
    for ( size_t i=0; i<list3.size(); i++ ) ME3[i]=false;
  }
  if (list4.size()) {
    ME4 = new bool[list4.size()];
    for ( size_t i=0; i<list4.size(); i++ ) ME4[i]=false;
  }
  if (list5.size()) {
    ME5 = new bool[list5.size()];
    for ( size_t i=0; i<list5.size(); i++ ) ME5[i]=false;
  }



  for ( iter=list1.begin(); iter!=list1.end(); iter++ ){
    if ( (*iter)->recHits().size() < 4 && list3.size() > 0 ) continue;// 3p.tr-seg.aren't so good for starting
    MuonSeedFinder Theseed;
    Theseed.add(*iter);
    complete(Theseed, list2, ME2);
    complete(Theseed, list3, ME3);
    complete(Theseed, list4, ME4);
    complete(Theseed, list5, ME5);
    complete(Theseed, list6, MB3);
    complete(Theseed, list7, MB2);
    complete(Theseed, list8, MB1);

    checkAndFill(Theseed);
    
  }


  for ( counter = 0; counter<list2.size(); counter++ ){
    if ( !ME2[counter] ) {
      MuonSeedFinder Theseed;
      Theseed.add(list2[counter]);
      complete(Theseed, list3, ME3);
      complete(Theseed, list4, ME4);
      complete(Theseed, list5, ME5);
      complete(Theseed, list6, MB3);
      complete(Theseed, list7, MB2);
      complete(Theseed, list8, MB1);

      checkAndFill(Theseed);
    } 
  }


  if ( list3.size() < 20 ) {   // +v
    for ( counter = 0; counter<list3.size(); counter++ ){
      if ( !ME3[counter] ) { 
	MuonSeedFinder Theseed;
	Theseed.add(list3[counter]);
	complete(Theseed, list4, ME4);
	complete(Theseed, list5, ME5);
	complete(Theseed, list6, MB3);
	complete(Theseed, list7, MB2);
	complete(Theseed, list8, MB1);

	checkAndFill(Theseed);
      }
    }
  }

  if ( list4.size() < 20 ) {   // +v
    for ( counter = 0; counter<list4.size(); counter++ ){
      if ( !ME4[counter] ) {
	MuonSeedFinder Theseed;
	Theseed.add(list4[counter]);
	complete(Theseed, list5, ME5);
	complete(Theseed, list6, MB3);
	complete(Theseed, list7, MB2);
	complete(Theseed, list8, MB1);

	checkAndFill(Theseed);
      }   
    }          
  } 


  // ----------    Barrel only
  
  RecHitContainer list9 = muonMeasurements.recHits(MB4DL,event);

  if ( list9.size() < 100 ) {   // +v
    for ( iter=list9.begin(); iter!=list9.end(); iter++ ){
      MuonSeedFinder Theseed;
      Theseed.add(*iter);
      complete(Theseed, list6, MB3);
      complete(Theseed, list7, MB2);
      complete(Theseed, list8, MB1);

      checkAndFill(Theseed);
    }
  }


  if ( list6.size() < 100 ) {   // +v
    for ( counter = 0; counter<list6.size(); counter++ ){
      if ( !MB3[counter] ) { 
	MuonSeedFinder Theseed;
	Theseed.add(list6[counter]);
	complete(Theseed, list7, MB2);
	complete(Theseed, list8, MB1);
	complete(Theseed, list9);

	checkAndFill(Theseed);
      }
    }
  }


  if ( list7.size() < 100 ) {   // +v
    for ( counter = 0; counter<list7.size(); counter++ ){
      if ( !MB2[counter] ) { 
	MuonSeedFinder Theseed;
	Theseed.add(list7[counter]);
	complete(Theseed, list8, MB1);
	complete(Theseed, list9);
	complete(Theseed, list6, MB3);
	if (Theseed.nrhit()>1 || (Theseed.nrhit()==1 &&
				  Theseed.firstRecHit()->dimension()==4) ) {
	  checkAndFill(Theseed);


	}
      }
    }
  }


  if ( list8.size() < 100 ) {   // +v
    for ( counter = 0; counter<list8.size(); counter++ ){
      if ( !MB1[counter] ) { 
	MuonSeedFinder Theseed;
	Theseed.add(list8[counter]);
	complete(Theseed, list9);
	complete(Theseed, list6, MB3);
	complete(Theseed, list7, MB2);
	if (Theseed.nrhit()>1 || (Theseed.nrhit()==1 &&
				  Theseed.firstRecHit()->dimension()==4) ) {
	  checkAndFill(Theseed);
	}
      }
    }
  }

  if ( ME5 ) delete [] ME5;
  if ( ME4 ) delete [] ME4;
  if ( ME3 ) delete [] ME3;
  if ( ME2 ) delete [] ME2;
  if ( MB3 ) delete [] MB3;
  if ( MB2 ) delete [] MB2;
  if ( MB1 ) delete [] MB1;

  //>> NEW

  // what is the id??
  //  output->put(SLId, theSeeds.begin(),theSeeds.end());
  event.put(output);
  //<<
}


void MuonSeedGenerator::complete(MuonSeedFinder& seed,
                                 RecHitContainer &recHits, bool* used) const {

  RecHitContainer good_rhit;

  //+v get all rhits compatible with the seed on dEta/dPhi Glob.

  TransientTrackingRecHit *first = seed.firstRecHit(); // first rechit of seed

  GlobalPoint ptg2 = first->globalPosition(); // its global pos +v

  int nr=0; // count rechits we have checked against seed

  for ( RecHitIterator iter=recHits.begin(); iter!=recHits.end(); iter++){

    GlobalPoint ptg1 = (*iter)->globalPosition();  //+v global pos of rechit

    // Cox: Just too far away?
    if ( fabs (ptg1.eta()-ptg2.eta()) > .2 || fabs (ptg1.phi()-ptg2.phi()) > .1 ) {
      nr++;
      continue;
    }   // +vvp!!!


    if( fabs ( ptg2.eta() ) < 1.0 ) {     //  barrel only

      LocalPoint pt1 = first->det()->toLocal(ptg1); // local pos of rechit in seed's det

      // FIXME!!! In CMSSW is missing!!!
      // LocalVector dir1 = first->localDirection();
      LocalVector dir1;

      LocalPoint pt2 = first->localPosition();

      float m = dir1.z()/dir1.x();   // seed's slope in local xz
      float yf = pt1.z();            // local z of rechit
      float yi = pt2.z();            // local z of seed
      float xi = pt2.x();            // local x of seed
      float xf = (yf-yi)/m + xi;     // x of linear extrap alone seed direction to z of rechit
      float dist = fabs ( xf - pt1.x() ); // how close is actual to predicted local x ?

      float d_cut = sqrt((yf-yi)*(yf-yi)+(pt1.x()-pt2.x())*(pt1.x()-pt2.x()))/10.;


      //@@ Tim asks: what is the motivation for this cut?
      //@@ It requires (xpred-xrechit)< 0.1 * distance between rechit and seed in xz plane

      if ( dist < d_cut ) {
	good_rhit.push_back(*iter);
	if (used) used[nr]=true;
      }

    }  // eta  < 1.0



    if( fabs ( ptg2.eta() ) > 1.0 ) {    //  endcap & overlap.

      if ( fabs (ptg1.eta()-ptg2.eta()) < .1 && fabs (ptg1.phi()-ptg2.phi()) < .07 ) {  

	good_rhit.push_back(*iter);
	if (used) used[nr]=true;
      }

    }  // eta > 1.0


    nr++;

  }  // recHits iter


  // select the best rhit among the compatible ones (based on Dphi Glob & Dir)

  TransientTrackingRecHit *best=0;

  float best_dphiG = M_PI;
  float best_dphiD = M_PI;

  if( fabs ( ptg2.eta() ) > 1.0 ) {    //  endcap & overlap.
      
    // select the best rhit among the compatible ones (based on Dphi Glob & Dir)
      
    // FIXME!!! In CMSSW is missing!!!
    // GlobalVector dir2 =  first->globalDirection();
    GlobalVector dir2;

    GlobalPoint  pos2 =  first->globalPosition();  // +v
      
    for ( RecHitIterator iter=good_rhit.begin(); iter!=good_rhit.end(); iter++){


      GlobalPoint pos1 = (*iter)->globalPosition();  // +v
 
      float dphi = pos1.phi()-pos2.phi();       //+v

      if (dphi < 0.) dphi = -dphi;             //+v
      if (dphi > M_PI) dphi = 2.*M_PI - dphi;  //+v

   
      if (  dphi < best_dphiG*1.5 ) {  


	if (  dphi < best_dphiG*.67  && best_dphiG > .005 )  best_dphiD = M_PI;  // thresh. of strip order

	// FIXME!!! In CMSSW is missing!!!
	// GlobalVector dir1 = (*iter)->globalDirection();
	GlobalVector dir1;

	float  dphidir = fabs ( dir1.phi()-dir2.phi() );

	if (dphidir > M_PI) dphidir = 2.*M_PI - dphidir;
	if (dphidir > M_PI*.5) dphidir = M_PI - dphidir;  // +v  [0,pi/2]

	if (  dphidir < best_dphiD ) {

	  best_dphiG = dphi;
	  if ( dphi < .002 )  best_dphiG =  .002;                          // thresh. of half-strip order
	  best_dphiD = dphidir;
	  best = (*iter);

	}

      }


    }   //  rhit iter

  }  // eta > 1.0


  if( fabs ( ptg2.eta() ) < 1.0 ) {     //  barrel only

    // select the best rhit among the compatible ones (based on Dphi)

    float best_dphi = M_PI;

    for ( RecHitIterator iter=good_rhit.begin(); iter!=good_rhit.end(); iter++){
      // FIXME!!! In CMSSW is missing!!!
      // GlobalVector dir1 = (*iter)->globalDirection();
      GlobalVector dir1;

      //@@ Tim: Why do this again? 'first' hasn't changed, has it?
      //@@ I comment it out.
      //    RecHit first = seed.rhit();

      // FIXME!!! In CMSSW is missing!!!
      //GlobalVector dir2 = first->globalDirection();
      GlobalVector dir2;

      float dphi = dir1.phi()-dir2.phi();

      if (dphi < 0.) dphi = -dphi;
      if (dphi > M_PI) dphi = 2.*M_PI - dphi;

      if (  dphi < best_dphi ) {

	best_dphi = dphi;
	best = (*iter);
      }

    }   //  rhit iter

  }  // eta < 1.0


  // add the best Rhit to the seed 
  if ( best->isValid() ){
    seed.add(best);
  } 

}  //   void complete.


void MuonSeedGenerator::checkAndFill(MuonSeedFinder& Theseed){

  if (Theseed.nrhit()>1 ) {
    vector<TrajectorySeed> the_seeds =  Theseed.seeds();
    for (vector<TrajectorySeed>::const_iterator
	   the_seed=the_seeds.begin(); the_seed!=the_seeds.end(); ++the_seed) {
      
      // FIXME, ask for this method
      //if ( (*the_seed).isValid() )
      theSeeds.push_back(*the_seed);
    }
  }

}
