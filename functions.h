#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void getAndValidateEquationNumber(int *equationNumber);
void getAndValidateAccuracy(double *epsilon);
void getAndValidateCoefficientsAndResultVector(int equationNumber, double **coefficients, double *constantTerms);
bool checkConvergence(int equationNumber, double **coefficients);
int solveSystem(int equationNumber, double **coefficients, double *constantTerms, double *results, double epsilon);
void clearAllocatedMemory(int equationNumber, double **coefficients, double *constantTerms, double *results);
bool askToContinue();
double truncateNumber(double value, int decimalPlaces);
int getDecimalPlaces(double epsilon);
void printResults(double epsilon, int equationNumber, double *results);

#endif // FUNCTIONS_H
