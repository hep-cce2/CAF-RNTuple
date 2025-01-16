# CAF-RNTuple

This is a standalone test framework to test the persistence of CAF objects in RNTuple. Please follow the instructions below to build and run this code:

# Requirements

1. Install ROOT (master version). Note that RNTuple is still in development (as of this writing). 
2. CMakeLists (version > 3.20)
3. duneanaobj (https://github.com/DUNE/duneanaobj.git) which has CAF objects (or Standard Record Objects). 

# Minimal Installation of the Package with external packages (duneanaobj)

```bash
git clone --recurse-submodules https://github.com/hep-cce2/CAF-RNTuple.git
cd CAF-RNTuple
mkdir build
cd build
cmake -DCMAKE_MODULE_PATH=/path/to/ROOTConfig.cmake ..
make -j$(nproc)
```
This will download CAF test framework and the *duneanaobj* package. 

## CAF-RNTuple 

CAF-RNTuple is the package that allows to test the persistence of CAF objects in RNTuple (and TTree). It depends upon the standalone duneanaobj build. 


**CMakeLists.txt** Assumes that the environment variables are setup according to setup_dunecaf.sh

## CAF-RNTuple/test

This directory contains the test codes for CAF object persistence in TTree and RNTuple

**Test_CAF.cxx** CAF Objects persist in **TTree**

**Test_CAFSimple** CAF Objects persist in **RNTuple**

**simple_converter.C** FlatCAF written in TTree are written into RNTuple using **RNTupleConverter**



## CAF-RNTuple/utilities

**StandardRecordObjects** 
This function basically fills each and every StandardRecord members. The APIs here are called by test codes in **CAF-RNTuple/test**

**FillStandardRecordObjects** 
This function fills the members of StandardRecord in TTree or RNTuple by calling the StandardRecordObjects. This goes inside the event loop of both TTree and RNTuple.

**CAF-RNTuple/external**
Git will download the duneanaobj and build the library here. 

