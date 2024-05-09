echo "chry" &&
./bin/cpm_model --alpha 1 --k 8 --threshold 10 --inputFileName chry.txt --bufSize 1024 &&
./bin/cpm_model --alpha 1 --k 8 --threshold 10 --inputFileName mutated1_chry.txt --bufSize 1024 &&
./bin/cpm_model --alpha 1 --k 8 --threshold 10 --inputFileName mutated2_chry.txt --bufSize 1024 &&
./bin/cpm_model --alpha 1 --k 8 --threshold 10 --inputFileName mutated3_chry.txt --bufSize 1024 &&
./bin/cpm_model --alpha 1 --k 8 --threshold 10 --inputFileName mutated4_chry.txt --bufSize 1024 &&

echo " " &&
echo "maias" &&
./bin/cpm_model --alpha 1 --k 8 --threshold 10 --inputFileName maias.txt --bufSize 1024 &&
./bin/cpm_model --alpha 1 --k 8 --threshold 10 --inputFileName mutated1_maias.txt --bufSize 1024 &&
./bin/cpm_model --alpha 1 --k 8 --threshold 10 --inputFileName mutated2_maias.txt --bufSize 1024 &&
./bin/cpm_model --alpha 1 --k 8 --threshold 10 --inputFileName mutated3_maias.txt --bufSize 1024 &&
./bin/cpm_model --alpha 1 --k 8 --threshold 10 --inputFileName mutated4_maias.txt --bufSize 1024 &&

echo " " &&
echo "gatsby" &&
./bin/cpm_model --alpha 1 --k 8 --threshold 10 --inputFileName gatsby.txt --bufSize 1024 &&
./bin/cpm_model --alpha 1 --k 8 --threshold 10 --inputFileName mutated1_gatsby.txt --bufSize 1024 &&
./bin/cpm_model --alpha 1 --k 8 --threshold 10 --inputFileName mutated2_gatsby.txt --bufSize 1024 &&
./bin/cpm_model --alpha 1 --k 8 --threshold 10 --inputFileName mutated3_gatsby.txt --bufSize 1024 &&
./bin/cpm_model --alpha 1 --k 8 --threshold 10 --inputFileName mutated4_gatsby.txt --bufSize 1024