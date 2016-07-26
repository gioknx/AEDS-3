#!/bin/bash
#########################################
#
# Roda o tp nos toys e compara resultado
#
# Autor: Giovani Barcelos
#
#########################################
#
# Coloque na raiz do tp1
# Coloque os testes na pasta testes_toy
# rode chmod 777 . -R na raiz do tp

# Roda nos 6 toys
./tp1.exe testes_toy/test1_myanswer.txt testes_toy/test1.txt 4 5 0 > /dev/null 2>&1
./tp1.exe testes_toy/test2_myanswer.txt testes_toy/test2.txt 5 10 0 > /dev/null 2>&1
./tp1.exe testes_toy/test3_myanswer.txt testes_toy/test3.txt 4 3 0 > /dev/null 2>&1
./tp1.exe testes_toy/test4_myanswer.txt testes_toy/test4.txt 6 3 0 > /dev/null 2>&1
./tp1.exe testes_toy/test5_myanswer.txt testes_toy/test5.txt 4 3 0 > /dev/null 2>&1
./tp1.exe testes_toy/test6_myanswer.txt testes_toy/test6.txt 7 3 0 > /dev/null 2>&1

mkdir testes_toy/diffs

# Gera a diferença
#echo -e "TOY1: \n\n\n"
diff -q testes_toy/test1_myanswer.txt testes_toy/test1_answer.txt > testes_toy/diffs/diff1.txt
eq1=$?
#echo -e "TOY2: \n\n\n"
diff -q testes_toy/test2_myanswer.txt testes_toy/test2_answer.txt > testes_toy/diffs/diff2.txt
eq2=$?
#echo -e "TOY3: \n\n\n"
diff -q testes_toy/test3_myanswer.txt testes_toy/test3_answer.txt > testes_toy/diffs/diff3.txt
eq3=$?
#echo -e "TOY4: \n\n\n"
diff -q testes_toy/test4_myanswer.txt testes_toy/test4_answer.txt > testes_toy/diffs/diff4.txt
eq4=$?
#echo -e "TOY5: \n\n\n"
diff -q testes_toy/test5_myanswer.txt testes_toy/test5_answer.txt > testes_toy/diffs/diff5.txt
eq5=$?
#echo -e "TOY6: \n\n\n"
diff -q testes_toy/test6_myanswer.txt testes_toy/test6_answer.txt > testes_toy/diffs/diff6.txt
eq6=$?

echo "Resultado:"

if [ $eq1 == 0  ]
then
   echo "Teste 1 PASSOU"
else
   echo "Teste 1 FALHOU"
fi

if [ $eq2 == 0 ]
then
   echo "Teste 2 PASSOU"
else
   echo "Teste 2 FALHOU"
fi

if [ $eq3 == 0 ]
then
   echo "Teste 3 PASSOU"
else
   echo "Teste 3 FALHOU"
fi

if [ $eq4 == 0 ]
then
   echo "Teste 4 PASSOU"
else
   echo "Teste 4 FALHOU"
fi

if [ $eq5 == 0 ]
then
   echo "Teste 5 PASSOU"
else
   echo "Teste 5 FALHOU"
fi

if [ $eq6 == 0 ]
then
   echo "Teste 6 PASSOU"
else
   echo "Teste 6 FALHOU"
fi