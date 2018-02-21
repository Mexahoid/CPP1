// CPP1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <stdarg.h> 
#include <string>
// 20. sprintf   s b d f
// Store Print
int sprintf_my(char *buffer, const char *format, ...);

int main()
{
	char string[100];
	sprintf_my(string, "Maintest string: %s, integer: %d, boolean: %b, real: %f", "TestSrt", 265, false, 3.141592f);
	return 0;
}

int sprintf_my(char *buffer, const char *format, ...)
{
	const char *p = format;
	char *b = buffer;
	const char *str;

	va_list vl;
	va_start(vl, format);

	while (*p)
	{
		if (*p == '%' && *(p + 1) != '\0')
		{
			switch (*++p)
			{
			case '+':
				break;
			case '-':
				break;
			case ' ':
				break;
			case '#':
				break;
			case '0':
				break;
			default:			// Если не найдено флагов, то смотрим на ширину
				switch (*p)		// Смотрим на первый символ
				{
				case '*':
					break;
				default:		// Сюда еще случаи чисел
					if(static_cast<int>(*p) - 48 > 0 && static_cast<int>(*p) - 48 < 10)
					{
						char buf[20];
						char *bf = buf;
						while(static_cast<int>(*(p + 1)) - 48 > 0 && static_cast<int>(*(p + 1)) - 48 < 10)
						{
							*bf++ = *p++;
						}
						*bf = '\0';
						int width = atoi(buf);

					}
					break;
				}
				break;
			}
			switch (*++p)
			{
			case 's':
			{
				str = va_arg(vl, char *);
				while (*str)
				{
					*b++ = *str++;
				}
			}
			break;
			case 'd':
			{
				std::string strng = std::to_string(va_arg(vl, int)).c_str();
				int i = 0;
				while (strng[i])
				{
					*b++ = strng[i++];
				}
			}
			break;
			case 'f':
			{
				std::string strng = std::to_string(va_arg(vl, double)).c_str();
				int i = 0;
				while (strng[i])
				{
					*b++ = strng[i++];
				}
			}
			break;
			case 'b':
			{
				str = va_arg(vl, bool) ? "true" : "false";
				while (*str)
				{
					*b++ = *str++;
				}
			}
			break;
			default:
				break;
			}
		}
		else
		{
			*b++ = *p;
		}
		p++;
	}
	va_end(vl);
	*b = '\0';
	return 0;
}