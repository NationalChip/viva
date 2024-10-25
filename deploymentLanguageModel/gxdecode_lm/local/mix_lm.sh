#!/bin/sh

universal_lm=$1
user_lm=$2
text=$3
#if [ -f $universal_lm ]; then
#	gz=.gz
#else
#	gz=
#fi

ngram -debug 1 \
	-lm $univeral_lm -lambda 0.3 \
	-mix-lm $user_lm -mix-lambda1 0.7\
	-bayes 1 \
	-bayes-scale 0.5 \
	-ppl $text

# now do the same with context-priors file

echo "0.3 0.4 0.3" > PRIORS
