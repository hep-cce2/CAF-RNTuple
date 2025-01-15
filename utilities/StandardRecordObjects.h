// The idea is to create a toy framework that creates SRObjects which can later be filled in whether CAFTree or CAFRNTuple.
//We will start out with 3 simple StandardRecord Members as a starting point and later decide for ther.
//Note that these 3 SRObjects have most of the Basic SRObjects from which Complicated SRObjects are made.

#include "TFile.h"
#include "TTree.h"
#include "TRandom3.h"
#include "TLorentzVector.h"
#include "TTimeStamp.h"

#include "duneanaobj/StandardRecord/StandardRecord.h"
#include "duneanaobj/StandardRecord/SRDetectorMetaBranch.h"
#include "duneanaobj/StandardRecord/SRVector3D.h"
#include "duneanaobj/StandardRecord/SRLorentzVector.h"
#include "duneanaobj/StandardRecord/SREnums.h"


void FillSRMetaObject(caf::SRDetectorMeta &sm, int run, int subrun, int event, int subevt){
  auto rand = std::make_unique<TRandom3>();
  
  rand->SetSeed(0);

  sm.run = run;
  sm.subrun = subrun;
  sm.event = event;
  sm.subevt = subevt;
  
  //now we fill some random numbers...
  //lets just assume there are 5 trigger types.
  sm.triggertype = static_cast<int>(rand->Integer(5));
  
  //lets just assume some reasonable range..
  unsigned long readoutstart_s = rand->Uniform(1e9,1e10);
  unsigned long readoutstart_ns = rand->Uniform(0,1e9);
  
  //lets assume the readout window is 120 seconds (?)
  unsigned long readoutend_s = readoutstart_s+rand->Integer(120);
  //give some end limit for nano seconds...
  unsigned long readoutend_ns =  readoutstart_ns+rand->Uniform(0,1e9);
  
  if(readoutend_ns >=1e9){
    readoutend_s += 1 ; //second increase by 1 if ns overflow...
    readoutend_ns -= 1e9; //reset the ns to original value after second increase
  } 
  sm.readoutstart_s = readoutstart_s;
  sm.readoutstart_ns = readoutstart_ns;
  sm.readoutend_s = readoutend_s;
  sm.readoutend_ns = readoutend_ns;
  
  //and now we enable the boolean for detector data availability....
  sm.enabled = true;
}

//Use FillSRMeta to fill the SRDetectorMetaBranch Object
void FillSRDetectorMetaBranch(caf::SRDetectorMetaBranch& srd){
  auto rand = std::make_unique<TRandom3>();
  rand->SetSeed(0);
  int run = rand->Integer(100);
  int subrun = rand->Integer(100);
  int event = rand->Integer(100);
  int subevt = rand->Integer(100);
  
  FillSRMetaObject(srd.nd_lar, run, subrun, event, subevt);
  FillSRMetaObject(srd.nd_gar, run, subrun, event, subevt);
  FillSRMetaObject(srd.tms, run,subrun, event, subevt);
  FillSRMetaObject(srd.sand, run, subrun, event, subevt);
  FillSRMetaObject(srd.lar2x2, run, subrun, event, subevt);
  FillSRMetaObject(srd.minerva, run, subrun, event, subevt);
  FillSRMetaObject(srd.fd_hd, run, subrun, event, subevt);
  FillSRMetaObject(srd.pd_hd, run, subrun, event, subevt); 
}

//create an instance of SRVector3D, fill with rand values and return.
caf::SRVector3D FillSRVector3D(TRandom3* rand){
  float_t arr[3];
  rand->RndmArray(3,arr);
  caf::SRVector3D srv(arr[0],arr[1],arr[2]);

  return srv;
}

caf::SRLorentzVector FillSRLorentzVector(TRandom3* rand){
  float_t arr[4];
  rand->RndmArray(4,arr);
  TLorentzVector tlv(arr[0],arr[1],arr[2],arr[3]);
  caf::SRLorentzVector srv(tlv);

  return srv;
}

caf::TrueParticleID FillTrueParticleID(TRandom3* rand){
  caf::TrueParticleID part;
  
  part.ixn = rand->Integer(10);
  part.type = static_cast<caf::TrueParticleID::PartType>(rand->Integer(caf::TrueParticleID::PartType::kSecondary+1));
  part.part = rand->Integer(10);

  return part;
}

