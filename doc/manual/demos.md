Demos {#demos}
===========

The demo programs are located in

    ./doc/demos/

They are intended to show some of the functionality of genesis in a ready-to-use setting. You can
also use them as starting point for developing your own applications. Be however aware that the
demos only contain basic error checks for the user input, for reasons of simplicity.

The simplest way to test and use them is to copy the needed files to the `./apps` directory and
compile them using `make update`. See @ref setup_apps for setup instructions.

For the C++ demos, start by inspecting the `main` function. From there, all other functions
of the demos are called.

# Visualize Placements {#demos_visualize_placements}

This demo is located at

    ./doc/demos/visualize_placements.cpp

The program takes a path to either a `jplace` file, or a directory containing `jplace` files, reads
all of them and counts the placement mass (according to the `like_weight_ratio` of each placement)
for each branch of the tree. Those masses are then turned into colors representing a heat
gradient of how much placement mass was placed on each branch, and writes a tree with this
color information to a given nexus file path. The resulting file can be read and visualized
with, e.g., [FigTree](http://tree.bio.ed.ac.uk/software/figtree/):

![Tree with branches colored according to weight of placements on the branches.](visualize_placements.png)

If a directory is given as first command line argument, all files in it that end in ".jplace"
are processed and their weights are accumulated. This means that all trees in the `jplace` files
need to have the same topology. For reasons of simplicity, we only check if they have the correct
number of edges. It is thus up to the user to make sure that all trees have identical topology.
Otherwise, the result will be meaningless. If for example EPA was run multiple times with different
sets of query sequences, but always the same reference tree, the resulting `jplace` files can be
used here.

If a single file is given as input, all of the above is obsolete. The filename also
does not need to end in ".jplace" in this case. In this case, simple this file is visualized.

Furthermore, as second command line argument, the user needs to provide a valid filename for the
output nexus file. That means, the path to the file needs to exist, but the file not (yet).

# Extract Clade Placements {#demos_extract_clade_placements}

This demo is located at

    ./doc/demos/extract_clade_placements.cpp

The program takes three input arguments in the following order:

 1. A `jplace` input file. The pqueries in this file are then split into different samples. Each
    such sample contains all pqueries whose placements are placed in a certain clade of the
    reference tree with more than a cutoff threshold of their accumulated likelihood weight.

    According to the `jplace` standard, each pquery can have multiple possible placement positions.
    Each position has a value `like_weight_ratio`, which can be interpreted as a measure of
    probability of how likely the placement belongs to the branch that it is attached to.
    The ratios for all branches of the tree thus sum up to 1.0.

    If more of this placement mass than the threshold is placed on the branches of a single
    clade of the tree, the according pquery is assigned to that clade. The threshold is
    hardcoded in this demo and set to 0.95 (but can be changed if needed, of course).

    It is possible that the placement algorithm (e.g., EPA or pplacer) did not output placements
    with low like_weight_ratios, depending on the selected options (see the respective manual
    for more details on how to change this). This means that the provided sum might be lower
    than 1.0 for some pqueries. In order to compensate for this (thus, to avoid classifying those
    pqueries as uncertain), we normalize the like_weight_ratios first, so that their sum is 1.0
    again. This step thus ignores the uncertainties resulting from the placement algorithm.
 2. A directory path, which needs to contain a single file for each clade of the reference tree.
    The file names are used as clade names. Each file in the directory then needs to contain a
    list of all taxa names of that clade, one per line.
    The taxa names need to be the same as the node names of the reference tree in the `jplace` file.

    If a taxon in a clade file is not found on the tree, a warning is issued, and the taxon is
    ignored. If the tree contains taxa which are not in any clade file, those branches are
    assigned to a special clade "basal_branches". This is also the case for the inner branches
    of the tree: all those branches which do not belong to one of the clades are collected in
    this special clade.

    As a second special clade, the "uncertain" clade is used to collect all those pqueries
    which did not fall into any clade with more than the threshold of accumulated likelihood
    weights.

    The edges that belong to a clade are determined by finding the smalles subtree (split) of
    the tree that contains all nodes of the clade. That means, the clades should be monophyletic
    in order for this algorithm to work properly. Furthermore, the user needs to make sure that
    each taxon is contained in at most one clade. Otherwise, the algorithm won't work properly.

    Remark: The rooting of the tree is insignificant for this program. Even if the root
    coincidentally lies within one of the clades, the result is the same. The program does not
    change the root; thus, when visualizing the clades, be aware that the tree might look different
    depending on the rooting.
 3. An output directory path. For each clade (including the two special clades), a `jplace` file
    named after the clade is written to that path. Each `jplace` file then contains all pqueries
    that were assigned to that clade.

A typical use case for this program is to extract pqueries that were placed in a particular
clade of interest in an evolutionary placement analysis. The extracted placements can then be
further examined in downstream analyses.

It is also possible to do a second run of evolutionary placement with the original sequences of
the pqueries of one clade, using a refined reference tree for that clade with a higher resolution
(more reference taxa). This two-step placement approach allows for finely grained
placement positions while keeping the computational load relatively small.