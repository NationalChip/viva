#!/bin/bash


# To be run from one directory above this script.
text=$1

order=3

for f in "$text/train.txt" "$text/test.txt"; do
  [ ! -f $x ] && echo "$0: No such file $f" && exit 1;
done

echo "==============================================="
echo "     Training a ${order}-gram language model          "
echo "==============================================="

rm -rf $text/train_lm.gz


begin_time=`date +%s`
#smooth="-cdiscount 0 -gt3min 1"
#smooth="-kndiscount1 -ukndiscount2 -kndiscount3 -interpolate1 -interpolate2 -interpolate3"
#smooth="-interpolate1 -interpolate2 -interpolate3"
#ngram-count -text $text/train.txt -order $order  -unk -map-unk "<unk>"  -lm $text/train_lm.gz
#ngram-count -text $text/train.txt -order $order -ukndiscount -interpolate -unk -map-unk "<unk>"  -lm $text/train_lm.gz
#ngram-count -text $text/train.txt -order 2 -unk -map-unk "<unk>"  -lm $text/train_lm.gz
#ngram-count -text $text/train.txt -order 3 -prune 1e-5 -kndiscount -interpolate -unk -map-unk "<unk>"  -lm $text/train_lm.gz 
ngram-count -text $text/train.txt -order $order -kndiscount -interpolate -unk -map-unk "<unk>"  -lm $text/train_lm.gz 
#ngram-count -text $text/train.txt -order 2 -unk -map-unk "<unk>"  -lm $text/train_lm.gz 
#ngram-count -text $text/train.txt -limit-vocab -vocab $text/vocab.txt -order $order -prune 5e-7 -kndiscount -interpolate -unk -map-unk "<unk>"  -lm $text/train_lm.gz 
#ngram-count -text $text/train.txt -limit-vocab -vocab make_lexicon/vocab.txt -maxent -lm $text/train_lm.gz -order $order  -debug 2 -maxent-convert-to-arpa
#ngram-count -text $text/train.txt -maxent -lm $text/train_lm.gz -order $order  -debug 2 -maxent-convert-to-arpa
#ngram -lm $text/word_lm.gz -ppl $text/test.txt 
#ngram-count -text $text/train.txt -order $order $smooth -unk -map-unk "<unk>" -lm $text/train_lm.gz 

#kenlm
#lmplz -o $order --prune 0 2 4 --discount_fallback < $text/train.txt > $text/train_lm
#lmplz -o $order --discount_fallback < $text/train.txt > $text/train_lm
#gzip $text/train_lm

end_time=`date +%s`
cost_time=$[end_time - begin_time]


#Test PPL
ngram -lm $text/train_lm.gz -ppl $text/text 

echo "Time cost of training a language model is:"$cost_time"s"
