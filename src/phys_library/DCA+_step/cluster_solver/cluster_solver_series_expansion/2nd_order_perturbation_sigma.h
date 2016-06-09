// Copyright (C) 2009-2016 ETH Zurich
// Copyright (C) 2007?-2016 Center for Nanophase Materials Sciences, ORNL
// All rights reserved.
//
// See LICENSE.txt for terms of usage.
// See CITATION.txt for citation guidelines if you use this code for scientific publications.
//
// Author: Peter Staar (peter.w.j.staar@gmail.com)
//         Urs R. Haehner (haehneru@itp.phys.ethz.ch)
//
// This class computes the self-energy in second order.

#ifndef PHYS_LIBRARY_DCA_STEP_CLUSTER_SOLVER_CLUSTER_SOLVER_SERIES_EXPANSION_2ND_ORDER_PERTURBATION_SIGMA_H
#define PHYS_LIBRARY_DCA_STEP_CLUSTER_SOLVER_CLUSTER_SOLVER_SERIES_EXPANSION_2ND_ORDER_PERTURBATION_SIGMA_H

#include "phys_library/DCA+_step/cluster_solver/cluster_solver_series_expansion/template_perturbation_sigma.h"

#include <complex>
#include <iostream>
#include <utility>

#include "dca/concurrency/parallelization_pthreads.h"
#include "dca/util/print_time.hpp"
#include "comp_library/function_library/include_function_library.h"
#include "phys_library/DCA+_step/cluster_solver/cluster_solver_series_expansion/compute_bare_bubble.h"
#include "phys_library/DCA+_step/cluster_solver/cluster_solver_series_expansion/compute_interaction.h"
#include "phys_library/domains/Quantum_domain/electron_band_domain.h"
#include "phys_library/domains/Quantum_domain/electron_spin_domain.h"
#include "phys_library/domains/time_and_frequency/frequency_domain.h"
#include "phys_library/domains/time_and_frequency/frequency_domain_compact.h"

namespace DCA {
namespace SERIES_EXPANSION {

template <class parameters_type, class k_dmn_t>
class sigma_perturbation<2, parameters_type, k_dmn_t> {
public:
  using concurrency_type = typename parameters_type::concurrency_type;

  using w = dmn_0<frequency_domain>;
  using w_VERTEX_BOSONIC = dmn_0<DCA::vertex_frequency_domain<DCA::EXTENDED_BOSONIC>>;
  using b = dmn_0<electron_band_domain>;
  using s = dmn_0<electron_spin_domain>;
  using nu = dmn_variadic<b, s>;  // orbital-spin index

  using ph_bubble_t = compute_bubble<ph, parameters_type, k_dmn_t, w>;
  // INTERNAL: Shouldn't the template argument be pp instead of pp?
  using pp_bubble_t = compute_bubble<ph, parameters_type, k_dmn_t, w>;

  using chi_function_type = typename ph_bubble_t::function_type;
  using phi_function_type = typename pp_bubble_t::function_type;
  using sp_function_type = FUNC_LIB::function<std::complex<double>, dmn_variadic<nu, nu, k_dmn_t, w>>;
  using U_function_type = typename compute_interaction::function_type;

public:
  sigma_perturbation(parameters_type& parameters_ref, compute_interaction& interaction_obj,
                     compute_bubble<ph, parameters_type, k_dmn_t, w>& chi_obj,
                     compute_bubble<pp, parameters_type, k_dmn_t, w>& phi_obj);

  sp_function_type& get_function() {
    return Sigma;
  }

  void execute_on_cluster(FUNC_LIB::function<std::complex<double>, dmn_variadic<nu, nu, k_dmn_t, w>>& G);
  void threaded_execute_on_cluster(
      FUNC_LIB::function<std::complex<double>, dmn_variadic<nu, nu, k_dmn_t, w>>& G);

  // void execute_on_cluster_2(FUNC_LIB::function<std::complex<double>, dmn_variadic<nu,nu, k_dmn_t,
  // w> >&
  // G);

  template <typename Writer>
  void write(Writer& writer);

private:
  void execute_2A(FUNC_LIB::function<std::complex<double>, dmn_variadic<nu, nu, k_dmn_t, w>>& G);
  void execute_2B(FUNC_LIB::function<std::complex<double>, dmn_variadic<nu, nu, k_dmn_t, w>>& G);

  static void* threaded_execute_2B(void* data);

  int subtract_freq_bf(int, int);  // fermion-boson

protected:
  parameters_type& parameters;
  concurrency_type& concurrency;

  U_function_type& U;

  chi_function_type& chi;
  phi_function_type& phi;

  FUNC_LIB::function<std::complex<double>, dmn_variadic<b, b, k_dmn_t, w_VERTEX_BOSONIC>> U_chi_U;

