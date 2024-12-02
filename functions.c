void getAndValidateEquationNumber(int *equationNumber)
{
    do
    {
        printf("Enter the equation number: ");
        if (scanf("%d", equationNumber) != 1)
        {
            printf("Invalid input for equation number. Please enter an integer.\n");
            fflush(stdin);

            continue;
        }
        fflush(stdin);

        if (*equationNumber < MIN_EQUATION_NUMBER || *equationNumber > MAX_EQUATION_NUMBER)
        {
            printf("Equation number value is out of range. Please enter a value between %d and %d.\n", MIN_EQUATION_NUMBER, MAX_EQUATION_NUMBER);
        }
    } while (*equationNumber < MIN_EQUATION_NUMBER || *equationNumber > MAX_EQUATION_NUMBER);
}

void getAndValidateAccuracy(double *epsilon)
{
    do
    {
        printf("Enter the accuracy (e.g., 0.0001): ");
        if (scanf("%lf", epsilon) != 1)
        {
            printf("Invalid input for accuracy. Please enter a valid number.\n");
            fflush(stdin);

            continue;
        }
        fflush(stdin);

        if (*epsilon < MIN_EPSILON || *epsilon > MAX_EPSILON)
        {
            printf("Accuracy value is out of range. Please enter a value between %.1e and %.1e.\n", MIN_EPSILON, MAX_EPSILON);
        }
    } while (*epsilon < MIN_EPSILON || *epsilon > MAX_EPSILON);
}

void getAndValidateCoefficientsAndResultVector(int equationNumber, double **coefficients, double *b)
{
    printf("Type coefficients of the matrix A and vector B:\n");

    for (int i = 0; i < equationNumber; i++)
    {
        for (int j = 0; j < equationNumber; j++)
        {
            int validInput = 0;

            while (!validInput)
            {
                printf("a[%d][%d] = ", i + 1, j + 1);

                if (scanf("%lf", &coefficients[i][j]) != 1)
                {
                    printf("Invalid input for coefficient. Please enter a valid number.\n");
                    fflush(stdin);

                    continue;
                }
                fflush(stdin);

                if (coefficients[i][j] < MIN_COEFFICIENT || coefficients[i][j] > MAX_COEFFICIENT)
                {
                    printf("Coefficient is out of range. Please enter a value between %d and %d.\n", MIN_COEFFICIENT, MAX_COEFFICIENT);
                }
                else
                {
                    validInput = 1;
                }
            }
        }

        int validInput = 0;

        while (!validInput)
        {
            printf("b[%d] = ", i + 1);

            if (scanf("%lf", &b[i]) != 1)
            {
                printf("Invalid input for result vector. Please enter a valid number.\n");
                fflush(stdin);

                continue;
            }
            fflush(stdin);

            if (b[i] < MIN_RESULT_VECTOR || b[i] > MAX_RESULT_VECTOR)
            {
                printf("Result vector is out of range. Please enter a value between %d and %d.\n", MIN_RESULT_VECTOR, MAX_RESULT_VECTOR);
            }
            else
            {
                validInput = 1;
            }
        }
    }
}

bool checkConvergence(int equationNumber, double **coefficients)
{
    for (int i = 0; i < equationNumber; i++)
    {
        double sum = 0.0;

        for (int j = 0; j < equationNumber; j++)
        {
            if (i != j)
            {
                sum += fabs(coefficients[i][j]);
            }
        }

        if (fabs(coefficients[i][i]) <= sum)
        {
            return false;
        }
    }

    return true;
}

int solveSystem(int equationNumber, double **coefficients, double *constantTerms, double *x, double epsilon)
{
    double *xp = malloc(equationNumber * sizeof(double));
    if (xp == NULL)
    {
        return -1;
    }

    for (int i = 0; i < equationNumber; i++)
    {
        xp[i] = constantTerms[i] / coefficients[i][i];
    }

    double maxDelta;
    do
    {
        maxDelta = 0.0;

        for (int i = 0; i < equationNumber; i++)
        {
            x[i] = constantTerms[i];
            for (int j = 0; j < equationNumber; j++)
            {
                if (i != j)
                {
                    x[i] -= coefficients[i][j] * xp[j];
                }
            }
            x[i] /= coefficients[i][i];

            double delta = fabs(x[i] - xp[i]);
            if (delta > maxDelta)
            {
                maxDelta = delta;
            }

            xp[i] = x[i];
        }
    } while (maxDelta >= epsilon);

    free(xp);

    return 0;
}

void clearAllocatedMemory(int equationNumber, double **coefficients, double *constantTerms, double *results)
{
    for (int i = 0; i < equationNumber; i++)
    {
        free(coefficients[i]);
    }

    free(coefficients);
    free(constantTerms);
    free(results);
}

bool askToContinue()
{
    printf("Do you want to run program again? Press 'y' to continue or any other key to exit: ");

    return getchar() == 'y';
}

double truncateNumber(double value, int decimalPlaces)
{
    double factor = pow(10.0, (double)decimalPlaces);

    return trunc(value * factor) / factor;
}

int getDecimalPlaces(double epsilon)
{
    int decimalPlaces = 0;

    while (epsilon < 1.0 && decimalPlaces < 15)
    {
        epsilon *= 10.0;
        decimalPlaces++;
    }

    return decimalPlaces;
}

void printResults(double epsilon, int equationNumber, double* x) {
    int decimalPlaces = getDecimalPlaces(epsilon);

    printf("Results:\n");
    for (int i = 0; i < equationNumber; i++)
    {
        printf("x[%d] = %.*lf\n", i, decimalPlaces, truncateNumber(x[i], decimalPlaces));
    }
}