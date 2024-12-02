#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void getAndValidateEquationNumber(int *equationNumber);
void getAndValidateAccuracy(double *eps);
void getAndValidateCoefficientsAndResultVector(int n, double **a, double *b);
bool checkConvergence(int n, double **a);
int solveSystem(int n, double **a, double *b, double *x, double eps);
void clearAllocatedMemory(int equationNumber, double **a, double *b, double *x);
bool askToContinue();
double truncateNumber(const double value, const int decimalPlaces);
int getDecimalPlaces(double accuracy);

#endif // FUNCTIONS_H
