#include "usmart.h"
#include "usmart_str.h"

/******************************************************************************************/
/* User configuration area
 * The next thing to do is to include the header file declared by the function
 * you are using (you add it yourself).
 */
#include "../../BSP/STMFLASH/stmflash.h"

/* Initialized list of function names (added by user)
 * The user directly enters the name of the function to execute and its lookup string here
 */
struct _m_usmart_nametab usmart_nametab[] =
{
#if USMART_USE_WRFUNS == 1      /* If read and write operations are enabled */
    {(void *)read_addr, "uint32_t read_addr(uint32_t addr)"},
    {(void *)write_addr, "void write_addr(uint32_t addr,uint32_t val)"},
#endif
    {(void *)stmflash_read_word, "uint32_t stmflash_read_word(uint32_t faddr)"},
    {(void *)stmflash_test_write, "void stmflash_test_write(uint32_t waddr, uint32_t wdata)"},
};

/******************************************************************************************/

/* Functions control manager initialization
 * Get the name of each controlled function
 * Get the total number of functions
 */
struct _m_usmart_dev usmart_dev =
{
    usmart_nametab,
    usmart_init,
    usmart_cmd_rec,
    usmart_exe,
    usmart_scan,
    sizeof(usmart_nametab) / sizeof(struct _m_usmart_nametab), /* Number of functions */
    0,      /* arity */
    0,      /* function ID */
    1,      /* Parameter display type,0:decimalism; 1:hexadecimal */
    0,      /* parameter type, 0:number; 1:string */
    0,      /* The length of each parameter is temporarily
               stored in the table, requiring MAX PARM 0 initializations */
    0,      /* The arguments of the function, require PARM LEN 0 initializations. */
};



















