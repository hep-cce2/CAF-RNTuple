#undef R__HAS_VDT 

//Helper Functions
#include "utilities/FillStandardRecordObjects.h"

//Headers related to RNTuple
#include <ROOT/RNTuple.hxx>
#include <ROOT/RNTupleModel.hxx> //Equivalent to TTree
#include <ROOT/RNTupleInspector.hxx>
#include <ROOT/RNTupleWriter.hxx> //Facilitates writing data
#include <ROOT/RNTupleReader.hxx> //Facilitates reading data

R__LOAD_LIBRARY(ROOTNTuple)

//namespace for ROOT RNTuple
using namespace ROOT::Experimental;

#define n_events 50000

const char *fname = "CAF_TestRNTuple.root";
const char *container_name = "cafRNTuple";
const char *obj_name = "StandardRecord";

void WriteStandardRecord(){
  //Create a model, add StandardRecord as its field
  auto model = RNTupleModel::Create();
  std::shared_ptr<caf::StandardRecord> field_sr = model->MakeField<caf::StandardRecord>(obj_name);
  //Once the model is populated with field(s), transfer ownership to RNTupleWriter
  //RNTupleWriter creates a new file and writes the model into it.
  auto ntuple = RNTupleWriter::Recreate(std::move(model),container_name,fname);
  
  //Write Data
  for(int i = 0;i<n_events;i++){
    //Create an entry instance in which RFields are filled with data
    std::unique_ptr<REntry> entry = ntuple->CreateEntry();
    std::shared_ptr<caf::StandardRecord>f_sr = entry->GetPtr<caf::StandardRecord>(obj_name);
    
    //Fill the data...
    FillSRObjects(f_sr);

    //Write the entry
    ntuple->Fill(*entry);  
  }
}

void ReadStandardRecord(){
  //Open the RNTupleModel and the file 
  auto ntuple = RNTupleReader::Open(container_name,fname); 
  const auto& ntuple_desc = ntuple->GetDescriptor();
  //Get Total Number of Entries
  auto nentries = ntuple->GetNEntries();
  std::cout<<"Total Entries "<<nentries<<std::endl;
  //Get Total Number of Fields (1 in this case)
  auto nfields = ntuple_desc.GetNFields();
  //create a view for StandardRecord
  //You can also create view for attributes of StandardRecord individually for selective I/O
  auto v_sr = ntuple->GetView<caf::StandardRecord>(obj_name);
  TimerUtil timer;
  timer.start(); //start timer
  for(int i=0;i<n_events;i++){
    //View of StandardRecord for the ith entry
    auto tv_sr = v_sr(i);
    //Print StandardRecord members
    PrintCAFObjects<caf::StandardRecord>(tv_sr,i);
  }
  timer.stop();
  std::cout<<"Elapsed time (sec) "<<timer.elapsed()<<std::endl;
  //Show the schema of the RNTupleModel
  ntuple->PrintInfo();
}

int main(){
  WriteStandardRecord();
  ReadStandardRecord();

  return 1;
}