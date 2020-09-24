#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define SIGN_BIT_IDX 63

#define EXP_START_BIT_IDX 52
#define EXP_END_BIT_IDX 62

#define FRAC_START_BIT_IDX 0
#define FRAC_END_BIT_IDX 51

#define FULL_EXP  0x00000000000007FF // Exponent of 11 1-s
#define FULL_FRAC 0x000FFFFFFFFFFFFF // Fraction of 52 1-s

#define PLUS_ZERO  0x0000000000000000
#define MINUS_ZERO 0x8000000000000000
#define PLUS_INF   0x7FF0000000000000
#define MINUS_INF  0xFFF0000000000000


/**
 * Library-level functions.
 * You should use them in the main sections.
 */

uint64_t convertToUint64 (double number) {
    return *((uint64_t *)(&number));
}

bool getBit (const uint64_t number, const uint8_t index) {
    uint64_t mask = 1ull << index;
    return mask & number;
}

/**
 *
 * @param start is starting index of the mask (inclusive)
 * @param end is ending index of the mask (inclusive)
 * @return mask that starts at 'start' and ends at 'end'
 */
uint64_t getMask(const uint8_t start, const uint8_t end) {
    uint64_t mask = 0ull;
    for(uint8_t i = start; i <= end; ++ i) {
        mask |= 1ull << i;
    }
    return mask;
}

/**
 *
 * @param number
 * @param start
 * @param end
 * @return all bits from 'number' between 'start' and 'end' (inclusive)
 */
uint64_t getBits(const uint64_t number, const uint64_t start, const uint64_t end) {
    return (number & getMask(start, end)) >> start;
}

uint64_t getExponent(const uint64_t number) {
    return getBits(number, EXP_START_BIT_IDX, EXP_END_BIT_IDX);
}

uint64_t getFraction(const uint64_t number) {
    return getBits(number, FRAC_START_BIT_IDX, FRAC_END_BIT_IDX);
}

bool checkNegative(const uint64_t number) {
    return getBit(number, SIGN_BIT_IDX);
}

// denormalized exponent is always 0
bool checkExponentDenormalized(const uint64_t number) {
    return getExponent(number) == 0ull;
}

//normalized exponent is bigger than 0, but smaller than all 1s
bool checkExponentNormalized(const uint64_t number) {
    uint64_t exp = getExponent(number);
    return exp > 0ull && exp < FULL_EXP;
}

// NaN exp is full 1s
bool checkExponentNan(const uint64_t number) {
    return getExponent(number) == FULL_EXP;
}

// denormalized fraction can't be 0, since it would make double a zero
bool checkFractionDenormalized(const uint64_t number) {
    return getFraction(number) > 0ull;
}

bool checkFractionSNaN(const uint64_t number) {
    uint64_t fraction = getFraction(number);
    return fraction > 0ull && fraction < FULL_FRAC;
}

//first bit of QNaN fraction should be 1, others are arbitrary
bool checkFractionQNaN(const uint64_t number) {
    return getBit(number, FRAC_END_BIT_IDX);
}


/**
 * Checkers here:
 */

bool checkForPlusZero (uint64_t number) {
    return number == PLUS_ZERO;
}

bool checkForMinusZero (uint64_t number) {
    return number == MINUS_ZERO;
}

bool checkForPlusInf (uint64_t number) {
    return number == PLUS_INF;
}

bool checkForMinusInf (uint64_t number) {
    return number == MINUS_INF;
}

bool checkForPlusNormal (uint64_t number) {
    return !checkNegative(number) && checkExponentNormalized(number);
}

bool checkForMinusNormal (uint64_t number) {
    return checkNegative(number) && checkExponentNormalized(number);
}

bool checkForPlusDenormal (uint64_t number) {
    return !checkNegative(number) && checkExponentDenormalized(number) && checkFractionDenormalized(number);
}

bool checkForMinusDenormal (uint64_t number) {
    return checkNegative(number) && checkExponentDenormalized(number) && checkFractionDenormalized(number);
}

bool checkForSignalingNan (uint64_t number) {
    return checkExponentNan(number) && checkFractionSNaN(number);
}

bool checkForQuietNan (uint64_t number) {
    return checkExponentNan(number) && checkFractionQNaN(number);
}

void classify (double number) {
    if (checkForPlusZero(convertToUint64(number))) {
        printf("Plus zero\n");
    }

    else if (checkForMinusZero(convertToUint64(number))) {
        printf("Minus zero\n");
    }

    else if (checkForPlusInf(convertToUint64(number))) {
        printf("Plus inf\n");
    }

    else if (checkForMinusInf(convertToUint64(number))) {
        printf("Minus inf\n");
    }

    else if (checkForPlusNormal(convertToUint64(number))) {
        printf("Plus normal\n");
    }

    else if (checkForMinusNormal(convertToUint64(number))) {
        printf("Minus normal\n");
    }

    else if (checkForPlusDenormal(convertToUint64(number))) {
        printf("Plus Denormal\n");
    }

    else if (checkForMinusDenormal(convertToUint64(number))) {
        printf("Minus Denormal\n");
    }

    else if (checkForSignalingNan(convertToUint64(number))) {
        printf("Signailing NaN\n");
    }

    else if (checkForQuietNan(convertToUint64(number))) {
        printf("Quiet NaN\n");
    }

    else {
        printf("Error.\n");
    }
}