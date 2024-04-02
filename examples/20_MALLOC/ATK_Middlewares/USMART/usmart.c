/**
 ****************************************************************************************************
 * @file        usmart.c
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
#include "usmart_port.h"
#include "tim.h"


/* system command */
char *sys_cmd_tab[] =
{
    "?",
    "help",
    "list",
    "id",
    "hex",
    "dec",
    "runtime",
};

/**
 * @brief   handles system instructions
 * @param   str : String pointer
 * @retval  0, processed successfully; Other, error code
 */
uint8_t usmart_sys_cmd_exe(char *str)
{
    uint8_t i;
    char sfname[MAX_FNAME_LEN];                   /* Store local function names */
    uint8_t pnum;
    uint8_t rval;
    uint32_t res;
    res = usmart_get_cmdname(str, sfname, &i, MAX_FNAME_LEN);   /* Get the instruction and its length */

    if (res)return USMART_FUNCERR;                /* Wrong instruction */

    str += i;

    for (i = 0; i < sizeof(sys_cmd_tab) / 4; i++) /* Supported system commands */
    {
        if (usmart_strcmp(sfname, sys_cmd_tab[i]) == 0)break;
    }

    switch (i)
    {
        case 0:
        case 1: /* help command */
            USMART_PRINTF("\r\n");
#if USMART_USE_HELP
            USMART_PRINTF("------------------------USMART V3.5------------------------ \r\n");
            USMART_PRINTF("    USMART is developed by ALIENTEK a clever serial debugging intercom component, through \r\n");
            USMART_PRINTF("It allows you to call any function in the program through the serial assistant and execute it. So, you can\r\n");
            USMART_PRINTF("To arbitrarily change the input parameters of the function (support numbers (10/16 base, support negative numbers), strings\r\n"),
            USMART_PRINTF(", function entry address, etc. as arguments), a single function supports up to 10 input arguments, and supports\r\n"),
            USMART_PRINTF("Function return values are displayed. Support parameter display base setting function, support base conversion function.\r\n");
            USMART_PRINTF("technical support:www.openedv.com\r\n");
            USMART_PRINTF("USMART has seven system commands (must be lowercase):\r\n");
            USMART_PRINTF("?   :   Get help\r\n");
            USMART_PRINTF("help:   Get help\r\n");
            USMART_PRINTF("list:   A list of available functions\r\n\n");
            USMART_PRINTF("id:     A list of IDs of the available functions\r\n\n");
            USMART_PRINTF("hex:    Argument hexadecimal display, followed by the space + number is the execution of the base conversion\r\n\n");
            USMART_PRINTF("dec:    The argument is displayed in decimal, followed by a space + number to perform the base conversion\r\n\n");
            USMART_PRINTF("runtime:1, enable function run timing. 0, turns off the function run time.\r\n\n");
            USMART_PRINTF("Please enter the name and parameters of the function in the program format and end with the ENTER key.\r\n");
            USMART_PRINTF("--------------------------ALIENTEK------------------------- \r\n");
#else
            USMART_PRINTF("Instruction invalidation\r\n");
#endif
            break;

        case 2: /* Query instructions */
            USMART_PRINTF("\r\n");
            USMART_PRINTF("------------------------Function listing--------------------------- \r\n");

            for (i = 0; i < usmart_dev.fnum; i++)USMART_PRINTF("%s\r\n", usmart_dev.funs[i].name);

            USMART_PRINTF("\r\n");
            break;

        case 3: /* Query ID */
            USMART_PRINTF("\r\n");
            USMART_PRINTF("-------------------------Function ID --------------------------- \r\n");

            for (i = 0; i < usmart_dev.fnum; i++)
            {
                usmart_get_fname((char *)usmart_dev.funs[i].name, sfname, &pnum, &rval); /* Get the local function name */
                USMART_PRINTF("%s id is:\r\n0X%08X\r\n", sfname, (unsigned int)usmart_dev.funs[i].func);  /* Display ID */
            }

            USMART_PRINTF("\r\n");
            break;

        case 4: /* hex instruction */
            USMART_PRINTF("\r\n");
            usmart_get_aparm(str, sfname, &i);

            if (i == 0)     /* Parameters are normal */
            {
                i = usmart_str2num(sfname, &res);       /* Record the parameter */

                if (i == 0) /* Base conversion function */
                {
                    USMART_PRINTF("HEX:0X%X\r\n", (unsigned int)res); /* Convert to hexadecimal */
                }
                else if (i != 4)return USMART_PARMERR;  /* Parameter error. */
                else        /* Parameter display setting function */
                {
                    USMART_PRINTF("Hexadecimal parameter display!\r\n");
                    usmart_dev.sptype = SP_TYPE_HEX;
                }

            }
            else return USMART_PARMERR; /* Parameter error. */

            USMART_PRINTF("\r\n");
            break;

        case 5: /* dec directive */
            USMART_PRINTF("\r\n");
            usmart_get_aparm(str, sfname, &i);

            if (i == 0)     /* Parameters are normal */
            {
                i = usmart_str2num(sfname, &res);       /* Record the parameter */

                if (i == 0) /* Base conversion function */
                {
                    USMART_PRINTF("DEC:%lu\r\n", (unsigned long)res);  /* Converting to decimal */
                }
                else if (i != 4)
                {
                    return USMART_PARMERR;  /* Parameter error. */
                }
                else        /* Parameter display setting function */
                {
                    USMART_PRINTF("Decimal parameter display!\r\n");
                    usmart_dev.sptype = SP_TYPE_DEC;
                }

            }
            else 
            {
                return USMART_PARMERR;  /* Parameter error. */
            }
                
            USMART_PRINTF("\r\n");
            break;

        case 6: /* runtime directive, which sets whether the function execution time should be displayed */
            USMART_PRINTF("\r\n");
            usmart_get_aparm(str, sfname, &i);

            if (i == 0) /* Parameters are normal */
            {
                i = usmart_str2num(sfname, &res);   /* Log the argument */

                if (i == 0) /* Read data at specified location */
                {
                    if (USMART_ENTIMX_SCAN == 0)
                    {
                        USMART_PRINTF("\r\nError! \r\nTo EN RunTime function,Please set USMART_ENTIMX_SCAN = 1 first!\r\n");/* report an error */
                    }
                    else
                    {
                        usmart_dev.runtimeflag = res;

                        if (usmart_dev.runtimeflag)
                        {
                            USMART_PRINTF("Run Time Calculation ON\r\n");
                        }
                        else 
                        {
                            USMART_PRINTF("Run Time Calculation OFF\r\n");
                        }
                    }
                }
                else 
                {
                    return USMART_PARMERR;  /* No parameters, or wrong parameters */
                }
            }
            else 
            {
                return USMART_PARMERR;      /* Parameter error. */
            }
            
            USMART_PRINTF("\r\n");
            break;

        default:/* Disable instruction */
            return USMART_FUNCERR;
    }

    return 0;
}

