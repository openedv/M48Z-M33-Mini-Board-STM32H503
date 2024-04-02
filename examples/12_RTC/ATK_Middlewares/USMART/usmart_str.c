/**
 ****************************************************************************************************
 * @file        usmart_str.c
 * @author      ALIENTEK
 * @brief       USMART code
 * @license     Copyright (C) 2012-2024, ALIENTEK
 ****************************************************************************************************
 * @attention
 *
 * platform     : ALIENTEK M48-STM32H503 board
 * website      : www.alientek.com
 * forum        : www.openedv.com/forum.php
 *
 * change logs  ：
 * version      data         notes
 * V1.0         20240309     the first version
 *
 ****************************************************************************************************
 */

#include "usmart.h"
#include "usmart_str.h"

/**
 * @brief   compares the strings str1 and str2
 * @param   str1 : first address of string 1 (pointer)
 * @param   str2 : first address of string 2 (pointer)
 * @retval  0, equal; 1, not equal;
 */
uint8_t usmart_strcmp(char *str1, char *str2)
{
    while (1)
    {
        if (*str1 != *str2)return 1; /* unequal */

        if (*str1 == '\0')break;    /* The comparison is done. */

        str1++;
        str2++;
    }

    return 0;/* Two strings are equal */
}

/**
 * @brief   copies the contents of src to dst
 * @param   src : source address
 * @param   dst : destination address
 * @retval  0, equal; 1, not equal;
 */
void usmart_strcopy(char *src, char *dst)
{
    while (1)
    {
        *dst = *src;            /* copy */

        if (*src == '\0')break; /* The copy is complete. */

        src++;
        dst++;
    }
}

/**
 * @brief   Gets the length of the string in bytes.
 * @param   str : String pointer
 * @retval  The length of the string
 */
uint8_t usmart_strlen(char *str)
{
    uint8_t len = 0;

    while (1)
    {
        if (*str == '\0')break; /* The copy is complete. */

        len++;
        str++;
    }

    return len;
}

/**
 * @brief   Squared function, m to the n
 * @param   m : The base
 * @param   n : The index
 * @retval  m to the n
 */
uint32_t usmart_pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;

    while (n--)result *= m;

    return result;
}

/**
 * @brief   Converts the string to a number
 * @note
 *          Hexadecimal conversion is supported, but only if the hexadecimal letter is uppercase and the format begins with 0X.
 *          Supports negative numbers
 * @param   str : String pointer
 * @param   res : Where the conversion will be stored.
 * @retval  results in the following:
 * @arg     0, conversion completed successfully
 * @arg     1, data format error
 * @arg     2, the hexadecimal digit is 0
 * @arg     3, the starting format is wrong
 * @arg     4, with a decimal digit of 0
 */
uint8_t usmart_str2num(char *str, uint32_t *res)
{
    uint32_t t;
    int tnum;
    uint8_t bnum = 0;       /* Number of bits */
    char *p;
    uint8_t hexdec = 10;    /* The default is decimal data */
    uint8_t flag = 0;       /* 0, there is no symbol to mark. 1, which represents a positive number 2 is a negative number. */
    p = str;
    *res = 0;

    while (1)
    {
        /* Parameter validity checking */
        if ((*p <= '9' && *p >= '0') || ((*str == '-' || *str == '+') && bnum == 0) || (*p <= 'F' && *p >= 'A') || (*p == 'X' && bnum == 1))
        {
            if (*p >= 'A')hexdec = 16;  /* There are letters in the string, in hexadecimal format. */

            if (*str == '-')
            {
                flag = 2;   /* Offset the sign */
                str += 1;
            }
            else if (*str == '+')
            {
                flag = 1;   /* Offset the sign */
                str += 1;
            }
            else
            {
                bnum++;     /* The number of bits increased. */
            }
        }
        else if (*p == '\0')
        {
            break;         /* At the end, exit */
        }
        else
        {
            return 1;      /* Not all decimal or hexadecimal data. */
        }

        p++;
    }

    p = str;               /* Relocates to the start of the string. */

    if (hexdec == 16)      /* Hexadecimal data */
    {
        if (bnum < 3)return 2;  /* If the number of bits is less than 3, exit. Because 0X takes up 2 of them,
                                 * if 0X is not followed by data, the data is invalid. */

        if (*p == '0' && (*(p + 1) == 'X'))   /* It must start with '0X'. */
        {
            p += 2;        /* Offset to the data start address. */
            bnum -= 2;     /* Subtract the offset */
        }
        else
        {
            return 3;      /* The starting header is not in the right format */
        }
    }
    else if (bnum == 0)
    {
        return 4;          /* If the number of bits is 0, exit directly */
    }

    while (1)
    {
        if (bnum)bnum--;

        if (*p <= '9' && *p >= '0')t = *p - '0';  /* Get the value of the number */
        else t = *p - 'A' + 10;                   /* The corresponding values of A~F are obtained */

        *res += t * usmart_pow(hexdec, bnum);
        p++;

        if (*p == '\0')break;                     /* done with the data */
    }

    if (flag == 2)                                /* Is it a negative number? */
    {
        tnum = -*res;
        *res = tnum;
    }

    return 0;                                     /* Successful conversion. */
}

