#!/bin/bash


echo "Start train lm"
echo -e '\n'


corpus=$1
flag=$2



########################################################
#                 pos lm                               #
########################################################
# python gen_lm.py $corpus 2_grams.txt 3_grams.txt $flag
./gen_lm $corpus 2_grams.txt 3_grams.txt $flag

mkdir -p 1_grams
mkdir -p 2_grams
mkdir -p 3_grams
mkdir -p result_grams

sort -u 2_grams.txt >tmp.txt
mv tmp.txt 2_grams/2_grams.txt

sort -u 3_grams.txt >tmp.txt
mv tmp.txt 3_grams/3_grams.txt

sed -e "s/ /\n/g" seg_file.txt |sort -u >1_grams/1_grams.txt

sed -e "s/^/-1.82 /g" 1_grams/1_grams.txt >1_grams/out_1_grams
sed -i 's/$/ -1.5/g' 1_grams/out_1_grams

sed -e "s/^/-1.45 /g" 2_grams/2_grams.txt >2_grams/out_2_grams
sed -i 's/$/ -1.5/g' 2_grams/out_2_grams

sed -e "s/^/-1.12 /g" 3_grams/3_grams.txt >3_grams/out_3_grams

##cat
cat 1_grams/out_1_grams > result_grams/out_1_grams
echo -e "\n\2-grams:" >> result_grams/out_1_grams

cat 2_grams/out_2_grams  > result_grams/out_2_grams
echo -e "\n\3-grams:" >> result_grams/out_2_grams

cat 3_grams/out_3_grams > result_grams/out_3_grams
echo -e "\n" >> result_grams/out_3_grams
echo -n "\end\\" >> result_grams/out_3_grams
echo -e "\n" >> result_grams/out_3_grams



################################################################################################
#                                     result file
###############################################################################################
cat head.txt result_grams/out_1_grams result_grams/out_2_grams result_grams/out_3_grams >train_lm
echo "拷贝旧文件"
cp train_lm train_lm.old
./word_probability_fine_tuned
cat 1_grams/1_grams.txt |sort -u > vocab.txt
echo "更改后的文件生成成功"

#cp
cp vocab.txt ../../make_lexicon
cp train_lm ../../data/language_model
