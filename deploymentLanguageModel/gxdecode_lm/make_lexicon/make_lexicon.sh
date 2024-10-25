#/bin/bash

vocab=$1

if [ $# -ne 1 ];then
	echo "ERROR******"
	echo "Usage:$0 vocab.txt"
	exit 1;
fi

if ls bux_lexicon_labels 1> /dev/null 2>&1; then
echo "Run the packaging program"
./bux_lexicon_labels $vocab
else
echo "Run the source file"
python bux_lexicon_labels.py $vocab
fi

sed -i 's/  / /g' lexicon.txt

cat ./dict_file/noise_lexicon.txt lexicon.txt > 1.txt
mv 1.txt lexicon.txt
echo 'Output is $output'
echo 'Success'
