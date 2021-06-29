
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef unsigned char digit;

typedef struct
{
    digit * digits;
    unsigned length;
    digit a;
} Generator;

static Generator * createGenerator(const char * pattern, digit a);
static void        deleteGenerator(Generator * generator);
static void        reset(Generator * generator);
static int         get(Generator * generator);
static int         next(Generator * generator);

#ifdef GENERATOR_TEST
static unsigned    combinationCount(Generator * generator);
static void generatorDemo(const char * pattern, digit a);
#endif

static Generator * createGenerator(const char * pattern, digit a)
{
    Generator * generator = NULL;

    if (a < 10)
    {
        generator = malloc(sizeof(Generator));
        if (NULL != generator)
        {
            generator->a = a;
            generator->length = strlen(pattern);
            generator->digits = NULL;
            if (generator->length > 0)
            {
                generator->digits = malloc(generator->length * sizeof(digit));
                if (NULL == generator->digits)
                {
                    free(generator);
                    generator = NULL;
                }
                else
                {
                    unsigned i;
                    for (i = 0; i < generator->length; ++i)
                    {
                        if ('A' == pattern[i])
                        {
                            generator->digits[i] = a;
                        }
                        else if (0 == i && generator->length > 1)
                        {
                            generator->digits[i] = 1;
                        }
                        else
                        {
                            generator->digits[i] = 0;
                        }
                    }
                }
            }
        }
    }

    return generator;
}

static void deleteGenerator(Generator * generator)
{
    if (NULL != generator)
    {
        if (NULL != generator->digits)
        {
            free(generator->digits);
        }
        free(generator);
    }
}

static void reset(Generator * generator)
{
    unsigned i;
    for (i = 0; i < generator->length; ++i)
    {
        if (generator->digits[i] != generator->a)
        {
            if (0 == i && generator->length > 1)
            {
                generator->digits[i] = 1;
            }
            else
            {
                generator->digits[i] = 0;
            }
        }
    }
}

static int get(Generator * generator)
{
    int number = 0;
    unsigned i;

    for (i = 0; i < generator->length; ++i)
    {
        number *= 10;
        number += generator->digits[i];
    }

    return number;
}

static int next(Generator * generator)
{
    int i;
    int carry = 1;

    for (i = (int) generator->length - 1; carry && (i >= 0); --i)
    {
        if (generator->digits[i] != generator->a)
        {
            ++generator->digits[i];
            if (generator->a == generator->digits[i])
            {
                ++generator->digits[i];
            }
            if (generator->digits[i] >= 10)
            {
                generator->digits[i] = 0;
            }
            else
            {
                carry = 0;
            }
        }
    }

    return carry == 1 ? 0 : 1;
}


#ifdef GENERATOR_TEST

static unsigned combinationCount(Generator * generator)
{
    unsigned count = 1;
    unsigned i;

    for (i = 0; i < generator->length; ++i)
    {
        if (generator->a != generator->digits[i])
        {
            count *= 10 - generator->digits[i];
        }
    }

    return count;
}

static void generatorDemo(const char * pattern, digit a)
{
    Generator * gen = createGenerator(pattern, a);
    unsigned count = combinationCount(gen);
    int i = 0;
    int dots = 0;

    printf("%s A := %d %d:\n", pattern, (int) a, (int) count);

    do
    {
        ++i;
        if (count > 1000000 && i > 10)
        {
            break;
        }
        else if (count < 100 || i <= 10 || i >= (count - 10))
        {
            printf("\t%d\n", get(gen));
        }
        else if (!dots)
        {
            printf("\t...\n");
            dots = 1;
        }
    }
    while (next(gen));
    printf("\n");

    deleteGenerator(gen);
}

#endif

