/* -*- c++ -*- */
/*
 * Copyright 2026 University of Toronto Aerospace Team.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_BER_ASYNC_BER_CALCULATOR_H
#define INCLUDED_BER_ASYNC_BER_CALCULATOR_H

#include <gnuradio/ber/api.h>
#include <gnuradio/hier_block2.h>

namespace gr {
  namespace ber {

    /*!
     * \brief <+description of block+>
     * \ingroup ber
     *
     */
    class BER_API async_ber_calculator : virtual public gr::hier_block2
    {
     public:
      typedef std::shared_ptr<async_ber_calculator> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of ber::async_ber_calculator.
       *
       * To avoid accidental use of raw pointers, ber::async_ber_calculator's
       * constructor is in a private implementation
       * class. ber::async_ber_calculator::make is the public interface for
       * creating new instances.
       */
      static sptr make(std::string sequence);
    };

  } // namespace ber
} // namespace gr

#endif /* INCLUDED_BER_ASYNC_BER_CALCULATOR_H */
