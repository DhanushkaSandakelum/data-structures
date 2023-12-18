// Math Utility
long long power(int base, int exponent)
{
    if (exponent < 0)
    {
        // Handle negative exponent if needed
        return 0; // You may want to return an error or handle it differently
    }

    long long result = 1;

    for (int i = 0; i < exponent; ++i)
    {
        result *= base;
    }

    return result;
}
