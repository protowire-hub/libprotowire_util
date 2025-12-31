
find_package(Boost CONFIG REQUIRED COMPONENTS
  preprocessor nowide)

add_catch_test(test_type_repr SYSTEM_PRIVATE Boost::preprocessor)

add_catch_test(test_object_repr SYSTEM_PRIVATE Boost::nowide)
