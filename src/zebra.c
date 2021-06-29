
typedef enum
{
    Red,
    Green,
    Ivory,
    Yellow,
    Blue,
    ColorUndefined,
} Color;

typedef enum
{
    English,
    Spanish,
    Ukrainian,
    Norwegian,
    Japanese,
    NationalityUndefined,
} Nationality;

typedef enum
{
    Dog,
    Snail,
    Fox,
    Horse,
    Zebra,
    PetUndefined,
} Pet;

typedef enum
{
    Coffee,
    Tea,
    Milk,
    OrangeJuice,
    Water,
    BeverageUndefined,
} Beverage;

typedef enum
{
    OldGold,
    Kools,
    Chesterfield,
    LuckyStrike,
    Parliment,
    CigaretteUndefined,
} Cigarette;

typedef struct
{
    Color       color;
    Nationality owner;
    Pet         pet;
    Beverage    beverage;
    Cigarette   cigs;
} House;

static const int HOUSES = 5;

typedef House Street [HOUSES];

typedef int (*checkConstraint)(Street street);

static int ruleTwo(Street street);
static int ruleThree(Street street);
static int ruleFour(Street street);
static int ruleFive(Street street);
static int ruleSix(Street street);
static int ruleSeven(Street street);
static int ruleEight(Street street);
static int ruleNine(Street street);
static int ruleTen(Street street);
static int ruleEleven(Street street);
static int ruleTwelwe(Street street);
static int ruleThirteen(Street street);
static int ruleFourteen(Street street);
static int ruleFifteen(Street street);

Street street = {
    {
        .color    = ColorUndefined,
        .owner    = Norwegian,
        .pet      = PetUndefined,
        .beverage = BeverageUndefined,
        .cigs     = CigaretteUndefined,
    },
    {
        .color    = ColorUndefined,
        .owner    = NationalityUndefined,
        .pet      = PetUndefined,
        .beverage = BeverageUndefined,
        .cigs     = CigaretteUndefined,
    },
    {
        .color    = ColorUndefined,
        .owner    = NationalityUndefined,
        .pet      = PetUndefined,
        .beverage = Milk,
        .cigs     = CigaretteUndefined,
    },
    {
        .color    = ColorUndefined,
        .owner    = NationalityUndefined,
        .pet      = PetUndefined,
        .beverage = BeverageUndefined,
        .cigs     = CigaretteUndefined,
    },
    {
        .color    = ColorUndefined,
        .owner    = NationalityUndefined,
        .pet      = PetUndefined,
        .beverage = BeverageUndefined,
        .cigs     = CigaretteUndefined,
    },
};

static const checkConstraint CONSTRAINTS [] =
{
    ruleTwo,
    ruleThree,
    ruleFour,
    ruleFive,
    ruleSix,
    ruleSeven,
    ruleEight,
    ruleNine,
    ruleTen,
    ruleEleven,
    ruleTwelwe,
    ruleThirteen,
    ruleFourteen,
    ruleFifteen,
};

static int ruleTwo(Street street)
{
    for (int i = 0; i < HOUSES; ++i)
    {
        if (Red == street[i].color)
        {
            return English == street[i].owner;
        }
    }
}

static int ruleThree(Street street)
{
    for (int i = 0; i < HOUSES; ++i)
    {
        if (Dog == street[i].pet)
        {
            return Spanish == street[i].owner;
        }
    }
}

static int ruleFour(Street street)
{
    for (int i = 0; i < HOUSES; ++i)
    {
        if (Green == street[i].color)
        {
            return Coffee == street[i].beverage;
        }
    }
}

static int ruleFive(Street street)
{
    for (int i = 0; i < HOUSES; ++i)
    {
        if (Ukrainian == street[i].owner)
        {
            return Tea == street[i].beverage;
        }
    }
}

static int ruleSix(Street street)
{
    for (int i = 0; i < HOUSES; ++i)
    {
        if (Ivory == street[i].color)
        {
            if (i < HOUSES - 1)
            {
                return street[i + 1].color == Green;
            }
            else
            {
                return 0;
            }
        }
    }
}

static int ruleSeven(Street street)
{
    for (int i = 0; i < HOUSES; ++i)
    {
        if (OldGold == street[i].cigs)
        {
            return Snail == street[i].pet;
        }
    }
}

static int ruleEight(Street street)
{
    for (int i = 0; i < HOUSES; ++i)
    {
        if (Kools == street[i].cigs)
        {
            return Yellow == street[i].color;
        }
    }
}

static int ruleNine(Street street)
{
    return street[2].beverage == Milk;
}

static int ruleTen(Street street)
{
    return street[0].owner == Norwegian;
}

static int ruleEleven(Street street)
{
    for (int i = 0; i < HOUSES; ++i)
    {
        if (Chesterfield == street[i].cigs)
        {
            if (0 == i)
            {
                return street[i + 1].pet == Fox;
            }
            else if (HOUSES - 1 == i)
            {
                return street[i - 1].pet == Fox;
            }
            else
            {
                return street[i - 1].pet == Fox || street[i + 1].pet == Fox;
            }
        }
    }
}

static int ruleTwelwe(Street street)
{
    for (int i = 0; i < HOUSES; ++i)
    {
        if (Kools == street[i].cigs)
        {
            if (0 == i)
            {
                return street[i + 1].pet == Horse;
            }
            else if (HOUSES - 1 == i)
            {
                return street[i - 1].pet == Horse;
            }
            else
            {
                return street[i - 1].pet == Horse || street[i + 1].pet == Horse;
            }
        }
    }
}

static int ruleThirteen(Street street)
{
    for (int i = 0; i < HOUSES; ++i)
    {
        if (LuckyStrike == street[i].cigs)
        {
            return OrangeJuice == street[i].beverage;
        }
    }
}

static int ruleFourteen(Street street)
{
    for (int i = 0; i < HOUSES; ++i)
    {
        if (Parliment == street[i].cigs)
        {
            return Japanese == street[i].owner;
        }
    }
}

static int ruleFifteen(Street street)
{
    for (int i = 0; i < HOUSES; ++i)
    {
        if (Norwegian == street[i].owner)
        {
            if (0 == i)
            {
                return street[i + 1].color == Blue;
            }
            else if (HOUSES - 1 == i)
            {
                return street[i - 1].color == Blue;
            }
            else
            {
                return street[i - 1].color == Blue || street[i + 1].color == Blue;
            }
        }
    }
}

int checkConstraints(Street street)
{
    for (int i = 0; i < sizeof(CONSTRAINTS) / sizeof(*CONSTRAINTS); ++i)
    {
        if (!CONSTRAINTS[i](street))
        {
            return false;
        }
    }
    return true;
}
