# Test case for each version of C++ supported
foreach(X 11 14 17 20 23)
  add_executable("test_cxx_${X}" "cxx_std_${X}.test.cpp")
  target_link_libraries("test_cxx_${X}" PRIVATE integral_types)
  target_compile_features("test_cxx_${X}" PRIVATE "cxx_std_${X}")
  set_target_properties("test_cxx_${X}" PROPERTIES CXX_EXTENSIONS OFF)
  add_test(NAME "test_cxx_${X}" COMMAND "test_cxx_${X}")
endforeach()