void printMultiplyCombinations(char * multiplier, char * result)
{
    for (int A = 0; A < 10; ++A)
    {
        Generator * agen = createGenerator(multiplier, A);
        Generator * bgen = createGenerator(result, A);

        for (int c = 0; c < 10; ++c)
        {
            if (c == A)
            {
                continue;
            }
            for (int a = get(agen); next(agen); a = get(agen))
            {
                for (int b = get(bgen); next(bgen); b = get(bgen))
                {
                    if (a * c == b)
                    {
                        printf("%d * %d = %d\n", a, c, b);
                    }
                }
                reset(bgen);
            }
            reset(agen);
        }

        deleteGenerator(agen);
        deleteGenerator(bgen);
    }
}

void partOne()
{
    printf("\nPart one\n");
    printMultiplyCombinations(".A.", "..AA");
}

void partTwo()
{
    printf("\nPart two\n");
    printMultiplyCombinations(".A.", "..A");
}

void partThree()
{
    printf("\nPart three\n");
    for (int A = 0; A < 10; ++A)
    {
        Generator * agen = createGenerator(".A.", A);
        Generator * bgen = createGenerator(".A..", A);

        for (int a = get(agen); next(agen); a = get(agen))
        {
            for (int b = get(bgen); next(bgen); b = get(bgen))
            {
                if (A * a == b)
                {
                    printf("%d * %d = %d\n", (int) A, a, b);
                }
            }
            reset(bgen);
        }

        deleteGenerator(agen);
        deleteGenerator(bgen);
    }
}

void partFour()
{
    printf("\nPart four\n");
    int divisor = 484;
    digit A = 8;

    for (int result = 7280; result < 7290; ++result)
    {
        int dividend = result * divisor;
        int passes = 1;
        int divi = dividend;

        for (int digi = 0; passes && (digi < 7); ++digi)
        {
            digit digit = divi % 10;
            divi /= 10;

            if (2 == digi)
            {
                passes = (digit == A);
            }
            else
            {
                passes = (digit != A);
            }
        }
        if (passes)
        {
            printf("%d / %d = %d\n", dividend, divisor, result);
        }
    }
}

int main(int argc, char ** argv)
{

#ifdef GENERATOR_TEST
    generatorDemo(".A.", 3);
    generatorDemo("..AA", 2);
    generatorDemo("A", 0);
    generatorDemo(".", 7);
    generatorDemo("..", 7);
#endif

    /* we are solving:
     *
     *            xxAx
     *         --------
     *         xxxxAxx / xAx
     *         xxAA             (1)
     *        ---------
     *         xxxA
     *          xxA             (2)
     *        --------
     *          xxxx
     *          xAxx            (3)
     *        --------
     *           xxxx
     *           xxxx
     *          ------
     *           = =
    */

    /* print combinations for (1):
     *   x * xAx = xxAA   */
    partOne();

    /* print combinations for (2):
     *  x * xAx = xxA   */
    partTwo();

    /* print combinations for (3):
     *   A * xAx = xAxx  */
    partThree();

    /* was too lazy to programm this, but looking at
     * what was printed, we determine that
     *  divider = xAx = 484 => A = 8
     * and division result = xxAx = 728x  */

    /* print combinations:
     *  xxx8xx / 484 = 728x */
    partFour();

    /* ..there was only one :
     *    3527876 / 484 = 7289  */

    /* knowing the result, calculating missing numbers */
    int sub1 = 10 *(3527 - 3388) + 8;
    int sub2 = 10 * (sub1 - 968) + 7;
    int sub3 = 10 * (sub2 - 3872) + 6;

    printf("\n\n"
     "    7289       \n"
     " --------      \n"
     " 3527876 / 484 \n"
     " 3388          \n"
     "--------       \n"
     "  %d         \n"
     "   968         \n"
     " -------       \n"
     "   %d        \n"
     "   3872        \n"
     " -------       \n"
     "   %d        \n"
     "   %d        \n"
     "  ------       \n"
     "   = =\n\n", sub1, sub2, sub3, sub3);

    return 0;
}