caf::SRTrueParticle FillTrueParticles(TRandom3* rand){
  
  caf::SRTrueParticle srp;
  
  srp.pdg = rand->Integer(10000);
  srp.G4ID = rand->Uniform(-1,100);
  srp.interaction_id = rand->Integer(100);
  srp.time = rand->Uniform(0,200);
  srp.ancestor_id = FillTrueParticleID(rand);
  srp.p = FillSRLorentzVector(rand);
  srp.start_pos = FillSRVector3D(rand);
  srp.end_pos = FillSRVector3D(rand);
  srp.parent = rand->Integer(50);
  int n_daughter = rand->Integer(10);
  std::vector<unsigned int>daughters;
  for(int i = 0;i<n_daughter;i++) 
    daughters.emplace_back(rand->Integer(10)); 
  
  srp.first_process = static_cast<unsigned int>(rand->Integer(1));
  srp.end_process = static_cast<unsigned int>(rand->Integer(1));
  
  return srp;
}


caf::SRTrueInteraction FillSRTrueInteraction(TRandom3* rand){
  
  //First fill the SRTrueInteraction basic types....
  caf::SRTrueInteraction srt;
  srt.id = rand->Integer(100);
  srt.genieIdx = rand->Integer(1000);
  srt.pdg = rand->Integer(2000);
  srt.pdgorig = rand->Integer(2000);
  srt.iscc = rand->Integer(1)==0?false:true;
  srt.targetPDG = rand->Integer(2000);
  srt.hitnuc = rand->Uniform(10000000,20000000);
  srt.removalE = rand->Uniform(0,120);
  srt.E = rand->Uniform(0,120);
  srt.vtx = FillSRVector3D(rand);
  srt.momentum = FillSRVector3D(rand);
  srt.isvtxcont = rand->Integer(1)==0?false:true;
  srt.time = rand->Uniform(0,200);
  srt.bjorkenX = rand->Uniform(0,1);
  srt.inelasticity = rand->Uniform(0,1);
  srt.Q2 = abs(rand->Gaus(0.1,100));
  srt.q0 = rand->Uniform(0,120);
  srt.modq = rand->Uniform(0,120);
  srt.W = sqrt(rand->Uniform(0,120));
  srt.t  = rand->Uniform(0,119);
  srt.ischarm = rand->Integer(1)==0?false:true;
  srt.isseaquark = rand->Integer(1)==0?false:true;
  srt.resnum = rand->Integer(10);
  srt.xsec = rand->Uniform(0,10)*1E-39;
  srt.genweight = rand->Uniform(0,1);
  srt.baseline = rand->Uniform(0,5);
  srt.prod_vtx  = FillSRVector3D(rand);
  srt.parent_dcy_mom = FillSRVector3D(rand);
  srt.parent_dcy_mode = rand->Integer(10);
  srt.parent_pdg = rand->Integer(1000);
  srt.parent_dcy_E = rand->Uniform(0,120);
  srt.imp_weight = rand->Uniform(0,1);
  std::vector<unsigned int>genVersion{rand->Integer(10),rand->Integer(20),rand->Integer(30)};
  srt.genVersion = genVersion;
  srt.nproton = rand->Integer(10);
  srt.nneutron = rand->Integer(10);
  srt.npip = rand->Integer(10);
  srt.npim = rand->Integer(10);
  srt.npi0 = rand->Integer(10);
  int nprim = rand->Integer(10);
  srt.nprim = nprim;
  std::vector<caf::SRTrueParticle>prefsi;
  for(int i = 0;i<nprim;i++)
    prefsi.emplace_back(FillTrueParticles(rand));
  srt.prefsi = prefsi;
  int nsec = rand->Integer(10);
  srt.nsec = nsec;
  std::vector<caf::SRTrueParticle>sec;
  for(int i = 0;i<nsec;i++)
    sec.emplace_back(FillTrueParticles(rand));
  srt.nsec = nsec;
  srt.xsec_cvwgt = rand->Uniform(0,1);
  
  return srt;
}

//Fill the SRTruthBranch object (Reconstructed)
void FillSRTruthBranchObject(caf::StandardRecord& sr){
  auto rand = std::make_shared<TRandom3>();
  rand->SetSeed(0);
  int nnu = rand->Integer(10);
  sr.mc.nnu = nnu;
  std::vector<caf::SRTrueInteraction>nu;
  for(int i = 0;i<nnu;i++){
    nu.emplace_back(FillSRTrueInteraction(rand.get()));
  }
}

