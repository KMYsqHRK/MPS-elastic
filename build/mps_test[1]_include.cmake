if(EXISTS "/home/l/MPS-elastic/build/mps_test[1]_tests.cmake")
  include("/home/l/MPS-elastic/build/mps_test[1]_tests.cmake")
else()
  add_test(mps_test_NOT_BUILT mps_test_NOT_BUILT)
endif()
