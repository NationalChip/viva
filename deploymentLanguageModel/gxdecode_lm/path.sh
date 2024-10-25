export LM_ROOT=`pwd`

export PATH=$PWD/utils/:$KALDI_ROOT/tools/openfst/bin:$PWD:$PATH
export PATH=$PATH:$LM_ROOT/bin/fstbin
export PATH=$PATH:$LM_ROOT/bin/openfst/bin
export PATH=$PATH:$LM_ROOT/tools/openfst-1.4.1/bin
export PATH=$PATH:$LM_ROOT/bin/decoderbin
export PATH=$PATH:$LM_ROOT/tools/openfst-1.4.1/include/fst
export LD_LIBRARY_PATH=$LM_ROOT/tools/openfst-1.4.1/lib${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH::$LM_ROOT/bin/openfst/lib/
export PATH=$PATH:`pwd`/tools/gxdecoder