//Fill the SRTruthBranch object (Reconstructed)
void FillSRTruthBranchObject(std::shared_ptr<caf::StandardRecord>& sr){
  auto rand = std::make_shared<TRandom3>();
  rand->SetSeed(0);
  int nnu = rand->Integer(10);
  sr->mc.nnu = nnu;
  std::vector<caf::SRTrueInteraction>nu;
  for(int i = 0;i<nnu;i++){
    nu.emplace_back(FillSRTrueInteraction(rand.get()));
  }
}

//Now we fill the SRBeamObjects....
void FillSRBeamObject(caf::StandardRecord& sr, bool ismc=true){
  auto rand = std::make_unique<TRandom3>();
  rand->SetSeed(0);
  sr.beam.ismc = ismc;
  unsigned long pulsetimesec = rand->Uniform(0,100);
  unsigned long pulsetimensec = rand->Uniform(1e9,1e10);

  //now for the gps time...
  TTimeStamp curr;
  
  unsigned long int gpspulsetimesec = curr.GetSec();
  unsigned long int gpspulsetimensec = rand->Uniform(1e9,1e10);
  unsigned long int deltapulsetimensec = rand->Uniform(1e9,1e10);
  
  float hornI = 230;
  
  //now assign values....
  sr.beam.pulsetimesec = pulsetimesec;
  sr.beam.pulsetimensec = pulsetimensec;
  
  sr.beam.gpspulsetimesec = gpspulsetimesec;
  sr.beam.gpspulsetimensec = gpspulsetimensec;
  sr.beam.deltapulsetimensec = deltapulsetimensec;
  sr.beam.hornI = hornI;
}

//Now we fill the SRBeamObjects....
void FillSRBeamObject(std::shared_ptr<caf::StandardRecord>& sr, bool ismc=true){
  auto rand = std::make_unique<TRandom3>();
  rand->SetSeed(0);
  sr->beam.ismc = ismc;
  unsigned long pulsetimesec = rand->Uniform(0,100);
  unsigned long pulsetimensec = rand->Uniform(1e9,1e10);

  //now for the gps time...
  TTimeStamp curr;
  unsigned long int gpspulsetimesec = curr.GetSec();
  unsigned long int gpspulsetimensec = rand->Uniform(1e9,1e10);
  unsigned long int deltapulsetimensec = rand->Uniform(1e9,1e10);
  float hornI = 230;

  //now assign values....
  sr->beam.pulsetimesec = pulsetimesec;
  sr->beam.pulsetimensec = pulsetimensec;
  sr->beam.gpspulsetimesec = gpspulsetimesec;
  sr->beam.gpspulsetimensec = gpspulsetimensec;
  sr->beam.deltapulsetimensec = deltapulsetimensec;
  sr->beam.hornI = hornI; 
}

caf::SRRecoParticle FillSRRecoParticle(TRandom3 *rand){
  caf::SRRecoParticle srp;
  srp.primary = rand->Integer(1)==0?true:false;
  srp.pdg = rand->Uniform(100,1000);
  srp.tgtA = rand->Uniform(4,80);
  srp.score = rand->Uniform(0,1);
  srp.E = rand->Uniform(0,120);
  srp.p = FillSRVector3D(rand);
  srp.start = FillSRVector3D(rand);
  srp.end = FillSRVector3D(rand);
  srp.contained = rand->Integer(1)==0?true:false;
  int n_trueparticle = rand->Integer(5);

  std::vector<caf::TrueParticleID>truth;
  for(int i = 0;i<n_trueparticle;i++)
    truth.emplace_back(FillTrueParticleID(rand));
  
  std::vector<float>truthOverlap;
  for(int i = 0;i<n_trueparticle;i++)
    truthOverlap.emplace_back(rand->Uniform(0,1));
  
  srp.truth = truth;
  srp.truthOverlap = truthOverlap;

  return srp;
}

caf::SRRecoParticlesBranch FillSRRecoParticleBranch(TRandom3* rand){
  caf::SRRecoParticlesBranch srp;
  int ndlp = rand->Integer(5);
  std::vector<caf::SRRecoParticle>dlp;
  for(int i = 0;i<ndlp;i++)
    dlp.emplace_back(FillSRRecoParticle(rand));
  srp.ndlp = ndlp;
  srp.dlp = dlp;
  
  int npandora = rand->Integer(5);
  std::vector<caf::SRRecoParticle>pandora;
  for(int i = 0;i<npandora;i++)
    pandora.emplace_back(FillSRRecoParticle(rand));
  srp.npandora = ndlp;
  srp.pandora = dlp;
  
  int npida = rand->Integer(5);
  std::vector<caf::SRRecoParticle>pida;
  for(int i = 0;i<npida;i++)
    pida.emplace_back(FillSRRecoParticle(rand));
  srp.npida = npida;
  srp.pida = pida;  

  return srp;
}

