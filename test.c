// Preston Engler
// ppengler@mtu.edu
//
// test.c
// Made for a class, CS3331 (Concurrent Programming)
//
// Runs 4 different processes at once which finish independently
// 1) computes the n-th Fibonacci number using recursion
// 2) finds the solution of Buffon's needle problem by throwing a needle r times
// 3) approximates the area of an ellipse by testing if randomly sized rectangles fit in the ellipse
// 4) simulates a simple carnival/pinball game

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

void fib(int n);
long int fibNum(int n);
void buffon(int r);
void ellipseArea(int a, int b, int s);
void pinball(int x, int y);

int main(int argc, char** argv ){

	// Error if not enough arguments
	if( argc < 8 ){
		printf("Not enough arguments\nUsage: prog1 n r a b s x y\n");
		exit(0);
	}

	// Parse command line input
	int n = atoi(argv[1]);
	int r = atoi(argv[2]);
	int a = atoi(argv[3]);
	int b = atoi(argv[4]);
	int s = atoi(argv[5]);
	int x = atoi(argv[6]);
	int y = atoi(argv[7]);

	printf("Main Process Started\n");
	
	// Output the input for posterity's sake
	char* out = "Fibonacci Input";	
	printf("%-26s = %d\n", out, n);
	out = "Buffon's Needle Iterations";
	printf("%-26s = %d\n", out, r);
	out = "Total random Number Pairs";
	printf("%-26s = %d\n", out, s);
	out = "Semi-Major Axis Length";
	printf("%-26s = %d\n", out, a);
	out = "Semi-Minor Axis Length";
	printf("%-26s = %d\n", out, b);
	out = "Number of Bins";
	printf("%-26s = %d\n", out, x);
	out = "Number of Ball Droppings";
	printf("%-26s = %d\n\n", out, y);



	// fork for process 1, fibonacci
	if( fork() == 0 ){
		fib(n);
		exit(0);
	}

	out = "Fibonacci Process Created\n";
	write(1, out, strlen(out));

	// fork for process 2, buffon's needle
	if( fork() == 0 ){
		buffon(r);
		exit(0);
	}

	out = "Buffon's Needle Process Created\n";
	write(1, out, strlen(out));

	// fork for process 3, ellipse area estimation
	if( fork() == 0 ){
		ellipseArea(a, b, s);
		exit(0);
	}

	out = "Ellipse Area Process Created\n";
	write(1, out, strlen(out));

	// fork for process 4, pinball simulation
	if( fork() == 0 ){
		pinball(x, y);
		exit(0);
	}
	
	out = "Pinball Process Created\n";
	write(1, out, strlen(out));

	
	// Wait for processes 1-4 to finish
	out = "Main Process Waits\n";
	write(1, out, strlen(out));

	int status = 0;
	
	wait(&status);	
	wait(&status);	
	wait(&status);	
	wait(&status);	

	// End the program
	out = "Main Process Exits\n";
	write(1, out, strlen(out));
	exit(0);
}



// FUNCTION : fib
//     Process 1
//     Calculates and prints the nth fibonacci number
// PARAMETER :
//     int n : the nth fibonacci number that is to be calculated and printed

void fib(int n){
	char* out = "   Fibonacci Process Started\n";
	write(1, out, strlen(out));

	char outArray [51];

	sprintf(outArray, "   %s %d\n", "Input Number", n);
	write(1, outArray, strlen(outArray));	

	// Calculate the nth fibonacci number
	long int result = fibNum(n);

	sprintf(outArray, "   %s%d%s %ld\n", "Fibonacci Number f(", n, ") is", result);
	write(1, outArray, strlen(outArray));
	
	out = "   Fibonacci Process Exits\n";
	write(1, out, strlen(out));	
}


// FUNCTION : fibNum
//     Helper function for Process 1
//     Recursively calculates the nth fibonacci number
// PARAMETER :
//     int n : the nth fibonacci number which is to be calculated

long int fibNum(int n){
	if( n <= 2 ){
		return 1;
	}
	return fibNum(n-1) + fibNum(n-2);
}



// FUNCTION : buffon
//     Process 2
//     Simulates Buffon's needle problem with a constant gap of 1 and needle length of 1, then prints the result
// PARAMETER :
//     int r : the number of times the random needle toss is to be simulated

