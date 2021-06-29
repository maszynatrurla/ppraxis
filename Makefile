OUTPUT_FOLDER=bin
SOURCE_FOLDER=src
CC=gcc
FLAGS=-Wall -O3

clean:
	rm -f ${OUTPUT_FOLDER}/*

rpn:	${SOURCE_FOLDER}/rpn.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/rpn ${SOURCE_FOLDER}/rpn.c

sieve:  ${SOURCE_FOLDER}/sieve.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/sieve ${SOURCE_FOLDER}/sieve.c -lm

bingo:  ${SOURCE_FOLDER}/bingo.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/bingo ${SOURCE_FOLDER}/bingo.c

sudoku: ${SOURCE_FOLDER}/sudoku.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/sudoku ${SOURCE_FOLDER}/sudoku.c

flavius: ${SOURCE_FOLDER}/flavius.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/flavius ${SOURCE_FOLDER}/flavius.c

rot13:  ${SOURCE_FOLDER}/rot13.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/rot13 ${SOURCE_FOLDER}/rot13.c

apartment: ${SOURCE_FOLDER}/apartment.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/apartment ${SOURCE_FOLDER}/apartment.c

pii:    ${SOURCE_FOLDER}/pii.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/pii ${SOURCE_FOLDER}/pii.c

quine:  ${SOURCE_FOLDER}/quine.c
	gcc ${FLAGS} -Wno-format-security -o ${OUTPUT_FOLDER}/quine ${SOURCE_FOLDER}/quine.c

easter: ${SOURCE_FOLDER}/easter.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/easter ${SOURCE_FOLDER}/easter.c

shaney: ${SOURCE_FOLDER}/shaney.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/shaney ${SOURCE_FOLDER}/shaney.c

vigenere: ${SOURCE_FOLDER}/vigenere.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/vigenere ${SOURCE_FOLDER}/vigenere.c

roman:	${SOURCE_FOLDER}/roman.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/roman ${SOURCE_FOLDER}/roman.c

words:  ${SOURCE_FOLDER}/words.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/words ${SOURCE_FOLDER}/words.c

friday: ${SOURCE_FOLDER}/friday.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/friday ${SOURCE_FOLDER}/friday.c

csv:	${SOURCE_FOLDER}/csv.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/csv ${SOURCE_FOLDER}/csv.c

dodgson: ${SOURCE_FOLDER}/dodgson.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/dodgson ${SOURCE_FOLDER}/dodgson.c

chop:  ${SOURCE_FOLDER}/chop.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/chop ${SOURCE_FOLDER}/chop.c

turing: ${SOURCE_FOLDER}/turing.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/turing ${SOURCE_FOLDER}/turing.c

railfence: ${SOURCE_FOLDER}/railfence.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/railfence ${SOURCE_FOLDER}/railfence.c

pancake: ${SOURCE_FOLDER}/pancake.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/pancake ${SOURCE_FOLDER}/pancake.c

anagram: ${SOURCE_FOLDER}/anagram.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/anagram ${SOURCE_FOLDER}/anagram.c

primes: ${SOURCE_FOLDER}/primes.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/primes ${SOURCE_FOLDER}/primes.c -lm

spellcheck: ${SOURCE_FOLDER}/spellcheck.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/spellcheck ${SOURCE_FOLDER}/spellcheck.c

bloomspell: ${SOURCE_FOLDER}/bloomspell.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/bloomspell ${SOURCE_FOLDER}/bloomspell.c

hyphen: ${SOURCE_FOLDER}/hyphen.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/hyphen ${SOURCE_FOLDER}/hyphen.c

morse:  ${SOURCE_FOLDER}/morse.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/morse ${SOURCE_FOLDER}/morse.c

primality: ${SOURCE_FOLDER}/primality.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/primality ${SOURCE_FOLDER}/primality.c

pqueue: ${SOURCE_FOLDER}/pqueue.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/pqueue ${SOURCE_FOLDER}/pqueue.c

wfactor: ${SOURCE_FOLDER}/wfactor.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/wfactor ${SOURCE_FOLDER}/wfactor.c -lm

loan:   ${SOURCE_FOLDER}/loan.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/loan ${SOURCE_FOLDER}/loan.c -lm

automata: ${SOURCE_FOLDER}/automata.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/automata ${SOURCE_FOLDER}/automata.c

fermat: ${SOURCE_FOLDER}/fermat.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/fermat ${SOURCE_FOLDER}/fermat.c -lm

palindrome: ${SOURCE_FOLDER}/palindrome.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/palindrome ${SOURCE_FOLDER}/palindrome.c

wsearch: ${SOURCE_FOLDER}/wsearch.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/wsearch ${SOURCE_FOLDER}/wsearch.c

columnar: ${SOURCE_FOLDER}/columnar.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/columnar ${SOURCE_FOLDER}/columnar.c

piglatin: ${SOURCE_FOLDER}/piglatin.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/piglatin ${SOURCE_FOLDER}/piglatin.c

tenarytree: ${SOURCE_FOLDER}/tenarytree.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/tenarytree ${SOURCE_FOLDER}/tenarytree.c

lcs:    ${SOURCE_FOLDER}/lcs.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/lcs ${SOURCE_FOLDER}/lcs.c

feynmann:    ${SOURCE_FOLDER}/feynmann.c
	gcc ${FLAGS} -o ${OUTPUT_FOLDER}/feynmann ${SOURCE_FOLDER}/feynmann.c

all: rpn sieve bingo sudoku flavius rot13 apartment pii quine easter vigenere roman words friday csv dodgson\
	chop turing railfence pancake anagram primes spellcheck bloomspell hyphen morse primality pqueue\
	wfactor loan automata fermat wsearch columnar piglatin tenarytree lcs feynmann