caf::SRNeutrinoEnergyBranch FillNuEnergyBranch(TRandom3* rand){
  caf::SRNeutrinoEnergyBranch srn; 
  srn.calo = rand->Uniform(0,100);
  srn.lep_calo = rand->Uniform(0,100);
  srn.regcnn = rand->Uniform(0,1);
  
  return srn;
}

caf::SRDirectionBranch FillDirectionBranch(TRandom3* rand){
  caf::SRDirectionBranch srd;
  srd.lngtrk = FillSRVector3D(rand);
  srd.heshw = FillSRVector3D(rand);
  
  return srd;

}

caf::SRCVNScoreBranch FillSRCVNScoreBranch(TRandom3* rand){
  caf::SRCVNScoreBranch srv;
  srv.isnubar = rand->Integer(1)==0?true:false;
  srv.nue = rand->Uniform(0,1);
  srv.numu = rand->Uniform(0,1);
  srv.nutau = rand->Uniform(0,1);
  srv.nc = rand->Uniform(0,1);
  srv.protons0 = rand->Uniform(0,1);
  srv.protons1 = rand->Uniform(0,1);
  srv.protons2 = rand->Uniform(0,1);
  srv.protonsN = rand->Uniform(0,1);
  srv.chgpi0 = rand->Uniform(0,1);
  srv.chgpi1 = rand->Uniform(0,1);
  srv.chgpi2 = rand->Uniform(0,1);
  srv.chgpiN = rand->Uniform(0,1);
  srv.pizero0 = rand->Uniform(0,1);
  srv.pizero1 = rand->Uniform(0,1);
  srv.pizero2 = rand->Uniform(0,1);
  srv.pizeroN = rand->Uniform(0,1);
  srv.neutron0 = rand->Uniform(0,1);
  srv.neutron1 = rand->Uniform(0,1);
  srv.neutron2 = rand->Uniform(0,1);
  srv.neutronN = rand->Uniform(0,1);
  
  return srv;  
}

caf::SRNeutrinoHypothesisBranch FillNuHypothesis(TRandom3* rand){
  caf::SRNeutrinoHypothesisBranch srnu ;
  srnu.cvn = FillSRCVNScoreBranch(rand);

  return srnu;
}

caf::SRInteraction FillSRInteraction(TRandom3* rand){
  caf::SRInteraction sri;
  sri.id = rand->Integer(1);
  sri.vtx = FillSRVector3D(rand);
  sri.dir = FillDirectionBranch(rand);
  sri.nuhyp = FillNuHypothesis(rand);
  sri.Enu = FillNuEnergyBranch(rand);
  sri.part = FillSRRecoParticleBranch(rand);
  
  int n_trueparticle = rand->Integer(5);
  std::vector<std::size_t>truth;
  for(int i = 0;i<n_trueparticle;i++)
    truth.emplace_back(i);
  
  std::vector<float>truthOverlap;
  for(int i = 0;i<n_trueparticle;i++)
    truthOverlap.emplace_back(rand->Uniform(0,1));
  
  sri.truth = truth;
  sri.truthOverlap = truthOverlap;

  return sri;
}


//Now we fill the SRCommonRecoBranch......
void FillSRCommonRecoBranchObject(caf::StandardRecord& sr){
  auto rand = std::make_shared<TRandom3>();
  rand->SetSeed(0); 
  size_t ndlp = rand->Integer(10);
  std::vector<caf::SRInteraction>dlp;
  for(int i = 0;i<ndlp;i++)
    dlp.emplace_back(FillSRInteraction(rand.get()));
  
  size_t npandora = rand->Integer(10);
  std::vector<caf::SRInteraction>pandora;
  for(int i = 0;i<npandora;i++)
    pandora.emplace_back(FillSRInteraction(rand.get()));
  
  caf::SRInteractionBranch srb;
  srb.ndlp = ndlp;
  srb.dlp = dlp;
  srb.npandora = npandora;
  srb.pandora = pandora;

  sr.common.ixn = srb;
}