void buffon(int r){
	char* out = "      Buffon's Needle Process Started\n";
	write(1, out, strlen(out));
	
	char outArray [51];

	sprintf(outArray, "      %s %d\n", "Input Number", r);
	write(1, outArray, strlen(outArray));

	
	// seed random with an appropriately random seed so it's not the same every time	
	srand(time(NULL));

	// simulate a random needle r times
	int t = 0;
	int i = 0;
	while( i < r ){
		double d = rand() / (double)RAND_MAX;
		double a = (rand() / (double)RAND_MAX) * 2 * acos(-1.0);
		double totalHeight = d + sin(a);
		
		if(totalHeight < 0 || totalHeight > 1){
			t++;
		}
		i++;
	}
	// calculate simulated probability
	double result = t / (double) r;

	sprintf(outArray, "      %s %.5f\n", "Estimated Probability is", result);
	write(1, outArray, strlen(outArray));

	out = "      Buffon's Needle Process Exits\n";
	write(1, out, strlen(out));
}



// FUNCTION : ellipseArea
//     Process 3
//     Estimates the area of an ellipse by testing random rectangles
// PARAMETER :
//     int a : The semi-major axis of the ellipse
//     int b : The semi-minor axis of the ellipse
//     int s : The number of random rectangles to test

void ellipseArea(int a, int b, int s){
	char* out = "         Ellipse Area Process Started\n";
	write(1, out, strlen(out));
	
	char outArray [51];
	sprintf(outArray, "         Total random Number Pairs %d\n", s);
	write(1, outArray, strlen(outArray));

	sprintf(outArray, "         Semi-Major Axis Length %d\n", a);
	write(1, outArray, strlen(outArray));

	sprintf(outArray, "         Semi-Minor Axis Length %d\n", b);
	write(1, outArray, strlen(outArray));

	srand(time(NULL));	

	// test if random rectangles are inside an ellipse
	double x;
	double y;
	int i = 0;
	int t = 0;
	while( i < s ){
		x = (rand() / (double) RAND_MAX) * a;
		y = (rand() / (double) RAND_MAX) * b;
		if( (x*x)/(a*a) + (y*y)/(b*b) <= 1 ){
			t++;	
		}
		i++;
	}
	
	// calculate the estimated and real areas
	double result = (t / (double) s) * 4 * a * b;
	double realArea = acos(-1.0) * a * b;

	sprintf(outArray, "         Total Hits %d\n", t);
	write(1, outArray, strlen(outArray));

	sprintf(outArray, "         Estimated Area is %.5f\n", result);
	write(1, outArray, strlen(outArray));

	sprintf(outArray, "         Actual Area is %.5f\n", realArea);
	write(1, outArray, strlen(outArray));

	out = "         Ellipse Area Process Exits\n";
	write(1, out, strlen(out));	
}



// FUNCTION : pinball
//     Process 4
//     Simulates one of those carnival pinball game-things. You drop the ball, it hits the pegs then goes into one of x buckets.
// PARAMETER :
//     int x : number of bins the ball can fall into
//     int y : number of balls to drop down
void pinball(int x, int y){
	
	char* out = "Simple Pinball Process Started\n";
	write(1, out, strlen(out));

	char outArray[101];
	sprintf(outArray, "Number of Bins %d\n", x);
	write(1, outArray, strlen(outArray));

	sprintf(outArray, "Number of Ball Droppings %d\n", y);
	write(1, outArray, strlen(outArray));

	// initialize an int array so all integers = 0. 
	// this array is to count how many balls fell into each bucket
	int i = 0;
	int buckets[x];
	while( i < x ){
		buckets[i] = 0;
		i++;
	}
		
	// seed the random number generator so it's fairly random
	srand(time(NULL));

	// simulate y balls dropping
	i = 0;
	while( i < y ){

		// Since there are x buckets, there will be x-1 levels for each ball to fall through
		int j = 0;
		double pos = x / 2.0 + 0.5;
		while( j < x-1 ){
			if( (rand() / (double) RAND_MAX) > 0.5 ){
				pos = pos + 0.5;
			}
			else{
				pos = pos - 0.5;
			}
			j++;
		}	
	
		// Increment the bucket which the ball fell into
		buckets[(int)pos - 1]++;
		
		i++;
	}
	
	// Find the max to scale histogram correctly
	i = 1;
	int max = 0;
	while( i < x ){
		if( buckets[i] > buckets[max] ){
			max = i;
		}
		i++;
	}

	i = 0;
	while( i < x ){
		sprintf(outArray, "%3d-(%7d)-(%5.2f)|", i+1, buckets[i], buckets[i]/(double) y * 100);
		
		// Add the stars for the histogram
		int numStars = (int) ((buckets[i]/(double) buckets[max]) * 50 );
		while( numStars > 0 ){
			strcat(outArray, "*");
			numStars--;
		}
		strcat(outArray, "\n");
		write(1, outArray, strlen(outArray));
		i++;
	}
	out = "Simple Pinball Process Exits\n";
	write(1, out, strlen(out));
}
