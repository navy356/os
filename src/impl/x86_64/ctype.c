int isAlpha(char c)
{
    if((c >= 'a' && c <='z')||(c>='A' && c<='Z'))
    {
        return 1;
    }
    return 0;
}

int isDigit(char c)
{
    if(c >= '0' && c <= '9')
    {
        return 1;
    }
    return 0;
}

int isUpperCaseAlpha(char c)
{
    if(c>='A' && c<='Z')
    {
        return 1;
    }
    return 0;
}

int isLowerCaseAlpha(char c)
{
    if(c >= 'a' && c <='z')
    {
        return 1;
    }
    return 0;
}