/**
 * @brief    Initializes USMART
 * @param    tclk : The operating frequency (in Mhz) of the timer.
 * @retval   None
 */
void usmart_init(uint16_t tclk)
{
#if USMART_ENTIMX_SCAN == 1
	MX_TIM2_Init();			/* Initialization of the timer */
#endif
    usmart_dev.sptype = 1;  /* Hexadecimal display parameters */
}

/**
 * @brief   gets the function name,id, and arguments from str
 * @param   str : String pointer.
 * @retval  0, successful recognition; Other, error code.
 */
uint8_t usmart_cmd_rec(char *str)
{
    uint8_t sta, i, rval;   /* State */
    uint8_t rpnum, spnum;
    char rfname[MAX_FNAME_LEN];  /* Staging space for the received function name */
    char sfname[MAX_FNAME_LEN];  /* Store local function names */
    sta = usmart_get_fname(str, rfname, &rpnum, &rval); /* The name of the function that received the data and the number of arguments */

    if (sta)return sta;    /* Error */

    for (i = 0; i < usmart_dev.fnum; i++)
    {
        sta = usmart_get_fname((char *)usmart_dev.funs[i].name, sfname, &spnum, &rval); /* Get the name of the local function and the number of arguments */

        if (sta)return sta; /* Error in local parsing */

        if (usmart_strcmp(sfname, rfname) == 0) /* Equality */
        {
            if (spnum > rpnum)return USMART_PARMERR;/* Parameter errors (fewer input parameters than source function parameters) */

            usmart_dev.id = i;  /* Logging function ids */
            break;              /* Run out */
        }
    }

    if (i == usmart_dev.fnum)return USMART_NOFUNCFIND;  /* No matching function found */

    sta = usmart_get_fparam(str, &i);   /* Get the number of function arguments */

    if (sta)return sta;     /* Return error */

    usmart_dev.pnum = i;    /* Record the number of parameters */
    return USMART_OK;
}

