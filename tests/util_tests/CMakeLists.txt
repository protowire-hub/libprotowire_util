
find_package(Boost CONFIG REQUIRED COMPONENTS mpl nowide preprocessor)

add_catch_test(test_lstring SYSTEM_PRIVATE Boost::mpl)

add_catch_test(test_type_repr SYSTEM_PRIVATE Boost::preprocessor)

add_catch_test(test_object_repr SYSTEM_PRIVATE Boost::nowide)

