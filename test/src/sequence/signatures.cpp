/*
    Genesis - A toolkit for working with phylogenetic data.
    Copyright (C) 2014-2017 Lucas Czech

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Contact:
    Lucas Czech <lucas.czech@h-its.org>
    Exelixis Lab, Heidelberg Institute for Theoretical Studies
    Schloss-Wolfsbrunnenweg 35, D-69118 Heidelberg, Germany
*/

/**
 * @brief
 *
 * @file
 * @ingroup test
 */

#include "src/common.hpp"

#include "genesis/sequence/formats/fasta_reader.hpp"
#include "genesis/sequence/functions/functions.hpp"
#include "genesis/sequence/functions/signatures.hpp"
#include "genesis/sequence/sequence_set.hpp"
#include "genesis/sequence/sequence.hpp"

#include "genesis/utils/math/common.hpp"
#include "genesis/utils/text/string.hpp"

#include <algorithm>
#include <numeric>
#include <string>

using namespace genesis::sequence;
using namespace genesis::utils;

TEST( Sequence, KmerList )
{
    // Test up to kmer size of 6
    for( size_t k = 1; k < 6; ++k ) {
        auto list = kmer_list( k, "ACGT" );
        EXPECT_EQ( genesis::utils::int_pow( 4, k ), list.size() );
        EXPECT_EQ( std::string( k, 'A' ), list.front() );
        EXPECT_EQ( std::string( k, 'T' ), list.back() );

        // for( auto ll : list ) {
        //     LOG_DBG << ll;
        // }
    }
}

TEST( Sequence, KmerCounts )
{
    // Skip test if no data availabe.
    NEEDS_TEST_DATA;

    // Load sequence file.
    std::string infile = environment->data_dir + "sequence/dna_10.fasta";
    SequenceSet sset;
    FastaReader().from_file(infile, sset);

    remove_all_gaps( sset );
    EXPECT_TRUE( validate_chars( sset, nucleic_acid_codes_plain() ));

    auto const alphabet = std::string( "ACGT" );

    // Test up to kmer size of 6
    for( size_t k = 1; k < 6; ++k ) {
        auto const list = kmer_list( k, alphabet );

        for( auto const& seq : sset ) {
            // LOG_DBG << "seq " << seq.sites();

            auto kmers = kmer_counts( seq, k );
            ASSERT_EQ( list.size(), kmers.size() );

            for( size_t i = 0; i < kmers.size(); ++i ) {
                // LOG_DBG1 << i << "\t" << kmers[i];

                auto const count = count_substring_occurrences( seq.sites(), list[i] );
                EXPECT_EQ( count, kmers[i] );
            }

            auto sum = std::accumulate( std::begin( kmers ), std::end( kmers ), 0 );
            EXPECT_EQ( seq.size() - k + 1, sum );
        }
    }
}

TEST( Sequence, SignatureFrequencies )
{
    // Skip test if no data availabe.
    NEEDS_TEST_DATA;

    // Load sequence file.
    std::string infile = environment->data_dir + "sequence/dna_10.fasta";
    SequenceSet sset;
    FastaReader().from_file(infile, sset);

    remove_all_gaps( sset );
    EXPECT_TRUE( validate_chars( sset, nucleic_acid_codes_plain() ));

    for( size_t k = 1; k < 6; ++k ) {
        for( auto const& seq : sset ) {
            auto const freqs = signature_frequencies( seq, k );
            auto const sum = std::accumulate( freqs.begin(), freqs.end(), 0.0 );

            EXPECT_FLOAT_EQ( 1.0, sum );
        }
    }
}

TEST( Sequence, KmerReverseComplements )
{
    // Test up to kmer size of 6
    for( size_t k = 1; k < 6; ++k ) {
        auto const list = kmer_list( k, "ACGT" );
        auto const revs = kmer_reverse_complement_indices( k );
        ASSERT_EQ( list.size(), revs.size() );

        // Get the length needed to store rev comp entries,
        auto const revcom_size = kmer_reverse_complement_size( k );

        for( size_t i = 0; i < list.size(); ++i ) {
            // LOG_DBG << list[i] << " <-- " << revs[i];

            auto const rev = reverse_complement( list[i] );

            // rev comp applied twice gives identical sequence.
            EXPECT_EQ( list[i], reverse_complement( rev ) );

            // Check that a seq and its rev map to the same index.
            auto const pos = std::distance(
                list.begin(), std::find( list.begin(), list.end(), rev )
            );
            EXPECT_LT( pos, list.size() );
            EXPECT_EQ( revs[i], revs[pos] );

            // Make sure that the indices are in range [0, revcom_size).
            EXPECT_LT( revs[i], revcom_size );

            // LOG_DBG1 << i << " " << pos << " -> " << revs[i];
        }
    }
}