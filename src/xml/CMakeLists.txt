
add_definitions("-std=c++11")

# Find xml libraries
find_library(XML_LIB xml2 $ENV{LIBXML_LIB_PATH})

add_library(HybridNode HybridNode.cxx)
target_link_libraries(HybridNode ${XML_LIB})

add_library(FebNode FebNode.cxx)
target_link_libraries(FebNode 
    ${XML_LIB}
    HybridNode
)

add_library(SvtXmlWriter SvtXmlWriter.cxx)
target_link_libraries(SvtXmlWriter 
    ${XML_LIB}
)


add_library(CalibrationWriter CalibrationWriter.cxx)
target_link_libraries(CalibrationWriter 
    SvtXmlWriter
    FebNode
)

add_library(ThresholdWriter ThresholdWriter.cxx)
target_link_libraries(ThresholdWriter SvtXmlWriter)

add_library(SvtXmlReader SvtXmlReader.cxx)
target_link_libraries(SvtXmlReader 
    ${XML_LIB}
)

add_library(ThresholdReader ThresholdReader.cxx)
target_link_libraries(ThresholdReader 
    SvtXmlReader    
)
