/* -*- c++ -*- */
/*
 * Copyright 2026 University of Toronto Aerospace Team.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <algorithm>
#include <gnuradio/io_signature.h>
#include "async_ber_calculator_impl.h"

namespace gr {
  namespace ber {

    after_correlation_processor::sptr
    after_correlation_processor::make(int sequence_length){
      return gnuradio::make_block_sptr<after_correlation_processor>(sequence_length);
    }

    after_correlation_processor::after_correlation_processor(int sequence_length)
    : gr::sync_block("after_correlation_processor",
                      gr::io_signature::make(2, 2, sizeof(gr_complex)),
                      gr::io_signature::make(2, 2, sizeof(float))),
      d_sequence_length{sequence_length}
    {
      set_history(d_sequence_length);
      set_tag_propagation_policy(gr::block::TPP_DONT);
    }

    after_correlation_processor::~after_correlation_processor()
    {
      // nothing
    }


    int after_correlation_processor::work(int noutput_items, gr_vector_const_void_star& input_items, gr_vector_void_star& output_items){
      const gr_complex* in_corr = static_cast<const gr_complex*>(input_items[1]);
      float* out_ber = static_cast<float*>(output_items[0]);
      float* out_corr = static_cast<float*>(output_items[1]);
      int new_samples_start = history() - 1;
      int n_consume = noutput_items < d_sequence_length ? noutput_items : d_sequence_length;
      int window_end = new_samples_start + n_consume;
      int window_start = window_end - d_sequence_length;
      float max = std::max_element(in_corr+window_start, in_corr+window_end, [](const gr_complex& a, const gr_complex& b){return a.real() < b.real();})->real();
      float ber = float(d_sequence_length-max)/2.f/d_sequence_length;
      for(int i = new_samples_start; i < window_end; i++){
        float r = in_corr[i].real();
        out_ber[i-new_samples_start] = ber;
        out_corr[i-new_samples_start] = float(d_sequence_length-r)/2.f/d_sequence_length;
      }
      return n_consume;
    }

    using input_type = unsigned char;
    using output_type = float;
    async_ber_calculator::sptr
    async_ber_calculator::make(std::string sequence)
    {
      return gnuradio::make_block_sptr<async_ber_calculator_impl>(sequence);
    }


    /*
     * The private constructor
     */
    async_ber_calculator_impl::async_ber_calculator_impl(std::string sequence)
      : gr::hier_block2("async_ber_calculator",
              gr::io_signature::make(1, 1, sizeof(input_type)),
              gr::io_signature::make(2, 2, sizeof(output_type)))
    {
      std::vector<gr_complex> seq_nrzi_c;
      seq_nrzi_c.reserve(sequence.length());
      for(const char& c : sequence){
        seq_nrzi_c.emplace_back( static_cast<float>(c-'0')*2.f-1.f, 0 );
      }

      d_chunks_to_symbols = gr::digital::chunks_to_symbols<unsigned char, gr_complex>::make(std::vector({gr_complex(-1,0), gr_complex(1,0)}), 1);
      d_corr_est = gr::digital::corr_est_cc::make(seq_nrzi_c, 1, 0);
      d_after_correlation_processor = after_correlation_processor::make(sequence.length());

      connect(self(), 0, d_chunks_to_symbols, 0);
      connect(d_chunks_to_symbols, 0, d_corr_est, 0);
      connect(d_corr_est, 0, d_after_correlation_processor, 0);
      connect(d_corr_est, 1, d_after_correlation_processor, 1);
      connect(d_after_correlation_processor, 0, self(), 0);
      connect(d_after_correlation_processor, 1, self(), 1);
    }

    /*
     * Our virtual destructor.
     */
    async_ber_calculator_impl::~async_ber_calculator_impl()
    {
    }


  } /* namespace ber */
} /* namespace gr */
