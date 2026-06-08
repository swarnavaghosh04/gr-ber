#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2026 University of Toronto Aerospace Team.
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

from gnuradio import gr, gr_unittest
from gnuradio import blocks
try:
    from gnuradio.ber import async_ber_calculator
except ImportError:
    import os
    import sys
    dirname, filename = os.path.split(os.path.abspath(__file__))
    sys.path.append(os.path.join(dirname, "bindings"))
    from gnuradio.ber import async_ber_calculator

class qa_async_ber_calculator(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None

    def test_instance(self):
        instance = async_ber_calculator("010110010101101011")

    def test_001(self):

        sequence  = "0001000101011100101001111010011110101011010110001111010101001111011101110110101"
        err_mask  = "0000000000000000000000000001000000000010000000000000010000000100000000100000000"
        seq_repeat = 3

        err_sequence = [ int(sequence[i])^int(err_mask[i]) for i in range(len(sequence)) ]
        n_errs = err_mask.count('1')
        expected_ber = n_errs/len(sequence)

        source    = blocks.vector_source_b(err_sequence*seq_repeat, False, 1, [])
        ber_calc  = async_ber_calculator(sequence)
        ber_sink  = blocks.vector_sink_f()
        corr_sink = blocks.vector_sink_f()

        self.tb.connect((source  , 0), (ber_calc , 0))
        self.tb.connect((ber_calc, 0), (ber_sink , 0))
        self.tb.connect((ber_calc, 1), (corr_sink, 0))
        
        self.tb.run()
        
        result = ber_sink.data()
        self.assertGreater(len(result), len(sequence)*(seq_repeat-1))
        self.assertTrue(all( abs(x-expected_ber) < 0.0000001 for x in result[-len(sequence):]))
        


if __name__ == '__main__':
    gr_unittest.run(qa_async_ber_calculator)
