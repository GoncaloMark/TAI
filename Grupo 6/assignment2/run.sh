#!/bin/bash
echo "k=6"
./bin/was_chatted --testDataPath dataset/dataTest.csv --textFile dataset/test.txt --rhModelPath dataset/rhModel.csv --rcModelPath dataset/rcModel.csv --alpha 1.0 --kSize 6 