  sp_function_type Sigma;

  sp_function_type Sigma_2A;
  sp_function_type Sigma_2B;

private:
  struct sigma_perturbation_data {
    U_function_type* U_ptr;

    sp_function_type* G_ptr;
    sp_function_type* S_ptr;

    chi_function_type* chi_ptr;
    phi_function_type* phi_ptr;

    concurrency_type* concurrency_ptr;
  };
};

template <class parameters_type, class k_dmn_t>
sigma_perturbation<2, parameters_type, k_dmn_t>::sigma_perturbation(
    parameters_type& parameters_ref, compute_interaction& interaction_obj,
    compute_bubble<ph, parameters_type, k_dmn_t, w>& chi_obj,
    compute_bubble<pp, parameters_type, k_dmn_t, w>& phi_obj)
    : parameters(parameters_ref),
      concurrency(parameters.get_concurrency()),

      U(interaction_obj.get_function()),

      chi(chi_obj.get_function()),
      phi(phi_obj.get_function()),

      U_chi_U("U-chi-U"),

      Sigma("Sigma-2nd-order"),
      Sigma_2A("Sigma-2nd-order-A"),
      Sigma_2B("Sigma-2nd-order-B") {}

template <class parameters_type, class k_dmn_t>
template <typename Writer>
void sigma_perturbation<2, parameters_type, k_dmn_t>::write(Writer& /*writer*/) {}

template <class parameters_type, class k_dmn_t>
void sigma_perturbation<2, parameters_type, k_dmn_t>::execute_on_cluster(
    FUNC_LIB::function<std::complex<double>, dmn_variadic<nu, nu, k_dmn_t, w>>& G) {
  if (concurrency.id() == 0)
    std::cout << __FUNCTION__ << std::endl;

  // cout << "\t U : " << U(0,0,0,1) << endl;

  sigma_perturbation<2, parameters_type, k_dmn_t>::execute_2B(G);

  // Sigma_2A *= 1.;
  // Sigma_2B *= 2.;

  Sigma = 0.;
  // Sigma += Sigma_2A;
  Sigma += Sigma_2B;

  if (true) {
    std::complex<double> I(0, 1);
    for (int b_ind = 0; b_ind < 2 * b::dmn_size(); ++b_ind) {
      for (int k_ind = 0; k_ind < k_dmn_t::dmn_size(); ++k_ind) {
        int wc_ind = w::dmn_size() / 8;

        double wc = w::get_elements()[wc_ind];

        std::complex<double> Sigma_wc = Sigma(b_ind, b_ind, k_ind, wc_ind);

        double alpha = real(Sigma_wc);
        double beta = imag(Sigma_wc * wc);

        for (int w_ind = 0; w_ind < wc_ind; ++w_ind) {
          Sigma(b_ind, b_ind, k_ind, w_ind) = alpha + beta * I / w::get_elements()[w_ind];
          Sigma(b_ind, b_ind, k_ind, w::dmn_size() - 1 - w_ind) =
              alpha - beta * I / w::get_elements()[w_ind];
        }
      }
    }
  }
}

template <class parameters_type, class k_dmn_t>
void sigma_perturbation<2, parameters_type, k_dmn_t>::execute_2A(
    FUNC_LIB::function<std::complex<double>, dmn_variadic<nu, nu, k_dmn_t, w>>& G) {
  if (concurrency.id() == 0)
    std::cout << __FUNCTION__ << std::endl;

  double U_value = U(0, 0, 0, 1);

  Sigma_2A = 0.;

  for (int nu1_ind = 0; nu1_ind < w_VERTEX_BOSONIC::dmn_size(); ++nu1_ind) {
    for (int q1_ind = 0; q1_ind < k_dmn_t::dmn_size(); ++q1_ind) {
      for (int nu2_ind = 0; nu2_ind < w_VERTEX_BOSONIC::dmn_size(); ++nu2_ind) {
        for (int q2_ind = 0; q2_ind < k_dmn_t::dmn_size(); ++q2_ind) {
          for (int w_ind = 0; w_ind < w::dmn_size(); ++w_ind) {
            int w_minus_nu1 = subtract_freq_bf(nu1_ind, w_ind);
            int w_minus_nu2 = subtract_freq_bf(nu2_ind, w_ind);
            int w_minus_nu1_minus_nu2 = subtract_freq_bf(nu2_ind, w_minus_nu1);

            if (w_minus_nu1 < 0 || w_minus_nu1 >= w::dmn_size() || w_minus_nu2 < 0 ||
                w_minus_nu2 >= w::dmn_size() || w_minus_nu1_minus_nu2 < 0 ||
                w_minus_nu1_minus_nu2 >= w::dmn_size())
              continue;

            for (int k_ind = 0; k_ind < k_dmn_t::dmn_size(); ++k_ind) {
              int k_minus_q1 = k_dmn_t::parameters_type::subtract(q1_ind, k_ind);
              int k_minus_q2 = k_dmn_t::parameters_type::subtract(q2_ind, k_ind);
              int k_minus_q1_minus_q2 = k_dmn_t::parameters_type::subtract(q2_ind, k_minus_q1);

              Sigma_2A(0, 0, 0, 0, k_ind, w_ind) +=
                  G(0, 0, 0, 0, k_minus_q1, w_minus_nu1) * G(0, 0, 0, 0, k_minus_q2, w_minus_nu2) *
                  G(0, 0, 0, 0, k_minus_q1_minus_q2, w_minus_nu1_minus_nu2);
            }
          }
        }
      }
    }
  }

  for (int w_ind = 0; w_ind < w::dmn_size(); ++w_ind)
    for (int k_ind = 0; k_ind < k_dmn_t::dmn_size(); ++k_ind)
      Sigma_2A(0, 1, 0, 1, k_ind, w_ind) = Sigma_2A(0, 0, 0, 0, k_ind, w_ind);

  double factor = 1. / (parameters.get_beta() * parameters.get_beta() * k_dmn_t::dmn_size() *
                        k_dmn_t::dmn_size()) *
                  U_value * U_value;
  Sigma_2A *= factor;
}

template <class parameters_type, class k_dmn_t>
void sigma_perturbation<2, parameters_type, k_dmn_t>::execute_2B(
    FUNC_LIB::function<std::complex<double>, dmn_variadic<nu, nu, k_dmn_t, w>>& G) {
  if (concurrency.id() == 0)
    std::cout << __FUNCTION__ << std::endl;

  double U_value = U(0, 0, 0, 1);

  Sigma_2B = 0.;

  for (int q_ind = 0; q_ind < k_dmn_t::dmn_size(); ++q_ind) {
    for (int k_ind = 0; k_ind < k_dmn_t::dmn_size(); ++k_ind) {
      int k_minus_q = k_dmn_t::parameters_type::subtract(q_ind, k_ind);

      for (int nu_ind = 0; nu_ind < w_VERTEX_BOSONIC::dmn_size(); ++nu_ind) {
        int nu_c = (nu_ind - w_VERTEX_BOSONIC::dmn_size() / 2);

        for (int w_ind = std::fabs(nu_c); w_ind < w::dmn_size() - std::fabs(nu_c); ++w_ind) {
          int w_minus_nu = w_ind - nu_c;

          Sigma_2B(0, 0, 0, 0, k_ind, w_ind) +=
              G(0, 0, 0, 0, k_minus_q, w_minus_nu) * chi(0, 0, 0, 0, q_ind, nu_ind);
        }
      }
    }
  }

  for (int w_ind = 0; w_ind < w::dmn_size(); ++w_ind)
    for (int k_ind = 0; k_ind < k_dmn_t::dmn_size(); ++k_ind)
      Sigma_2B(0, 1, 0, 1, k_ind, w_ind) = Sigma_2B(0, 0, 0, 0, k_ind, w_ind);

  double factor = 1. / (parameters.get_beta() * k_dmn_t::dmn_size()) * U_value * U_value;
  Sigma_2B *= factor;
}

template <class parameters_type, class k_dmn_t>
void sigma_perturbation<2, parameters_type, k_dmn_t>::threaded_execute_on_cluster(
    FUNC_LIB::function<std::complex<double>, dmn_variadic<nu, nu, k_dmn_t, w>>& G) {
  if (concurrency.id() == 0)
    std::cout << "\n\n\t\t second-order Self-energy \n\n" << std::endl;

  //       std::cout << "\t U : " << U(0,0,0,1) << std::endl;

  int nr_threads = parameters.get_nr_HTS_threads();

  sigma_perturbation_data args;
  {
    args.U_ptr = &U;

    args.G_ptr = &G;
    args.S_ptr = &Sigma_2B;

    args.chi_ptr = &chi;
    args.phi_ptr = &phi;

    args.concurrency_ptr = &concurrency;
  }

  {
    dca::concurrency::parallelization<dca::concurrency::POSIX_LIBRARY> pthreads;

    pthreads.execute(nr_threads, threaded_execute_2B, (void*)&args);
  }

  {
    double U_value = U(0, 0, 0, 1);
    double factor = 1. / (parameters.get_beta() * k_dmn_t::dmn_size()) * U_value * U_value;
    Sigma_2B *= factor;

    concurrency.sum(Sigma_2B);
  }

  Sigma = 0.;
  Sigma += Sigma_2B;

  if (true) {
    std::complex<double> I(0, 1);
    for (int b_ind = 0; b_ind < 2 * b::dmn_size(); ++b_ind) {
      for (int k_ind = 0; k_ind < k_dmn_t::dmn_size(); ++k_ind) {
        int wc_ind = w::dmn_size() / 8;

        double wc = w::get_elements()[wc_ind];

        std::complex<double> Sigma_wc = Sigma(b_ind, b_ind, k_ind, wc_ind);

        double alpha = real(Sigma_wc);
        double beta = imag(Sigma_wc * wc);

        for (int w_ind = 0; w_ind < wc_ind; ++w_ind) {
          Sigma(b_ind, b_ind, k_ind, w_ind) = alpha + beta * I / w::get_elements()[w_ind];
          Sigma(b_ind, b_ind, k_ind, w::dmn_size() - 1 - w_ind) =
              alpha - beta * I / w::get_elements()[w_ind];
        }
      }
    }
  }
}

template <class parameters_type, class k_dmn_t>
void* sigma_perturbation<2, parameters_type, k_dmn_t>::threaded_execute_2B(void* void_ptr) {
  dca::concurrency::posix_data* data_ptr = static_cast<dca::concurrency::posix_data*>(void_ptr);
  sigma_perturbation_data* sigma_pert_ptr = static_cast<sigma_perturbation_data*>(data_ptr->args);

  // U_function_type&   U   = *(sigma_pert_ptr->U_ptr);

  sp_function_type& G = *(sigma_pert_ptr->G_ptr);
  sp_function_type& S = *(sigma_pert_ptr->S_ptr);
  chi_function_type& chi = *(sigma_pert_ptr->chi_ptr);

  concurrency_type& concurrency = *(sigma_pert_ptr->concurrency_ptr);

  k_dmn_t q_dmn;
  std::pair<int, int> q_bounds = concurrency.get_bounds(q_dmn);

  int id = data_ptr->id;
  int nr_threads = data_ptr->nr_threads;

  k_dmn_t k_dmn;
  std::pair<int, int> k_bounds =
      dca::concurrency::parallelization<dca::concurrency::POSIX_LIBRARY>::get_bounds(id, nr_threads,
                                                                                     k_dmn);

  for (int k_ind = k_bounds.first; k_ind < k_bounds.second; k_ind++) {
    double percentage = double(k_ind - k_bounds.first) / double(k_bounds.second - k_bounds.first);

    if (concurrency.id() == 0 and id == 0 and (int(100 * percentage) % 10 == 0))
      std::cout << "\t" << int(100 * percentage) << " % finished\t" << dca::util::print_time()
                << "\n";

    for (int q_ind = q_bounds.first; q_ind < q_bounds.second; q_ind++) {
      int k_minus_q = k_dmn_t::parameter_type::subtract(q_ind, k_ind);

      for (int nu_ind = 0; nu_ind < w_VERTEX_BOSONIC::dmn_size(); ++nu_ind) {
        int nu_c = (nu_ind - w_VERTEX_BOSONIC::dmn_size() / 2);

        for (int w_ind = std::fabs(nu_c); w_ind < w::dmn_size() - std::fabs(nu_c); ++w_ind) {
          int w_minus_nu = w_ind - nu_c;

          S(0, 0, 0, 0, k_ind, w_ind) +=
              G(0, 0, 0, 0, k_minus_q, w_minus_nu) * chi(0, 0, 0, 0, q_ind, nu_ind);  //*factor;
        }
      }
    }

    for (int w_ind = 0; w_ind < w::dmn_size(); ++w_ind)
      S(0, 1, 0, 1, k_ind, w_ind) = S(0, 0, 0, 0, k_ind, w_ind);
  }

  //       for(int w_ind=0; w_ind<w::dmn_size(); ++w_ind)
  // 	for(int k_ind=0; k_ind<k_dmn_t::dmn_size(); ++k_ind)
  // 	  S(0,1, 0,1, k_ind, w_ind) = S(0,0, 0,0, k_ind, w_ind);

  //       Sigma_2B *= factor;

  return 0;
}

template <class parameters_type, class k_dmn_t>
int sigma_perturbation<2, parameters_type, k_dmn_t>::subtract_freq_bf(int w1, int w2) {
  int w_f = 2 * (w2 - w::dmn_size() / 2) + 1;             // transform fermionic
  int w_b = 2 * (w1 - w_VERTEX_BOSONIC::dmn_size() / 2);  // transform bosonic
  int res = ((w_f - w_b) - 1 + w::dmn_size()) / 2;        // result is fermionic
  return res;
}
}
}

#endif  // PHYS_LIBRARY_DCA_STEP_CLUSTER_SOLVER_CLUSTER_SOLVER_SERIES_EXPANSION_2ND_ORDER_PERTURBATION_SIGMA_H