//Now we fill the SRCommonRecoBranch......
void FillSRCommonRecoBranchObject(std::shared_ptr<caf::StandardRecord>& sr){
  auto rand = std::make_shared<TRandom3>();
  rand->SetSeed(0); 
  size_t ndlp = rand->Integer(10);
  std::vector<caf::SRInteraction>dlp;
  for(int i = 0;i<ndlp;i++)
    dlp.emplace_back(FillSRInteraction(rand.get()));
  
  size_t npandora = rand->Integer(10);
  std::vector<caf::SRInteraction>pandora;
  for(int i = 0;i<npandora;i++)
    pandora.emplace_back(FillSRInteraction(rand.get()));
  
  caf::SRInteractionBranch srb;
  srb.ndlp = ndlp;
  srb.dlp = dlp;
  srb.npandora = npandora;
  srb.pandora = pandora;
  
  sr->common.ixn = srb;
}

caf::SRTrack FillSRTrack(TRandom3 *rand){
  caf::SRTrack srt;
  srt.start = FillSRVector3D(rand);
  srt.end = FillSRVector3D(rand);
  srt.dir = FillSRVector3D(rand);
  srt.Evis = rand->Uniform(0,100);
  srt.qual = rand->Uniform(0,1);
  srt.len_gcm2 = rand->Uniform(0,10);
  srt.len_cm = rand->Uniform(0,10);
  srt.E = rand->Uniform(0,120000);
  
  int n_trueparticle = rand->Integer(5);
  std::vector<caf::TrueParticleID>truth;
  for(int i = 0;i<n_trueparticle;i++)
    truth.emplace_back(FillTrueParticleID(rand));
  
  std::vector<float>truthOverlap;
  for(int i = 0;i<n_trueparticle;i++)
    truthOverlap.emplace_back(rand->Uniform(0,1));
  
  srt.truth = truth;
  srt.truthOverlap = truthOverlap;  
  return srt;
}


caf::SRShower FillSRShower(TRandom3 *rand){
  caf::SRShower srs;
  srs.start = FillSRVector3D(rand);
  srs.direction = FillSRVector3D(rand);
  srs.Evis = rand->Uniform(0,100);
  
  int n_trueparticle = rand->Integer(5);
  std::vector<caf::TrueParticleID>truth;
  for(int i = 0;i<n_trueparticle;i++)
    truth.emplace_back(FillTrueParticleID(rand));
  
  std::vector<float>truthOverlap;
  for(int i = 0;i<n_trueparticle;i++)
    truthOverlap.emplace_back(rand->Uniform(0,1));
  
  srs.truth = truth;
  srs.truthOverlap = truthOverlap;  
  return srs;
}

caf::SRFDInt FillSRFDInt(TRandom3 *rand){
  caf::SRFDInt srf;
  int ntracks = rand->Integer(5);
  std::vector<caf::SRTrack>tracks;
  for(int i = 0;i<ntracks;i++)
    tracks.emplace_back(FillSRTrack(rand));
  srf.ntracks = ntracks;
  srf.tracks = tracks;
  
  int nshowers = rand->Integer(3);
  std::vector<caf::SRShower>showers;
  
  for(int i = 0;i<nshowers;i++)
    showers.emplace_back(FillSRShower(rand));
  srf.showers = showers;
  srf.nshowers = nshowers;
  
  return srf;
}

caf::SRFDID FillSRFDID(TRandom3* rand){
  caf::SRFDID srd;
  srd.ixn = rand->Integer(10);
  srd.idx = rand->Integer(10);
  
  return srd;
}

caf::SRFD FillSRFD(TRandom3* rand){
  caf::SRFD srfd;
  int npandora = rand->Integer(10);
  std::vector<caf::SRFDInt>pandora;
  for(int i = 0;i<npandora;i++)
    pandora.emplace_back(FillSRFDInt(rand));
  
  srfd.npandora = npandora;
  srfd.pandora = pandora;
  
  return srfd;
  
}

//Now we fill the SRFDBranch......

void FillSRFDBranch(caf::StandardRecord& sr){
  auto rand = std::make_shared<TRandom3>();
  rand->SetSeed(0); 
  
  sr.fd.hd = FillSRFD(rand.get());
  sr.fd.vd = FillSRFD(rand.get());
  sr.fd.pd_hd = FillSRFD(rand.get());
  sr.fd.pd_vd = FillSRFD(rand.get());
}

//Now we fill the SRFDBranch......

void FillSRFDBranch(std::shared_ptr<caf::StandardRecord>& sr){
  auto rand = std::make_shared<TRandom3>();
  rand->SetSeed(0); 
  
  sr->fd.hd = FillSRFD(rand.get());
  sr->fd.vd = FillSRFD(rand.get());
  sr->fd.pd_hd = FillSRFD(rand.get());
  sr->fd.pd_vd = FillSRFD(rand.get());
}

//I will leave out the SRND for now...this code needs reorganization.
