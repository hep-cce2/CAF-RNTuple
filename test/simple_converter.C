#include <TFile.h>
#include <TTree.h>
#include <ROOT/RNTupleImporter.hxx>
#include <ROOT/RNTuple.hxx>

#include <vector>


void WriteIntoRNTuple(){
    TFile *file = new TFile("../output.caf","read");
    
    TTree *fSourceTree = file->Get<TTree>("cafTree");
    auto importer = RNTupleImporter::Create(fSourceTree, "output.root");
    importer->SetConvertDotsInBranchNames(true);
    importer->SetIsQuiet(false);
    importer->Import();
    
}

void ReadRNTuple(){
  //auto model = RNTupleModel::Create();
  
  auto ntuple = RNTupleReader::Open("cafTree","output.root");
  auto nfields = ntuple->GetDescriptor().GetNFields();
  ntuple->PrintInfo(ENTupleInfo::kStorageDetails);
  
}
void simple_converter() {
  WriteIntoRNTuple();
  ReadRNTuple();
}