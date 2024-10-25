#!/usr/bin/bash

if [ $# != 2 ]; then 
    echo "Usage: convert_fst.sh kaldi_hclg gxdecoder_fst" 
    exit 0;
fi

hclg=$1
gxdecoder_fst=$2

openfst_txt_tempfile=$(mktemp /tmp/fst.XXXXXX)
fstprint $hclg > $openfst_txt_tempfile
./fst-init $openfst_txt_tempfile $gxdecoder_fst


