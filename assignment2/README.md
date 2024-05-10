# TAI

Trabalho_2 de Teoria Algorítmica da Informação
Universidade de Aveiro 

## Introduction and Project Idea
In this project, we explore the idea of using compression techniques and algorithms to determine how a text was created. Normally, the approaches to classify texts, involve feature extraction and selection. These features are then put into a function that maps them onto unique classes and executes a classification process. 
Here, for each class (rh for Human, or rc for ChatGPT) associated to a piece of text, a model is built that encapsulates that given text. This set of data will then be used to calculate the test text’s normalised relative compression using the model’s entropy. The higher the relative compression compared to a model, the higher the likelihood of those being similar. This, in short, means that the model that can use the least amount of bits to describe a context/text will be the most similar to it, allowing the program to predict which class the reference text belongs to.

## Parameters

### Finite Context Model
The program takes in 6 arguments:
- A csv file, composed of multiple smaller texts written by humans and by ChatGPT. This is used for testing and evaluating the predictions by registering the number of hits and misses;
- A text file for experimentation to try and do a one-time prediction of a larger text whether it’s human-made or AI-written;
- The path to a csv file, made off of texts written by humans to make a model of the rh class;
- The path to a csv file, made off of texts written by AI to make a model of the rc class;
- An alpha serving as a smoothing parameter (further explained later);
- And, lastly, an integer value, k, which determines the amount of characters of context for each event.

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

    Remember to get 'test.txt', 'dataTest.csv', 'rhModel.csv', 'rcModel.csv' in order to be able to fully test the program. We got our datasets from the following link: https://www.kaggle.com/datasets/jdragonxherrera/augmented-data-for-llm-detect-ai-generated-text/versions/1.
  The provided files in the "dataset" folder are but a very small sample to allow new users to run the program from the get go.

This will create the executables by compiling and linking the project. You can now test and run the appropriate shell script to test the programs.
For example:

    ```bash
    bash run.sh
    ```

- **Note:** The use of a Linux based system is recommended.

- **Note:** To run the programs on a windows system, you can use the windows subsystem for linux (WSL2).

## Future Work
We'd like to further explore the area of Information Theory and learn more about compression algorithms. The program does work incredibly well with a ~90% hit chance on average. However, it can get incredibly slow with higher numbers of k. Even though, we do believe this is simply related to the size of the training datasets, this could also be due to a limitation from our machines (which we can't do anything about) or, worst case scenario, a optimisation issue which could be looked into as well.
All in all this project was widely successful and we look forward to learn more!

### Proudly brought to you by our group ❤️