/**
 * @brief USMART executes the function
 * @note
 *        This function is used to finally execute the valid function received from the serial port.
 * Functions with up to 10 arguments, more arguments are easy to implement. However, it is seldom used.
 * Functions with about 5 parameters are very rare.The function prints the execution on the serial port.
 * Start with :" Function name (parameter 1, parameter 2... Parameter N)= return value ".
 * When the function executed does not return a value, the printed return value is meaningless data.
 *
 * @param  None
 * @retval None
 */
void usmart_exe(void)
{
    uint8_t id, i;
    uint32_t res;
    uint32_t temp[MAX_PARM];        /* Argument converted to support strings */
    char sfname[MAX_FNAME_LEN];     /* Store local function names */
    uint8_t pnum, rval;
    id = usmart_dev.id;

    if (id >= usmart_dev.fnum)return;   /* No execution. */

    usmart_get_fname((char *)usmart_dev.funs[id].name, sfname, &pnum, &rval);    /* Get the name of the local function and the number of arguments */
    USMART_PRINTF("\r\n%s(", sfname);   /* Prints the name of the function that is being executed */

    for (i = 0; i < pnum; i++)          /* Output parameter */
    {
        if (usmart_dev.parmtype & (1 << i)) /* The argument is a string */
        {
            USMART_PRINTF("%c", '"');
            USMART_PRINTF("%s", usmart_dev.parm + usmart_get_parmpos(i));
            USMART_PRINTF("%c", '"');
            temp[i] = (uint32_t) & (usmart_dev.parm[usmart_get_parmpos(i)]);
        }
        else    /* The arguments are numbers */
        {
            temp[i] = *(uint32_t *)(usmart_dev.parm + usmart_get_parmpos(i));

            if (usmart_dev.sptype == SP_TYPE_DEC)
            {
                USMART_PRINTF("%ld", (long)temp[i]); /* Decimal argument display */
            }
            else 
            {
                USMART_PRINTF("0X%X", (unsigned int)temp[i]);      /* Hexadecimal parameter display */
            }
        }

        if (i != pnum - 1)USMART_PRINTF(",");
    }

    USMART_PRINTF(")");
#if USMART_ENTIMX_SCAN==1
    usmart_timx_reset_time();   /* The timer is reset and the clock starts */
#endif

    switch (usmart_dev.pnum)
    {
        case 0: /* No arguments (void type) */
            res = (*(uint32_t(*)())usmart_dev.funs[id].func)();
            break;

        case 1: /* There is one parameter */
            res = (*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0]);
            break;

        case 2: /* There is two parameter */
            res = (*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0], temp[1]);
            break;

        case 3: /* There is three parameter */
            res = (*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0], temp[1], temp[2]);
            break;

        case 4: /* There is four parameter */
            res = (*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0], temp[1], temp[2], temp[3]);
            break;

        case 5: /* There is five parameter */
            res = (*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0], temp[1], temp[2], temp[3], temp[4]);
            break;

        case 6: /* There is six parameter */
            res = (*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0], temp[1], temp[2], temp[3], temp[4], \
                    temp[5]);
            break;

        case 7: /* There is seven parameter */
            res = (*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0], temp[1], temp[2], temp[3], temp[4], \
                    temp[5], temp[6]);
            break;

        case 8: /* There is eight parameter */
            res = (*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0], temp[1], temp[2], temp[3], temp[4], \
                    temp[5], temp[6], temp[7]);
            break;

        case 9: /* There is nine parameter */
            res = (*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0], temp[1], temp[2], temp[3], temp[4], \
                    temp[5], temp[6], temp[7], temp[8]);
            break;

        case 10:/* There is ten parameter */
            res = (*(uint32_t(*)())usmart_dev.funs[id].func)(temp[0], temp[1], temp[2], temp[3], temp[4], \
                    temp[5], temp[6], temp[7], temp[8], temp[9]);
            break;
    }