/**
 * @brief   gets the instruction name
 * @param   str     : The source string
 * @param   cmdname : Instruction name
 * @param   nlen    : The length of the instruction name
 * @param   maxlen  : Maximum length (to limit, instructions can't be too long)
 * @retval  0, success; Others, fail.
 */
uint8_t usmart_get_cmdname(char *str, char *cmdname, uint8_t *nlen, uint8_t maxlen)
{
    *nlen = 0;

    while (*str != ' ' && *str != '\0') /* If a space or terminator is found, it is considered over */
    {
        *cmdname = *str;
        str++;
        cmdname++;
        (*nlen)++;                      /* Counting command length */

        if (*nlen >= maxlen)return 1;   /* Wrong instruction */
    }

    *cmdname = '\0';                    /* Adding a terminator */
    return 0;                           /* Return as normal */
}

/**
 * @brief   gets the next character (when there are a lot of Spaces in between, this function simply ignores the space and finds the first character after the space)
 * @param   str : String pointer
 * @retval  The next character
 */
uint8_t usmart_search_nextc(char *str)
{
    str++;

    while (*str == ' ' && str != 0)str++;

    return *str;
}

/**
 * @brief   gets the function name from str
 * @param   str   : The source string pointer
 * @param   fname : The obtained pointer to the function name
 * @param   pnum  : The number of arguments in the function
 * @param   rval  : Should the return value be displayed (0, no; 1, need)
 * @retval  0, success; Other, error code.
 */
