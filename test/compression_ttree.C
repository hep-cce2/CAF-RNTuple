#include <TROOT.h>
#include <TTree.h>
#include <TFile.h>

//Idea is to read the existing flat CAF in TTree and write back with different compression algorithm and compression level.

constexpr char const *container_name = "cafTree";
constexpr char const *file_name = "merged_cafTree.root";

//compression info
//kZLIB, kLZMA, kLZ4, kZSTD
auto compressionAlgorithm = ROOT::kZSTD;
int compressionLevel = 5;

//output filename 
constexpr char const* outname = "merged_FlatcafTree_kZSTD_5.root";

void compress(){
  TFile* infile = new TFile(file_name,"READ");
  TTree* incaf = infile->Get<TTree>(container_name);
  incaf->SetBranchStatus("*",1);

  auto nentries = incaf->GetEntries();
  TFile* outfile = new TFile(outname,"RECREATE","",ROOT::CompressionSettings(compressionAlgorithm,compressionLevel));
  TTree* outcaf = incaf->CloneTree();
  outfile->Close();
  std::cout<<"Wrote Total Entries "<<nentries<<std::endl;
}


void read(){
  TFile *file = TFile::Open(outname);

  auto compress_factor = file->GetCompressionFactor();
  auto compress_alg = file->GetCompressionAlgorithm();
  auto comp_level = file->GetCompressionLevel();
  
  std::cout<<"Printing Compression Information "<<std::endl;
  std::cout<<"Alg "<<compress_alg<<" Level "<<comp_level<<" Factor "<<compress_factor<<std::endl;
  
}

void compression_ttree(){
 compress();
 read();
}