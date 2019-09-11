// Copyright (C) 2018 ETH Zurich
// Copyright (C) 2018 UT-Battelle, LLC
// All rights reserved.
//
// See LICENSE for terms of usage.
// See CITATION.md for citation guidelines, if DCA++ is used for scientific publications.
//
// Author: Peter Staar (taa@zurich.ibm.com)
//         Urs R. Haehner (haehneru@itp.phys.ethz.ch)
//         Giovanni Balduzzi (gbalduzz@itp.phys.ethz.ch)
//
// This class provides an interface for parallelizing using a pool of STL threads.

#ifndef DCA_PARALLEL_HPXTHREAD_HPXTHREAD_HPP
#define DCA_PARALLEL_HPXTHREAD_HPXTHREAD_HPP

#include <cassert>
#include <iostream>
#include <vector>

//#include "dca/parallel/hpx/hpx_thread_pool/thread_pool.hpp"
#include "hpx/include/async.hpp"
#include "hpx/include/threads.hpp"
#include "hpx/lcos/local/mutex.hpp"
#include "hpx/lcos/local/condition_variable.hpp"
#include "hpx/lcos/local/packaged_task.hpp"

namespace dca {
    namespace parallel {

        class hpxthread {
        public:
            hpxthread() = default;

            // Executes the function f(id, num_tasks, args...) for each integer value of id in [0, num_tasks).
            template <class F, class... Args>
            void execute(int num_threads, F&& f, Args&&... args) {
                assert(num_threads > 0);

                std::vector<hpx::future<void>> futures;
//                auto& pool = HPXThreadPool::get_instance();
//                pool.enlarge(num_threads);

                // Fork.
                for (int id = 0; id < num_threads; ++id)
                    futures.emplace_back(
                            hpx::async(std::forward<F>(f), id, num_threads, std::forward<Args>(args)...));
                // Join.
                for (auto& future : futures)
                    future.get();
            }

            // Returns the sum of the return values of f(id, num_tasks, args...) for each integer value of id
            // in [0, num_tasks).
            // Precondition: the return type of f can be initialized with 0.
            // TODO: ONE PLACE
            template <class F, class... Args>
            auto sumReduction(int num_threads, F&& f, Args&&... args) {
                assert(num_threads > 0);

                using ReturnType = typename std::result_of<F(int, int, Args...)>::type;

                std::vector<hpx::future<ReturnType>> futures;
//                auto& pool = HPXThreadPool::get_instance();
//                pool.enlarge(num_threads);

                // Spawn num_threads tasks.
                for (int id = 0; id < num_threads; ++id)
                    futures.emplace_back(
                            hpx::async(std::forward<F>(f), id, num_threads, std::forward<Args>(args)...));
                // Sum the result of the tasks.
                ReturnType result = 0;
                for (auto& future : futures)
                    result += future.get();

                return result;
            }

//            friend std::ostream& operator<<(std::ostream& some_ostream, const hpxthread& this_concurrency);

        private:
            static constexpr char parallel_type_str_[] = "stdthread";
        };

    }  //  parallel
}  //  dca

#endif  // DCA_PARALLEL_HPXDTHREAD_HPXTHREAD_HPP
