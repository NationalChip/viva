#!/bin/bash

echo =====================================================================
echo "                       TLG WFST Construction                       "
echo =====================================================================
#Script start time
begin_time=`date +%s`

# Construct the phoneme-based dict.
local/bux_prepare_phn_dict.sh || exit 1;

# Compile the lexicon and token FSTs
utils/ctc_compile_dict_token.sh --dict-type "phn" data/dict_phn data/lang_tmp data/lang || exit 1;
#exit 0
# Train and compile LMs. 
#local/bux_train_lms.sh data/language_model || exit 1;

# Compile the language-model FST and the final decoding graph TLG.fst
local/bux_decode_graph.sh data/language_model data/lang data/search_Graph || exit 1;

#Script end time
end_time=`date +%s`

#Compute time of constructing TLG.fst
cost_time=$[end_time - begin_time]
echo "Running time is:"$cost_time"s"
