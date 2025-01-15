//Helper Functions
#include "utilities/FillStandardRecordObjects.h"

//ROOT and TTree related headers
#include <TROOT.h>
#include <TTree.h>
#include <TFile.h>


#define n_events 50000
const char *fname = "CAF_TestTree.root";
const char *container_name = "cafTree";
const char *obj_name ="StandardRecord";

void WriteStandardRecord(){
  //Simplified and relevant code snippets taken from ND_CAFMaker CAF.cxx/CAF.h
  TFile *cafFile = new TFile(fname, "recreate");
  std::shared_ptr<caf::StandardRecord> sr = std::make_shared<caf::StandardRecord>();
  TTree *cafSR = new TTree(container_name,"Tree Container to write StandardRecord Object");
  cafSR->Branch(obj_name,"caf::StandardRecord",sr.get());
  
  //Write Data...
  for(int i=0;i<n_events;i++){
    //Fill the data....
    FillSRObjects(sr);

    //Write the data
    cafSR->Fill();
  }

  cafFile->cd();
  cafSR->Write();
  cafFile->Close();
}

void ReadStandardRecord(){
  TFile *file = TFile::Open(fname);

  TTree *tree = (TTree*)file->Get(container_name);
  std::shared_ptr<caf::StandardRecord>sr = std::make_shared<caf::StandardRecord>();
  caf::StandardRecord *raw_sr = sr.get();
  //TTree expects address of the raw pointer
  tree->SetBranchAddress(obj_name,&raw_sr);
  //Get Total Number of Entries
  auto nentries = tree->GetEntries();
  //Get Total Number of Branches (1 in this case)
  std::cout<<"Total Entries "<<nentries<<std::endl;
  auto nbranches = tree->GetNbranches();
  TimerUtil timer;
  timer.start();
  for(int i=0;i<n_events;i++){
    //Get ith entry from TTree
    tree->GetEntry(i);
    //Print StandardRecord members
    PrintCAFObjects<caf::StandardRecord>(*raw_sr,i);
  }
  timer.stop();
  std::cout<<"Elapsed time (sec) "<<timer.elapsed()<<std::endl;
  //Print the schema of the Tree
  tree->Print();
}

int main(){
  WriteStandardRecord();
  ReadStandardRecord();

  return 1;
}
