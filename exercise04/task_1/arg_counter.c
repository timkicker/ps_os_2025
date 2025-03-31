#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

enum errorCodes
{
    noArgs = 13,
    tooManyArgs = 7, //./arg_counter "January@10Ab" "FebruAry!3C" "March$7D" "April%5E" "May^6F" "June&8G" "July*4H" "August(9I" "September)11J" "October#12K" "November+13L" "December-14M" "III_15He"
    invalidOFFSET = 99,
    // see https://neal.fun/password-game/ for rule inspiration
    minFiveArgs = 2, //the length of each arg must be greater or equal to 5
    incNumArgs = 3, //each arg must have a number
    incUpperArgs = 4, //each arg must have a uppercase letter from the english alphabet
    incSpecialArgs = 5, //each arg must include a special ASCII character which is neither a letter nor number (e.g. @, $, %)
    addTwentyFiveArgs = 6, //the sum of all numbers in all args together must be greater than 25
    incMonthArgs = 8, //at least one arg must contain a month
    incRomanArgs = 9, //at least one arg must contain a roman number
    incElemArgs = 10, //at least one arg must contain a element in its letter notation ./arg_counter "July@5xxX" "LM*1ßß" "-2099M"

    //OFFSET=5 ./arg_counter "JULy@15xxX" "DecembeR*10" "IV-2099Mg"
    //or
    //make test

};

bool hasNumber(const char *str)
{
    while (*str)
    {
        if (isdigit(*str))
            return true;
        str++;
    }
    return false;
}

bool hasUpperCase(const char *str)
{
    while (*str)
    {
        if (isupper(*str))
            return true;
        str++;
    }
    return false;
}

bool hasSpecialChar(const char *str)
{
    while (*str)
    {
        if (!isalnum(*str))
            return true;
        str++;
    }
    return false;
}

int sumNumbers(const char *str)
{
    int sum = 0;
    int current = 0;
    while (*str)
    {
        if (isdigit(*str))
        {
            //for multi digit number addition
            current = current * 10 + (*str - '0');
        }
        else
        {
            sum += current;
            current = 0;
        }
        str++;
    }
    sum += current;
    return sum;
}

bool containsMonth(const char *str)
{
    const char *months[] = {"january", "february", "march", "april", "may", "june",
                            "july", "august", "september", "october", "november", "december"};
    char lower[100];
    unsigned long i = 0;
    while (str[i] && i < sizeof(lower) - 1)
    {
        lower[i] = tolower(str[i]);
        i++;
    }
    lower[i] = '\0';

    for (int j = 0; j < 12; j++)
    {
        if (strstr(lower, months[j]))
        {
            return true;
        }
    }
    return false;
}

bool containsRomanNumeral(const char *str)
{
    const char *numerals[] = {"i", "ii", "iii", "iv", "v", "vi", "vii", "viii", "ix", "x"};
    char lower[100];
    for (int i = 0; str[i]; i++)
    {
        lower[i] = tolower(str[i]);
        lower[i + 1] = '\0';
    }

    for (int i = 0; i < 10; i++)
    {
        if (strstr(lower, numerals[i]))
            return true;
    }
    return false;
}

bool containsElementSymbol(const char *str) {
    //correct element symbols (case-sensitive)
    const char *elements[] = {
        "H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar",
        "K", "Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br", "Kr",
        "Rb", "Sr", "Y", "Zr", "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te", "I", "Xe",
        "Cs", "Ba", "La", "Ce", "Pr", "Nd", "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu",
        "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg", "Tl", "Pb", "Bi", "Po", "At", "Rn",
        "Fr", "Ra", "Ac", "Th", "Pa", "U", "Np", "Pu", "Am", "Cm", "Bk", "Cf", "Es", "Fm", "Md", "No", "Lr",
        "Rf", "Db", "Sg", "Bh", "Hs", "Mt", "Ds", "Rg", "Cn", "Nh", "Fl", "Mc", "Lv", "Ts", "Og"};

    for (size_t i = 0; i < sizeof(elements) / sizeof(elements[0]); i++) {
        const char *found = strstr(str, elements[i]);
        while (found != NULL) {
            size_t len = strlen(elements[i]);
            if (
                //case 1: full string is just the element
                (found == str && found[len] == '\0') ||
                //case 2: element is at the start
                (found == str && !isalpha(found[len])) ||
                //case 3: element is in the middle/end
                (!isalpha(found[-1]) && !isalpha(found[len]))
            ) {
                return true;
            }
            found = strstr(found + 1, elements[i]);
        }
    }
    return false;
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("Usage: %s [arguments...]\n", argv[0]);
        return noArgs;
    }

    if (argc > 11)
    {
        return tooManyArgs;
    }

    int offset = 0;
    char *offsetStr = getenv("OFFSET");
    if (offsetStr != NULL)
    {
        char *end;
        long tempOffset = strtol(offsetStr, &end, 10);

        if (*end != '\0')
        {
            return invalidOFFSET;
        }

        offset = (int)tempOffset;
    }

    bool hasMonth = false;
    bool hasRoman = false;
    bool hasElement = false;

    int totalSum = 0;
    for (int i = 1; i < argc; i++)
    {

        if (strlen(argv[i]) < 5)
        {
            return minFiveArgs;
        }

        if (!hasNumber(argv[i]))
        {
            return incNumArgs;
        }

        if (!hasUpperCase(argv[i]))
        {
            return incUpperArgs;
        }

        if (!hasSpecialChar(argv[i]))
        {
            return incSpecialArgs;
        }

        totalSum += sumNumbers(argv[i]);

        if (!hasMonth && containsMonth(argv[i]))
        {
            hasMonth = true;
        }

        if (!hasRoman && containsRomanNumeral(argv[i]))
        {
            hasRoman = true;
        }

        if (!hasElement && containsElementSymbol(argv[i]))
        {
            hasElement = true;
        }
    }

    if (!hasMonth)
    {
        return incMonthArgs;
    }

    if (totalSum < 25)
    {
        return addTwentyFiveArgs;
    }

    if (!hasRoman)
    {
        return incRomanArgs;
    }

    if (!hasElement)
    {
        return incElemArgs;
    }

    int totalCount = (argc - 1) + offset;
    printf("Result: %d\n", totalCount);
    return EXIT_SUCCESS;
}
