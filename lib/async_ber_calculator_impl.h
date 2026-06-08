/* -*- c++ -*- */
/*
 * Copyright 2026 University of Toronto Aerospace Team.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_BER_ASYNC_BER_CALCULATOR_IMPL_H
#define INCLUDED_BER_ASYNC_BER_CALCULATOR_IMPL_H

#include <gnuradio/digital/chunks_to_symbols.h>
#include <gnuradio/digital/corr_est_cc.h>
// #include <gnuradio/blocks/null_sink.h>
#include <gnuradio/blocks/complex_to_real.h>
#include <gnuradio/ber/async_ber_calculator.h>

namespace gr {
  namespace ber {

    class after_correlation_processor : public gr::sync_block
    {
     private:
      int d_sequence_length;
     public:
      typedef std::shared_ptr<after_correlation_processor> sptr;
      static sptr make(int sequence_length);
      after_correlation_processor(int sequence_length);
      ~after_correlation_processor();
      int work(int noutput_items,
               gr_vector_const_void_star& input_items,
               gr_vector_void_star& output_items) override;
    };

    class async_ber_calculator_impl : public async_ber_calculator
    {
     private:
      gr::digital::chunks_to_symbols<unsigned char, gr_complex>::sptr d_chunks_to_symbols;
      gr::digital::corr_est_cc::sptr d_corr_est;
      // gr::blocks::null_sink::sptr d_null_sink;
      after_correlation_processor::sptr d_after_correlation_processor;


     public:
      async_ber_calculator_impl(std::string sequence);
      ~async_ber_calculator_impl();

      // Where all the action really happens
    };

  } // namespace ber
} // namespace gr

#endif /* INCLUDED_BER_ASYNC_BER_CALCULATOR_IMPL_H */
