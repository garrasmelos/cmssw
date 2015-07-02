
#include "GeneratorInterface/Core/interface/GeneratorFilter.h"
#include "GeneratorInterface/ExternalDecays/interface/ExternalDecayDriver.h"

#include "GeneratorInterface/Pythia8Interface/interface/Py8GunBase.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include <TH1F.h>
#include <TF1.h>
#include <TRandom3.h>
#include "TRandom2.h"
#include "TMath.h"

namespace gen {
class Py8PtGunTFunc : public Py8GunBase {
   
   public:
      
      Py8PtGunTFunc( edm::ParameterSet const& );
      ~Py8PtGunTFunc() {}
	 
      bool generatePartonsAndHadronize() override;
      const char* classname() const override;
      
	 
   private:
      
      // PtGun particle(s) characteristics
      double  fMinEta;
      double  fMaxEta;
      double  fMinPt ;
      double  fMaxPt ;
      bool    fAddAntiParticle;
      TF1* f1;

};

// implementation 
//
Py8PtGunTFunc::Py8PtGunTFunc( edm::ParameterSet const& ps )
   : Py8GunBase(ps) 
{
  using std::string;

   // ParameterSet defpset ;
   edm::ParameterSet pgun_params = 
      ps.getParameter<edm::ParameterSet>("PGunParameters"); // , defpset ) ;
   fMinEta     = pgun_params.getParameter<double>("MinEta"); // ,-2.2);
   fMaxEta     = pgun_params.getParameter<double>("MaxEta"); // , 2.2);
   fMinPt      = pgun_params.getParameter<double>("MinPt"); // ,  0.);
   fMaxPt      = pgun_params.getParameter<double>("MaxPt"); // ,  0.);
   fAddAntiParticle = pgun_params.getParameter<bool>("AddAntiParticle"); //, false) ;  
   
   string tfunction_string = pgun_params.getParameter<string>("TFunction_string");
   double tfunction_min = pgun_params.getParameter<double>("TFunction_min");
   double tfunction_max = pgun_params.getParameter<double>("TFunction_max");

   f1 = new TF1("pt_func", tfunction_string.c_str(), tfunction_min, tfunction_max);
   if ( fMinPt < tfunction_min || fMaxPt > tfunction_max) std::cout << "Warning: The cuts for the pt are out of the limits of the function."<< "\n";


}
bool Py8PtGunTFunc::generatePartonsAndHadronize()
{

   fMasterGen->event.reset();
   
   for ( size_t i=0; i<fPartIDs.size(); i++ )
   {

      int particleID = fPartIDs[i]; // this is PDG - need to convert to Py8 ???

      double phi = (fMaxPhi-fMinPhi) * randomEngine().flat() + fMinPhi;
      double eta  = (fMaxEta-fMinEta) * randomEngine().flat() + fMinEta;
      double the  = 2.*atan(exp(-eta));

      double  pt = f1->GetRandom();
      if (pt<fMinPt || pt > fMaxPt) continue;



      double mass = (fMasterGen->particleData).m0( particleID );

      double pp = pt / sin(the); // sqrt( ee*ee - mass*mass );
      double ee = sqrt( pp*pp + mass*mass );
      
      double px = pt * cos(phi);
      double py = pt * sin(phi);
      double pz = pp * cos(the);

      if ( !((fMasterGen->particleData).isParticle( particleID )) )
      {
         particleID = std::fabs(particleID) ;
      }
      if( 1<= fabs(particleID) && fabs(particleID) <= 6) // quarks
	(fMasterGen->event).append( particleID, 23, 101, 0, px, py, pz, ee, mass ); 
      else if (fabs(particleID) == 21)                   // gluons
	(fMasterGen->event).append( 21, 23, 101, 102, px, py, pz, ee, mass );
      else                                               // other
	(fMasterGen->event).append( particleID, 1, 0, 0, px, py, pz, ee, mass ); 
      
// Here also need to add anti-particle (if any)
// otherwise just add a 2nd particle of the same type 
// (for example, gamma)
//
      if ( fAddAntiParticle )
      {
	if( 1 <= fabs(particleID) && fabs(particleID) <= 6){ // quarks
	  (fMasterGen->event).append( -particleID, 23, 0, 101, -px, -py, -pz, ee, mass );
	}
	else if (fabs(particleID) == 21){                   // gluons
	  (fMasterGen->event).append( 21, 23, 102, 101, -px, -py, -pz, ee, mass );
	}
	else if ( (fMasterGen->particleData).isParticle( -particleID ) )
	  {
	    (fMasterGen->event).append( -particleID, 1, 0, 0, -px, -py, -pz, ee, mass );
	  }
	else
	  {
	    (fMasterGen->event).append( particleID, 1, 0, 0, -px, -py, -pz, ee, mass );
	  }
      }
   }


   if ( !fMasterGen->next() ) return false;
   
   event().reset(new HepMC::GenEvent);
   return toHepMC.fill_next_event( fMasterGen->event, event().get() );
  
}

const char* Py8PtGunTFunc::classname() const
{
   return "Py8PtGunTFunc"; 
}

typedef edm::GeneratorFilter<gen::Py8PtGunTFunc, gen::ExternalDecayDriver> Pythia8PtGunTFunc;

} // end namespace

using gen::Pythia8PtGunTFunc;
DEFINE_FWK_MODULE(Pythia8PtGunTFunc);
