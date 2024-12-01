#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define MIN_EPSILON 1e-12
#define MAX_EPSILON 1e-1
#define MAX_EQUATION_NUMBER 10
#define MIN_EQUATION_NUMBER 2
#define MAX_COEFFICIENT 1000
#define MIN_COEFFICIENT -1000
#define MAX_RESULT_VECTOR 1000000
#define MIN_RESULT_VECTOR -1000000
#define ESCAPE_ASCII_CODE 27

void getAndValidateEquationNumber(int* equationNumber) {
    do {
        printf("Enter the equation number: ");
        if (scanf("%d", equationNumber) != 1) {
            printf("Invalid input for equation number. Please enter an integer.\n");
            fflush(stdin);
            continue;
        }
        fflush(stdin);

        if (*equationNumber < MIN_EQUATION_NUMBER || *equationNumber > MAX_EQUATION_NUMBER) {
            printf("Equation number value is out of range. Please enter a value between %d and %d.\n", MIN_EQUATION_NUMBER, MAX_EQUATION_NUMBER);
        }
    } while (*equationNumber < MIN_EQUATION_NUMBER || *equationNumber > MAX_EQUATION_NUMBER);
}

void getAndValidateAccuracy(double* eps) {
    do {
        printf("Enter the accuracy (e.g., 0.0001): ");
        if (scanf("%lf", eps) != 1) {
            printf("Invalid input for accuracy. Please enter a valid number.\n");
            fflush(stdin);
            continue;
        }
        fflush(stdin);

        if (*eps < MIN_EPSILON || *eps > MAX_EPSILON) {
            printf("Accuracy value is out of range. Please enter a value between %.1e and %.1e.\n", MIN_EPSILON, MAX_EPSILON);
        }
    } while (*eps < MIN_EPSILON || *eps > MAX_EPSILON);
}

void getAndValidateCoefficientsAndResultVector(int n, double** a, double* b) {
    printf("Type coefficients of the matrix A and vector B:\n");

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int validInput = 0;
            while (!validInput) {
                printf("a[%d][%d] = ", i + 1, j + 1);
                if (scanf("%lf", &a[i][j]) != 1) {
                    printf("Invalid input for coefficient. Please enter a valid number.\n");
                    fflush(stdin);  // Очистка потока для предотвращения зацикливания
                    continue;
                }
                fflush(stdin);

                if (a[i][j] < MIN_COEFFICIENT || a[i][j] > MAX_COEFFICIENT) {
                    printf("Coefficient is out of range. Please enter a value between %d and %d.\n", MIN_COEFFICIENT, MAX_COEFFICIENT);
                } else {
                    validInput = 1;  // Корректный ввод
                }
            }
        }

        int validInput = 0;
        while (!validInput) {
            printf("b[%d] = ", i + 1);
            if (scanf("%lf", &b[i]) != 1) {
                printf("Invalid input for result vector. Please enter a valid number.\n");
                fflush(stdin);  // Очистка потока
                continue;
            }
            fflush(stdin);

            if (b[i] < MIN_RESULT_VECTOR || b[i] > MAX_RESULT_VECTOR) {
                printf("Result vector is out of range. Please enter a value between %d and %d.\n", MIN_RESULT_VECTOR, MAX_RESULT_VECTOR);
            } else {
                validInput = 1;  // Корректный ввод
            }
        }
    }
}


bool checkConvergence(int n, double** a) {
    for (int i = 0; i < n; i++) {
        double sum = 0.0;

        for (int j = 0; j < n; j++) {
            if (i != j) {
                sum += fabs(a[i][j]);
            }
        }
        if (fabs(a[i][i]) <= sum) {
            return false;
        }
    }

    return true;
}

int solveSystem(int n, double** a, double* b, double* x, double eps) {
    double* xp = malloc(n * sizeof(double));
    if (xp == NULL) {
        return -1;
    }

    for (int i = 0; i < n; i++) {
        xp[i] = b[i] / a[i][i];
    }

    double maxDelta;
    do {
        maxDelta = 0.0;
        for (int i = 0; i < n; i++) {
            x[i] = b[i];
            for (int j = 0; j < n; j++) {
                if (i != j) {
                    x[i] -= a[i][j] * xp[j];
                }
            }
            x[i] /= a[i][i];

            double delta = fabs(x[i] - xp[i]);
            if (delta > maxDelta) {
                maxDelta = delta;
            }
            xp[i] = x[i];
        }
    } while (maxDelta >= eps);

    free(xp);

    return 0;
}

void clearAllocatedMemory(int equationNumber, double** a, double* b, double* x) {
    for (int i = 0; i < equationNumber; i++) {
        free(a[i]);
    }
    free(a);
    free(b);
    free(x);
}

bool askToContinue() {
    printf("Do you want to run programm again? Press 'y' to continue or any other key to exit: ");

    return getchar() == 'y';
}

int main() {
    bool continueProgram = true;

    while (continueProgram) {
        int equationNumber = 0;
        double eps = 0;

        getAndValidateEquationNumber(&equationNumber);

        double** a = malloc(equationNumber * sizeof(double*));
        if (a == NULL) {
            printf("Memory allocation failed.\n");
            continueProgram = askToContinue();
            continue;
        }

        for (int i = 0; i < equationNumber; i++) {
            a[i] = malloc(equationNumber * sizeof(double));
            if (a[i] == NULL) {
                printf("Memory allocation failed for row %d.\n", i);
                clearAllocatedMemory(equationNumber, a, NULL, NULL);
                continueProgram = askToContinue();
                continue;
            }
        }

        double* b = malloc(equationNumber * sizeof(double));
        double* x = malloc(equationNumber * sizeof(double));
        if (b == NULL || x == NULL) {
            printf("Memory allocation failed.\n");
            clearAllocatedMemory(equationNumber, a, b, x);
            continueProgram = askToContinue();
            continue;
        }

        getAndValidateAccuracy(&eps);
        getAndValidateCoefficientsAndResultVector(equationNumber, a, b);

        if (!checkConvergence(equationNumber, a)) {
            printf("The convergence condition is not fulfilled. The method cannot work.\n");
            clearAllocatedMemory(equationNumber, a, b, x);
            continueProgram = askToContinue();
            continue;
        }

        if (solveSystem(equationNumber, a, b, x, eps) == -1) {
            printf("Memory allocation failed.\n");
            clearAllocatedMemory(equationNumber, a, b, x);
            continueProgram = askToContinue();
            continue;
        }

        printf("Results:\n");
        for (int i = 0; i < equationNumber; i++) {
            printf("x[%d] = %lf\n", i, x[i]);
        }

        clearAllocatedMemory(equationNumber, a, b, x);
        continueProgram = askToContinue();
    }

    return 0;
}
