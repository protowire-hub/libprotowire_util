
find_package(Boost CONFIG REQUIRED
    COMPONENTS mpl)

add_catch_test(test_lstring SYSTEM_PRIVATE Boost::mpl)