uint8_t usmart_get_fname(char *str, char *fname, uint8_t *pnum, uint8_t *rval)
{
    uint8_t res;
    uint8_t fover = 0;  /* Bracket depth */
    char *strtemp;
    uint8_t offset = 0;
    uint8_t parmnum = 0;
    uint8_t temp = 1;
    char fpname[6];     /* void+X+'/0' */
    uint8_t fplcnt = 0; /* The length counter of the first argument */
    uint8_t pcnt = 0;   /* Parameter counters */
    uint8_t nchar;

    /* Determining whether a function returns a value */
    strtemp = str;

    while (*strtemp != '\0')    /* Do not end. */
    {
        if (*strtemp != ' ' && (pcnt & 0X7F) < 5)   /* A maximum of 5 characters is recorded */
        {
            if (pcnt == 0)pcnt |= 0X80;       /* When the most significant bit is set, the flag begins to receive the return type */

            if (((pcnt & 0x7f) == 4) && (*strtemp != '*'))break;    /* The last character, must be * */

            fpname[pcnt & 0x7f] = *strtemp;   /* Keep track of the return type of the function */
            pcnt++;
        }
        else if (pcnt == 0X85)
        {
            break;
        }

        strtemp++;
    }

    if (pcnt)           /* Completion of reception */
    {
        fpname[pcnt & 0x7f] = '\0'; /* Adding a terminator */

        if (usmart_strcmp(fpname, "void") == 0)
        {
            *rval = 0;  /* No return value is required */
        }
        else
        {
            *rval = 1;  /* Need to return a value */
        }

        pcnt = 0;
    }

    res = 0;
    strtemp = str;

    while (*strtemp != '(' && *strtemp != '\0')   /* This code finds the real starting position of the function name */
    {
        strtemp++;
        res++;

        if (*strtemp == ' ' || *strtemp == '*')
        {
            nchar = usmart_search_nextc(strtemp);   /* Gets the next character */

            if (nchar != '(' && nchar != '*')offset = res;  /* Skip Spaces and * signs */
        }
    }

    strtemp = str;

    if (offset)strtemp += offset + 1;   /* Skip to the beginning of the function name */

    res = 0;
    nchar = 0;  /* Whether the flag is in the string,0, is not in the string; 1, in the string; */

    while (1)
    {
        if (*strtemp == 0)
        {
            res = USMART_FUNCERR;   /* Function errors */
            break;
        }
        else if (*strtemp == '(' && nchar == 0)
        {
            fover++;                /* Bracket depth is increased by one level */
        }
        else if (*strtemp == ')' && nchar == 0)
        {
            if (fover)
            {
                fover--;
            }
            else
            {
                res = USMART_FUNCERR;  /* End error, no '(' */
            }

            if (fover == 0)break;      /* At the end, exit */
        }
        else if (*strtemp == '"')
        {
            nchar = !nchar;
        }

        if (fover == 0)                /* We're not done with function names yet */
        {
            if (*strtemp != ' ')       /* Spaces are not part of function names */
            {
                *fname = *strtemp;     /* Get the function name */
                fname++;
            }
        }
        else                           /* You're done accepting the function name. */
        {
            if (*strtemp == ',')
            {
                temp = 1;              /* Enable to add a parameter */
                pcnt++;
            }
            else if (*strtemp != ' ' && *strtemp != '(')
            {
                if (pcnt == 0 && fplcnt < 5)    /* When the first argument comes, a judgment must be made in order to avoid counting arguments of type void. */
                {
                    fpname[fplcnt] = *strtemp;  /* Record the parameter characteristics. */
                    fplcnt++;
                }

                temp++;         /* Get valid arguments (not Spaces) */
            }

            if (fover == 1 && temp == 2)
            {
                temp++;         /* Prevent repeated increases */
                parmnum++;      /* Adding a parameter */
            }
        }

        strtemp++;
    }

    if (parmnum == 1)           /* There is only one parameter. */
    {
        fpname[fplcnt] = '\0';  /* Adding a terminator */

        if (usmart_strcmp(fpname, "void") == 0)parmnum = 0; /* The argument is void, meaning there is no argument. */
    }

    *pnum = parmnum;            /* Record the number of parameters */
    *fname = '\0';              /* Adding a terminator */
    return res;                 /* Return the result of execution */
}

/**
 * @brief   gets a function argument from str
 * @param   str   : The source string pointer
 * @param   fparm : Parameter string pointer
 * @param   ptype : The type of the parameter
 * @arg     0, number
 * @arg     1, a string
 * @arg     0XFF, argument error
 * @retval
 * @arg     0, no more arguments
 * @arg     Other, the offset of the next argument.
 */
