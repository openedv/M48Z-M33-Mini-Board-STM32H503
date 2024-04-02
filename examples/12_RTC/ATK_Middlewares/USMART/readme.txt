USMART V3.4
  USMART is developed by ALIENTEK a clever serial debugging interoperability component, through it, 
you can call any function in the program through the serial assistant, and execute. 
Therefore, you can change the input parameters of the function (supports numbers (10/16, negative numbers), 
strings, function entry addresses, etc.), a single function supports up to 10 input parameters, and supports 
the function return value display.
Two new instructions, hex and dec, have been added since V2.1.
They can be used to format the display of function arguments. 
It can also be used to convert data to base, for example:
  Type "hex 100" to see HEX 0X64 on the serial debugger.
  Typing "dec 0X64" will see DEC 100 on the serial debugging assistant.

Upgrade Notes
V1.4
1,Added support for functions with string arguments. The scope of application is greatly improved.
2,The memory footprint is optimized with a static memory footprint of 79 bytes @10 parameters. Dynamically ADAPTS to number and string lengths

V2.0
1, modify the list instruction to print the full expression of the function.
2, adds the id directive, which prints the entry address of each function.
3, parameter matching has been modified to support function parameter calls (input entry addresses).
4, added the function name length macro definition.	

V2.1 20110707
1, add dec,hex two instructions, used to set parameters to display base system, and perform base conversion.
Note: When dec,hex does not take parameters, that is, set the display parameter base. When followed by an argument, a base conversion is performed.
For example, "dec 0XFF" converts 0XFF to 255, which is returned by the serial port.
For example, "hex 100" converts 100 to 0X64 and is returned by the serial port
The usmart_get_cmdname function is added to get the instruction name.

V2.2 20110726
1, fixed an error in parameter statistics for parameters of type void.
2, modify the data display format to default to hexadecimal.

V2.3 20110815
Remove the restriction that function names must be followed by a "(".
2, fixed a bug where string arguments could not have "(" in them.
3, changed how the default format of the function's arguments is changed.

V2.4 20110905
The usmart_get_cmdname function is modified to add a maximum argument length limit. It avoids the crash phenomenon when the wrong parameters are entered.
Add the USMART_ENTIM2_SCAN macro definition to configure whether to use TIM2 to execute the scan function.

V2.5 20110930
1, modify the usmart_init function to void usmart_init(u8 sysclk), which can automatically set the scanning time according to the system frequency (fixed 100ms).
2, remove the uart_init function in the usmart_init function, and the serial port initialization must be initialized outside, which is convenient for users to manage themselves.

V2.6 20111009
The read_addr and write_addr functions are added. You can use these two functions to read and write any internal address (must be a valid address). More convenient debugging.
The read_addr and write_addr functions can be enabled and disabled by setting USMART_USE_WRFUNS to.
3, usmart_strcmp is modified to normalize it.

V2.7 20111024
1, fixed a bug where the return value was displayed in hexadecimal without a line break.
2, added a test whether the function returns a value, if not, will not be displayed. Display the return value only if it is present.

V2.8 20111116
1, fixed a bug where instructions without arguments such as list may cause a crash after being sent.

V2.9 20120917
1, fixed the form: void*xxx(void) type function can not recognize the bug.

V3.0 20130425
1, adding support for escape characters in string arguments.

V3.1 20131120
1, add runtime system instruction, which can be used to calculate function execution time.
Usage:
Sending :runtime 1 enables the function execution time statistics
Send :runtime 0, then turn off the function execution time statistics
runtime statistics :USMART_ENTIMX_SCAN must be set to 1 in order to use!!

V3.2 20140828
Modify the usmart_get_aparm function to include support for the +/- symbol
Modify the usmart_str2num function to support negative conversions

V3.3 20160506
Fixed a bug where the usmart_exe function gave an error when USMART_ENTIMX_SCAN was 0

V3.4 20200324
Change the variable name to: uint8_t, uint16_t, uint32_t, no longer use u8,u16,u32, but still support
u8,u16,u32 named function call.

							@ALIENTEK
							technical forum : www.openedv.com
							date            : Aug 28, 2014
							Copyright(C) ALIENTEK 2011-2021



















