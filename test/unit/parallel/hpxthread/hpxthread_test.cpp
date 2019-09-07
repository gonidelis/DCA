// Copyright (C) 2018 ETH Zurich
// Copyright (C) 2018 UT-Battelle, LLC
// All rights reserved.
//
// See LICENSE for terms of usage.
// See CITATION.md for citation guidelines, if DCA++ is used for scientific publications.
//
// Author: Urs R. Haehner (haehneru@itp.phys.ethz.ch)
//         Giovanni Balduzzi (gbalduzz@itp.phys.ethz.ch)
//
// This file tests stdthread.hpp.

//#include "dca/config/haves_defines.hpp"
#include "gtest/gtest.h"
//#ifdef DCA_HAVE_HPX
//    #include <hpx/hpx_init.hpp>
#include <hpx/hpx_main.hpp>
#include <hpx/include/iostreams.hpp>
//#include "dca/parallel/hpx/hpxthread.hpp"
//#endif
//using Threading = dca::parallel::hpxthread;

//TEST(HpxthreadTest, Execute) {
//hpx::cout<<"hello world\n";
//auto routine = [](const int id, const int num_threads, std::vector<int>& vec) {
//    EXPECT_EQ(4, num_threads);
//
//    vec[id] += id;
//};
//Threading threading;

//const int num_threads = 4;
//std::vector<int> vec{0, 10, 20, 30};
//std::vector<int> vec_check{0, 11, 22, 33};
//
//threading.execute(num_threads, routine, std::ref(vec));
//
//EXPECT_EQ(vec_check, vec);
//}
//
//#ifdef DCA_HAVE_HPX
//int hpx_main(int argc, char *argv[]) {
//    ::testing::InitGoogleTest(&argc, argv);
//
//    int result = RUN_ALL_TESTS();
//    hpx::finalize();
//    return result;
//}
//#endif

//TEST(HpxthreadTest, Execute){
//    hpx::cout << "hello world\n";
//}
int hpx_main(int argc, char *argv[]) {
    hpx::cout << "hello world\n";
    ::testing::InitGoogleTest(&argc, argv);
    //
    int result = RUN_ALL_TESTS();
    hpx::finalize();
    return result;
}
int main(int argc, char* argv[])
{
    // Initialize HPX, run hpx_main as the first HPX thread, and
    // wait for hpx::finalize being called.
    return hpx::init(argc, argv);
}
//-----------------------------------------------------------------------------
//int main(int argc, char* argv[])
//{
//    using namespace boost::program_options;

    //    options_description desc_commandline;
//    desc_commandline.add_options()
//        ("dummy", value<boost::uint64_t>()->default_value(10),
//         "Dummy")
//    ;

    // Initialize and run HPX, this example requires to run hpx_main on all
    // localities
//    std::vector<std::string> cfg;
//    cfg.push_back("hpx.run_hpx_main!=1");

    // We force this test to use several threads by default.
//    std::vector<std::string> const cfg = {
//            "hpx.os_threads=all"
//    };
//
//    // Initialize and run HPX
//    return hpx::init(argc, argv, cfg);
//}