uint8_t usmart_get_aparm(char *str, char *fparm, uint8_t *ptype)
{
    uint8_t i = 0;
    uint8_t enout = 0;
    uint8_t type = 0;   /* The default is a number. */
    uint8_t string = 0; /* Flag whether str is reading */

    while (1)
    {
        if (*str == ',' && string == 0)enout = 1;   /* The immediate exit is deferred in order to find the start address of the next argument. */

        if ((*str == ')' || *str == '\0') && string == 0)break; /* Exit identifier immediately */

        if (type == 0)   /* The default is numeric */
        {
            /* Digit string detection */
            if ((*str >= '0' && *str <= '9') || *str == '-' || *str == '+' || (*str >= 'a' && *str <= 'f') || (*str >= 'A' && *str <= 'F') || *str == 'X' || *str == 'x')
            {
                if (enout)break;          /* When the next argument is found, exit. */

                if (*str >= 'a')
                {
                    *fparm = *str - 0X20; /* Convert lowercase to uppercase */
                }
                else
                {
                    *fparm = *str;        /* Lowercase or number stays the same */
                }

                fparm++;
            }
            else if (*str == '"')         /* Find the start flag of the string */
            {
                if (enout)break;          /* I found it, I found it, "and I thought it was over. */

                type = 1;
                string = 1;               /* Register that STRING is being read */
            }
            else if (*str != ' ' && *str != ',')  /* Invalid character found, parameter error */
            {
                type = 0XFF;
                break;
            }
        }
        else                      /* The string class */
        {
            if (*str == '"')string = 0;

            if (enout)break;      /* When the next argument is found, exit. */

            if (string)           /* String reading */
            {
                if (*str == '\\') /* Escape character encountered (do not copy escape character) */
                {
                    str++;        /* Offset to the character after the escape character, whatever character, just COPY */
                    i++;
                }

                *fparm = *str;    /* Lowercase or number stays the same */
                fparm++;
            }
        }

        i++;                     /* The offset is increased */
        str++;
    }

    *fparm = '\0';               /* Adding a terminator */
    *ptype = type;               /* Return parameter type */
    return i;                    /* Return the length of the argument */
}

/**
 * @brief   Gets the start address of the specified parameter
 * @param   num : The num-th argument in the range 0-9.
 * @retval  The start address of the parameter
 */
uint8_t usmart_get_parmpos(uint8_t num)
{
    uint8_t temp = 0;
    uint8_t i;

    for (i = 0; i < num; i++)
    {
        temp += usmart_dev.plentbl[i];
    }

    return temp;
}

/**
 * @brief  gets the function arguments from str
 * @param  str  : The source string
 * @param  parn : The number of arguments.0 means void with no arguments
 * @retval 0, success; Other, error code.
 */
uint8_t usmart_get_fparam(char *str, uint8_t *parn)
{
    uint8_t i, type;
    uint32_t res;
    uint8_t n = 0;
    uint8_t len;
    char tstr[PARM_LEN + 1]; /* The byte cache can hold strings of up to PARM_LEN characters */

    for (i = 0; i < MAX_PARM; i++)
    {
        usmart_dev.plentbl[i] = 0;  /* Clear the parameter length table */
    }

    while (*str != '(')   /* Offset to the beginning of the argument */
    {
        str++;

        if (*str == '\0')return USMART_FUNCERR; /* We've hit a terminator */
    }

    str++;                /* Offset to the first byte after "(" */

    while (1)
    {
        i = usmart_get_aparm(str, tstr, &type); /* Get the first argument */
        str += i;         /* Offset */

        switch (type)
        {
            case 0:       /* number */
                if (tstr[0] != '\0')    /* The arguments received are valid */
                {
                    i = usmart_str2num(tstr, &res);  /* Record the parameter */

                    if (i)return USMART_PARMERR;     /* Parameter error. */

                    *(uint32_t *)(usmart_dev.parm + usmart_get_parmpos(n)) = res;   /* Record the result of a successful transformation. */
                    usmart_dev.parmtype &= ~(1 << n);/* Marking numbers */
                    usmart_dev.plentbl[n] = 4;       /* The length of this parameter is 4 */
                    n++;                             /* Increase in parameters */

                    if (n > MAX_PARM)return USMART_PARMOVER;  /* parameter number too much */
                }

                break;

            case 1:       /* string */
                len = usmart_strlen(tstr) + 1;  /* Includes the terminator '\0' */
                usmart_strcopy(tstr, (char *)&usmart_dev.parm[usmart_get_parmpos(n)]);  /* Copy tstr data to usmart dev.parm[n] */
                usmart_dev.parmtype |= 1 << n;  /* Tag string */
                usmart_dev.plentbl[n] = len;    /* The length of this parameter is len */
                n++;

                if (n > MAX_PARM)return USMART_PARMOVER;    /* parameter number too much */

                break;

            case 0XFF:    /* Error */
                return USMART_PARMERR;          /* parameter error */
        }

        if (*str == ')' || *str == '\0')break;  /* We got the end sign. */
    }

    *parn = n;                                 /* Record the number of parameters */
    return USMART_OK;                          /* The parameters are correctly obtained */
}