#if USMART_ENTIMX_SCAN==1
    usmart_timx_get_time(); /* Get the function execution time */
#endif

    if (rval == 1)          /* Need to return a value */
    {
        if (usmart_dev.sptype == SP_TYPE_DEC)USMART_PRINTF("=%lu;\r\n", (unsigned long)res);   /* Output execution result (decimal argument display) */
        else USMART_PRINTF("=0X%X;\r\n", (unsigned int)res);  /* Output execution result (hexadecimal parameter display) */
    }
    else USMART_PRINTF(";\r\n");    /* No return value is needed, and the output ends directly */

    if (usmart_dev.runtimeflag)     /* Need to display function execution time */
    {
        USMART_PRINTF("Function Run Time:%d.%1dms\r\n", ((int)(usmart_dev.runtime / 10)), (int)(usmart_dev.runtime % 10));  /* Print the execution time of the function */
    }
}

/**
 * @brief USMART scan function
 * @note
 *        Implement the various controls of USMART by calling this function.
 *        The function needs to be called at regular intervals. Execute functions sent from the serial port in a timely manner.
 *        This function can be called from within an interrupt for automatic management.
 *        USART_RX_STA and USART_RX_BUF[] need to be implemented by users if they are not positive atomic users
 *
 * @param  None
 * @retval None
 */
void usmart_scan(void)
{
    uint8_t sta, len;
    char *pbuf = 0;

    pbuf = usmart_get_input_string();   /* Get the data stream */
    if (pbuf == 0) return ;             /* Data stream is empty, return directly */
     
    sta = usmart_dev.cmd_rec(pbuf);     /* Get information about the function */

    if (sta == 0)
    {
        usmart_dev.exe();               /* Executing the function */
    }
    else
    {
        len = usmart_sys_cmd_exe(pbuf);

        if (len != USMART_FUNCERR)sta = len;

        if (sta)
        {
            switch (sta)
            {
                case USMART_FUNCERR:
                    USMART_PRINTF("Function error!\r\n");
                    break;

                case USMART_PARMERR:
                    USMART_PRINTF("Parameter error\r\n");
                    break;

                case USMART_PARMOVER:
                    USMART_PRINTF("Parameter number too much\r\n");
                    break;

                case USMART_NOFUNCFIND:
                    USMART_PRINTF("No matching function found!\r\n");
                    break;
            }
        }
    } 
 
}

#if USMART_USE_WRFUNS == 1  /* If read and write operations are enabled */

/**
 * @brief  Reads the value at the specified address
 * @param  addr : the value at the specified address
 * @retval address
 */ 
uint32_t read_addr(uint32_t addr)
{
    return *(uint32_t *)addr;
}

/**
 * @brief  Reads the value at the specified address
 * @param  addr : the value at the specified address
 * @param  val  : the value
 * @retval address
 */
void write_addr(uint32_t addr, uint32_t val)
{
    *(uint32_t *)addr = val;
}

#endif





















