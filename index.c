#include "./includes.h";

int main()
{
    bool continueProgram = true;

    printf("Welcome! This program solves matrix equations!\n");

    do
    {
        int equationNumber = 0;
        double eps = 0;

        getAndValidateEquationNumber(&equationNumber);

        double **a = malloc(equationNumber * sizeof(double *));
        if (a == NULL)
        {
            printf("Memory allocation failed.\n");
            continueProgram = askToContinue();

            continue;
        }

        for (int i = 0; i < equationNumber; i++)
        {
            a[i] = malloc(equationNumber * sizeof(double));

            if (a[i] == NULL)
            {
                printf("Memory allocation failed for row %d.\n", i);
                clearAllocatedMemory(equationNumber, a, NULL, NULL);
                continueProgram = askToContinue();

                continue;
            }
        }

        double *b = malloc(equationNumber * sizeof(double));
        double *x = malloc(equationNumber * sizeof(double));

        if (b == NULL || x == NULL)
        {
            printf("Memory allocation failed.\n");
            clearAllocatedMemory(equationNumber, a, b, x);
            continueProgram = askToContinue();

            continue;
        }

        getAndValidateAccuracy(&eps);
        getAndValidateCoefficientsAndResultVector(equationNumber, a, b);

        if (!checkConvergence(equationNumber, a))
        {
            printf("The convergence condition is not fulfilled. The method cannot work.\n");
            clearAllocatedMemory(equationNumber, a, b, x);
            continueProgram = askToContinue();

            continue;
        }

        if (solveSystem(equationNumber, a, b, x, eps) == -1)
        {
            printf("Memory allocation failed.\n");
            clearAllocatedMemory(equationNumber, a, b, x);
            continueProgram = askToContinue();

            continue;
        }

        printResults(eps, equationNumber, x);
        clearAllocatedMemory(equationNumber, a, b, x);
        continueProgram = askToContinue();
    } while (continueProgram);

    return 0;
}
