#!/bin/bash

# This script prepares the phoneme-based lexicon. It also generates the list of lexicon units
# and represents the lexicon using the indices of the units. 

dir=data/dict_phn
mkdir -p $dir

echo ==============================================
echo "    Phoneme-based Dictionary Preparation    "
echo ==============================================

# Convert phoneme sequences into the corresponding sequences of units indices, encoded by units.txt
utils/sym2int.pl -f 2- $dir/units.txt < $dir/lexicon.txt > $dir/lexicon_numbers.txt

echo "Phoneme-based dictionary preparation succeeded"
echo -e "\n"
