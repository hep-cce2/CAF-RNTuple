find_package(Threads REQUIRED) #This is explicitly required for the master version of ROOT not sure why
set (libraries ${libraries} 
            ${ROOT_LIBRARIES}
            ${DUNEANAOBJ_LIB} 
            ROOT::Core
            ROOT::RIO
            ROOT::Tree
            ROOT::Hist
            )
            
add_executable(Test_CAFTree Test_CAFTree.cxx)
target_link_libraries(Test_CAFTree PRIVATE ${libraries})

add_executable(Test_CAFRNTuple Test_CAFRNTuple.cxx)
target_link_libraries(Test_CAFRNTuple PRIVATE ${libraries} ROOT::ROOTNTuple ROOT::ROOTNTupleUtil)

