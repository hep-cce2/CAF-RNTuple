# for classes_def.xml!!
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

set( PACKAGE duneanaobj_StandardRecord )
FILE( GLOB src_files duneanaobj/duneanaobj/StandardRecord/*.cxx )
FILE( GLOB inc_files duneanaobj/duneanaobj/StandardRecord/*.h )


set (libraries ${libraries}
            ${ROOT_LIBRARIES}
     )
    
#AB: Try to generate dictionary using reflex before generating the library....
REFLEX_GENERATE_DICTIONARY(G__duneanaobj_StandardRecord ${inc_files} SELECTION ${CMAKE_CURRENT_SOURCE_DIR}/duneanaobj/duneanaobj/StandardRecord/classes_def.xml)
#add_library(duneanaobj_StandardRecord ${src_files})
#AB: Instead use the dictionary generated from REFLEX
add_library(duneanaobj_StandardRecord SHARED G__duneanaobj_StandardRecord.cxx ${src_files})
target_link_libraries(duneanaobj_StandardRecord
                 PUBLIC ${libraries})

