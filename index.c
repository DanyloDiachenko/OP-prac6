#include "./includes.h"

int main()
{
    bool continueProgram = true;

    printf("Welcome! This program solves matrix equations!\n");

    do
    {
        int equationNumber = 0;
        double epsilon = 0.0;

        getAndValidateEquationNumber(&equationNumber);

        double **coefficients = malloc(equationNumber * sizeof(double *));
        if (coefficients == NULL)
        {
            printf("Memory allocation failed.\n");
            continueProgram = askToContinue();

            continue;
        }

        for (int i = 0; i < equationNumber; i++)
        {
            coefficients[i] = malloc(equationNumber * sizeof(double));

            if (coefficients[i] == NULL)
            {
                printf("Memory allocation failed for row %d.\n", i);
                clearAllocatedMemory(equationNumber, coefficients, NULL, NULL);
                continueProgram = askToContinue();

                continue;
            }
        }

        double *constantTerms = malloc(equationNumber * sizeof(double));
        double *results = malloc(equationNumber * sizeof(double));

        if (constantTerms == NULL || results == NULL)
        {
            printf("Memory allocation failed.\n");
            clearAllocatedMemory(equationNumber, coefficients, constantTerms, results);
            continueProgram = askToContinue();

            continue;
        }

        getAndValidateAccuracy(&epsilon);
        getAndValidateCoefficientsAndResultVector(equationNumber, coefficients, constantTerms);

        if (!checkConvergence(equationNumber, coefficients))
        {
            printf("The convergence condition is not fulfilled. The method cannot work.\n");
            clearAllocatedMemory(equationNumber, coefficients, constantTerms, results);
            continueProgram = askToContinue();

            continue;
        }

        if (solveSystem(equationNumber, coefficients, constantTerms, results, epsilon) == -1)
        {
            printf("Memory allocation failed.\n");
            clearAllocatedMemory(equationNumber, coefficients, constantTerms, results);
            continueProgram = askToContinue();

            continue;
        }

        printResults(epsilon, equationNumber, results);
        clearAllocatedMemory(equationNumber, coefficients, constantTerms, results);
        continueProgram = askToContinue();
    } while (continueProgram);

    return 0;
}
