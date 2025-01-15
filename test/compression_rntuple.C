#include <ROOT/RNTuple.hxx>
#include <ROOT/RNTupleModel.hxx>
#include <ROOT/RDataFrame.hxx>
#include <ROOT/RNTupleWriteOptions.hxx>
#include <ROOT/RNTupleImporter.hxx>

#include <vector>

//This script takes existing Flat CAF in TTree and 
//converts into RNTuple using user specified compression setting.

using namespace ROOT::Experimental;
constexpr char const* container_name = "cafTree";
constexpr char const* file_name = "merged_cafTree.root";
//compression info
//kZLIB; kLZMA, kLZ4, kZSTD
auto compressionAlgorithm = ROOT::RCompressionSetting::EAlgorithm::kZSTD;
int compressionLevel = 5;

constexpr char const* out_filename = "/tmp/CAFTEST/merged_FlatcafRNTuple_ZSTD_5.root";

//Read RNTuple and Write back
void ImportRNTuple(){
  
  TFile *file = new TFile(file_name,"read");  
  TTree *fSourceTree = file->Get<TTree>(container_name);
  
  //now the compression settings....
  ROOT::Experimental::RNTupleWriteOptions options;
  options.SetCompression(compressionAlgorithm,compressionLevel);
  
  auto importer = RNTupleImporter::Create(fSourceTree,out_filename);
  importer->SetWriteOptions(options);
  importer->SetConvertDotsInBranchNames(true);
  importer->SetIsQuiet(false);
  importer->Import();

  
}

void ReadRNTuple(){

  
  TFile *file = TFile::Open(out_filename);
  auto compress_factor = file->GetCompressionFactor();
  auto compress_alg = file->GetCompressionAlgorithm();
  auto comp_level = file->GetCompressionLevel();
  
  std::cout<<"Printing Compression Information "<<std::endl;
  std::cout<<"Alg "<<compress_alg<<" Level "<<comp_level<<" Factor "<<compress_factor<<std::endl;
  
}

void compression_rntuple(){
  ImportRNTuple();
  ReadRNTuple();
}