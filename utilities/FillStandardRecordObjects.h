#include "StandardRecordObjects.h"
#include <chrono> //Header for time calculation common to both TTree and RNTuple

//Utility function to fill SRObjects
void FillSRObjects(std::shared_ptr<caf::StandardRecord>&sr)
{
    //Fill beam Objects
    FillSRBeamObject(sr);

    //Fill the truth object...
    FillSRTruthBranchObject(sr);

    //Fill the detector meta branch..
    FillSRDetectorMetaBranch(sr->meta);

    //FillSRTruthObject
    FillSRTruthBranchObject(sr);
    
    //Fill the SRCommonRecoBranch...
    FillSRCommonRecoBranchObject(sr);

    //Fill the SRFDBranch....
    FillSRFDBranch(sr);
}

//Utility to measure time taken for certain task
struct TimerUtil
{
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    std::chrono::time_point<std::chrono::high_resolution_clock> end_time;
    
   // Start the timer
    void start() {
        start_time = std::chrono::high_resolution_clock::now();
    }

    // Stop the timer
    void stop() {
        end_time = std::chrono::high_resolution_clock::now();
    }

    // Get elapsed time in seconds
    double elapsed() const {
        auto elapsed_duration = end_time - start_time;
        return std::chrono::duration<double>(elapsed_duration).count();
    }

};

//Utility function to print compression information from a TFile
void PrintCompressionInfo(const char *fname){
  TFile *file = TFile::Open(fname);
  auto compress_factor = file->GetCompressionFactor();
  auto compress_alg = file->GetCompressionAlgorithm();
  auto comp_level = file->GetCompressionLevel();
  
  std::cout<<"Printing Compression Information "<<std::endl;
  std::cout<<"Alg "<<compress_alg<<" Level "<<comp_level<<" Factor "<<compress_factor<<std::endl;

}

//Utility function to print CAF objects
template<typename T>
void PrintCAFObjects(const T& sr_obj, int index){
    caf::SRInteractionBranch ixn = sr_obj.common.ixn;
    int nnu = sr_obj.mc.nnu;
    std::vector<caf::SRInteraction> dlp = ixn.dlp;
    std::size_t dlp_size = ixn.ndlp;
    if(index<10){
        std::cout<<"nnu "<<nnu<<" dlp_size "<<dlp_size<<std::endl;
    }
}