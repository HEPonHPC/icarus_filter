add_library(ProductSizeFilter_module SHARED ProductSizeFilter_module.cc)
target_link_libraries(ProductSizeFilter_module PRIVATE ROOT::Core)
target_include_directories(ProductSizeFilter_module PRIVATE  $ENV{ROOT_INC} $ENV{ART_INC} $ENV{LARDATAOBJ_INC} $ENV{LARCOREOBJ_INC} ${CMAKE_CURRENT_BINARY_DIR})
