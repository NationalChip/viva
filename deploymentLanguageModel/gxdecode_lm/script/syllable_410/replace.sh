#!/bin/bash

cp -rf units.txt ../../data/dict_phn/units.txt

if ls bux_lexicon_labels_* 1> /dev/null 2>&1; then
echo "Copy packaging program"
cat bux_lexicon_labels_* > bux_lexicon_labels
chmod +x bux_lexicon_labels
cp -rf bux_lexicon_labels ../../make_lexicon/bux_lexicon_labels
else
echo "Copy source file"
cp -rf bux_lexicon_labels.py ../../make_lexicon/bux_lexicon_labels.py
fi

cp -rf units ../../make_lexicon/
