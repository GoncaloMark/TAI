# TAI

Trabalho de Teoria Algorítmica da informação

## Introduction and Project Idea
In the landscape of data compression, the copy model is an essential and effective strategy. It works by identifying similarities within a file by predicting what symbol is coming in the future based on a probability distribution measured by what was processed in the past and estimating the number of bits necessary for representing each symbol. Its effectiveness can, however, be affected by the occurrence of events that haven’t occurred before. A way to deal with this is to have a model which is adaptive and turns itself off when necessary by measuring the number of hits and misses of its predictions.
This project focuses on two main objectives which are: 
- To develop a copy model which is to theoretically estimate the amount of bits necessary to represent a certain amount of information using a probabilistic approach to compression; 
- To develop a mutation algorithm to randomise text files and test generic compression tools against the copy model.

## Parameters

### Copy model
The CPM takes in five parameters: 

- inputFileName;
- a or alpha, a smoothing parameter to avoid values of 0 in the probability calculations;
- k, the size of the sequence/Kmer being analysed;
- t, a threshold value for controlling when the model should turn itself off, resetting its counts;
- bufferSize, which defines the size of the chunk of data to be analysed and stored.

### Mutate
The Mutate takes in four parameters: 

- inputFileName;
- probability, a percentual number defining the odds of a symbol being mutated to a different one;
- outputFileName;
- bufferSize, which defines the size of the chunk of data to be analysed and stored.

## How to use

This section contains steps that one can use to get their own copy of our programs.

- Start by forking this repository using the fork button at the top right corner of the page to get your own copy of the repository. This is to ensure that you can actively experiment with the code of the project.

- Once the repository has been forked, you can clone the repository to your local machine using the following command:

	```bash
	git clone <paste URL of the forked repo here>
	```

- Lastly, with the repository cloned, you can navigate to the directory of the repository and run the following command to compile the code:

    ```bash
    make all
    ```

    Remember to get 'chry.txt'

This will create the executables by compiling and linking the project. You can now test and run the appropriate shell script to test the programs.
For example:

    ```bash
    bash run_cpm.sh
    ```

- **Note:** The use of a Linux based system is recommended.

- **Note:** To run the programs on a windows system, you can use the windows subsystem for linux (WSL2).

## Future Work
Ideally, we’d want to dedicate more time to improving the logging abilities of the program as it’s still considerably difficult to understand what it is doing or has done. Efforts need to be put towards understanding the ambiguous nature of the total bit calculations and reevaluating the methodology implemented. 

### Proudly brought to you by our group ❤️
