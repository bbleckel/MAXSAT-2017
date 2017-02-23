GA & PBIL MAXSAT
By: Bo Bleckel, Jasper Houston, and Dylan Parsons
Nature Inspired Computation - CSCI 3445
Bowdoin College

This program solves satisfiability problems using evolutionary algorithms.
	The main.cpp file deals with executing the file and parsing the command line arguments.
	The command line arguments are as follows, in the order presented:

For running Genetic Algorithm:
	file_name    = path of file from which to read (string)
	individuals  = number of individuals in population (int)
	selection    = type of selection of breeding pool (string):
	                 ts   = tournament selection - implies ts1
	                        ts1 = same individual cannot compete against self
	                        ts2 = same individual can compete against self
	                 rs   = rank-based selection
	                 bs   = Boltzmann selection
	crossover    = crossover method (string):
	                 1c   = 1-point crossover
	                 uc   = uniform crossover
	pC           = crossover probability (double)
	pM           = mutation probability (double)
	generations  = max number of generations to run (int)
	algorithm    = type of algorithm (g or p) (string)

	example: ./main t3pm3-5555.spn.cnf 100 ts 1c 0.7 0.01 1000 g

For running PBIL:
	file_name          = path of file from which to read (string)
	individuals        = number of individuals in population (int)
	pos_learning_rate  = positive learning rate for best-individual update (double):
	neg_learning_rate  = negative learning rate for worst-individual update (double):
	mutation_prob      = mutation probability (double)
	mutation_amnt      = amount the PV is changed if mutated (double)
	generations        = max number of generations to run (int)
	algorithm          = type of algorithm (g or p) (string)

	example: ./main t3pm3-5555.spn.cnf 100 0.1 0.075 0.02 0.05 1000 p

These arguments are entered after the executable for the file, which is ./main
	and can be compiled using the included Makefile (simply type make to compile).

MAXSAT files to run can be found in the included folder. Additional files can be
	added to the folder. The program will run any file as long as the correct
	filepath is given as the first argument when executed.

The algorithms are implemented using a class. This class is contained in the MAXSAT.cpp
and MAXSAT.h files. The MAXSAT class holds all variables for solving both algorithms 
	as well as all functions. 

The folder also includes our paper which details our experimental methodology
	as well as our results. Note that some functions described in the paper such
	as the 99% accuracy cutoff, are not included in the final version of the code.
	In addition, there are functions included in the code that were used for batch
	testing parameters that are not called in the final build.
