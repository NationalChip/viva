#!/bin/bash
#coding:utf-8

if [ $# -ne 3 ]; then
    echo "No argument"
    echo "./run.sh corpus.txt modeling_unit(65 or 410) flag"
    exit 21
fi

chmod +x ./path.sh
source ./path.sh

text=$1
flag=$2
modeling_unit=$3

echo "text: ${text} flag: ${flag} modeling_unit: ${modeling_unit}"

if [ $modeling_unit -eq 65 ]; then
echo "run phoneme"
cd script/phoneme_65
./replace.sh
cd ../../
elif [ $modeling_unit -eq 410 ]; then
echo "run syllable"
cd script/syllable_410
./replace.sh
cd ../../
elif [ $modeling_unit -eq 191 ]; then
echo "run phoneme2"
cd script/phoneme_191
./replace.sh
cd ../../
else
echo "Error: Unexpected number of arguments. Expected 65 or 410, got $#."
exit 22
fi


#生成经中文分词的训练语料
cp $text data/corpus
cd data/corpus
echo "*****************************"
echo " Make Chinese training corpus"
echo "*****************************"
./run_lm.sh $text $flag

cd ../language_model
rm -rf train_lm.gz
gzip train_lm

cd ../../make_lexicon
echo "*****************************"
echo "   Make lexicon file	       "
echo "*****************************"
sed -i '/^$/d' vocab.txt
. make_lexicon.sh vocab.txt

sed -i "/<UNK> $/d" lexicon.txt

cp lexicon.txt ../data/dict_phn

cd ..
echo "**************************************"
echo " Starting to Construct decoding Graph "
echo "**************************************"

./make_TLG_WFST.sh

cd tools/gxdecoder/tools/
sh convert_fst.sh ../../../data/search_Graph/TLG.fst ../../../data/search_Graph/gxdecoder_tlg.fst
cd -

#rm -rf data/search_Graph/{G.fst,lexicon_numbers.txt,L.fst,LG_1.fst,LG_2.fst,LG_3.fst,LG.fst,T.fst,tokens.txt,units.txt}
rm -rf data/search_Graph/{G.fst,lexicon_numbers.txt,L.fst,LG_1.fst,LG_2.fst,LG_3.fst,LG.fst,T.fst,units.txt}


#fstdraw  --portrait --ranksep=2.0 --nodesep=0.5 data/search_Graph/TLG.fst >data/search_Graph/TLG.dot
#dot -Tsvg data/search_Graph/TLG.dot -o data/search_Graph/TLG.svg
echo "Language Model is in  data/search_Graph"
