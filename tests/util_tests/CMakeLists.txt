
find_package(Boost CONFIG REQUIRED COMPONENTS mpl nowide preprocessor)

add_catch_test(test_lstring SYSTEM_PRIVATE Boost::mpl)


