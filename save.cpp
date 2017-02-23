//array copy is used to deal with moving malloced arrays
void MaxSat::arrayCopy(int* arr1, int* arr2, int size) {
	for (int i = 0; i < size; i++) {
		arr1[i] = arr2[i];
	}
}

void MaxSat::selectRanking() {
	//a 2-D array to store the index and fitness of each individual
	//this array will be sorted by fitness
	//this is useful to keep track of the index of the individual
	//in the population list after we sort
	int** rankList = (int**) malloc(sizeof(int) * individuals  * 2);
	for (int i = 0; i < individuals; i++) {
		rankList[i] = (int*) malloc(sizeof(int) * 2);
		
		//save index
		rankList[i][0] = i;
		//save fitness
		rankList[i][1] = fitnessList[i];
	}
	
	//quicksort to sort the individuals by fitness
	qsort(rankList, individuals, sizeof rankList[0], compare);
	
	//calcualte total sum of individuals for denominator
	//of ranksort formula
	double sum = (individuals * (individuals + 1))/2;
	
	//outer loop to add n new individuals to the breeding pool
	for (int i = 0; i < individuals; i++) {
		double probability = 0;
		//generate a random probability
		double randomProbability = ((double) rand())/(INT_MAX);
		//sum up probability of individuals in ranked order until the
		//cumulative probability is greater than the random probability
		//generated
		for (int j = 0; j < individuals; j++) {
			probability += (j+1)/(sum);
			
			//once the sum is greater than or equal to the random probability
			//add the individual that broke the barrier of the random prob
			if (probability >= randomProbability) {
				arrayCopy(breedingPool[i], population[rankList[j][0]], numVariables);
			}
		}
	}
	
	//free pointer
	free(rankList);
}

void MaxSat::selectTournament() {
	int randNum;
	//malloc for two indiviuals
	int* individual1 = (int*) malloc(sizeof(int) * numVariables);
	int* individual2 = (int*) malloc(sizeof(int) * numVariables);
	//select n = individuals individuals
	for (int i = 0; i < individuals; i++) {
		//generate a random index and set population at that
		//index to be the first individual
		randNum = rand() % individuals;
		arrayCopy(individual1, population[randNum], numVariables);
		//get fitness of individual
		int fitness1 = fitnessList[randNum];
		
		//generate another random index and set population at it
		//to second individual
		randNum = rand() % individuals;
		arrayCopy(individual2, population[randNum], numVariables);
		//get the fitness
		int fitness2 = fitnessList[randNum];
		
		//compare fitnesses; put more fit one
		//into breeding pool
		if (fitness1 > fitness2) {
			arrayCopy(breedingPool[i], individual1, numVariables);
		} else {
			arrayCopy(breedingPool[i], individual2, numVariables);
		}
	}
	
	//free pointers
	free(individual1);
	free(individual2);
}

void MaxSat::selectBoltzmann() {
	//calcualte the total fitness for denominator of
	//boltzman formula. this is the sum of
	//e^fitness of each individual
	double totalFitness = 0;
	for (int n = 0; n < individuals; n++) {
		totalFitness += exp(fitnessList[n]);
	}
	
	//select n=individuals individuals
	for (int i = 0; i < individuals; i++) {
		double probability = 0;
		//generate a random probability 0<->1
		double randomProbability = ((double) rand())/(INT_MAX);
		//sum up probabilities of individuals until their cumulative
		//probability is greater than or equal to the random one
		//generated
		for (int j = 0; j < individuals; j++) {
			//probability of each individual is defined by
			//e^fitness of the individual all divided by the
			//previously calcualted total finess
			probability += exp(fitnessList[j])/totalFitness;
			
			//once the probability is high enough, add the individual
			//to the breeding pool
			if (probability >= randomProbability) {
				arrayCopy(breedingPool[i], population[j], numVariables);
			}
		}
	}
}
