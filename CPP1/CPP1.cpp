// CPP1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <stdarg.h> 
#include <string>
// 20. sprintf   s b d f
// Store Print
int sprintf_my(char *buffer, const char *format, ...);
bool isFlagValid(const char symbol);

int main()
{
	char string[100];
	// sprintf_my(string, "Maintest string: %s, integer: %d, boolean: %b, real: %f", "TestSrt", 265, false, 3.141592f);
	// sprintf_my(string, "Width trick: %+*d \n", 5, 10);
	int count = sprintf_my(string, "floats: %6.2f %+.0f %f \n", 3.1416, 3.1416, 3.1416);
	printf("String: %s, count: %d", string, count);
	int val;
	std::cin >> val;
	return 0;
}

int sprintf_my(char *buffer, const char *format, ...)
{
	const int FLAG_BUF = 128;

	char flagBuf[FLAG_BUF];
	char *pFlag = flagBuf;

	const char *p = format;
	char *b = buffer;
	const char *str;

	va_list vl;
	va_start(vl, format);

	int returnValue = -1;

	// Модификаторы
	bool isFlagPresent, isWidthPresent, isPrecisionPresent;

	char flag;
	int width;
	int precision;

	while (*p)
	{
		isFlagPresent = false;
		isWidthPresent = false;
		isPrecisionPresent = false;

		width = 0;
		precision = 0;


		if (*p == '%' && *(p + 1) != '\0')
		{
			short flags = 0;
			while (isFlagValid(*++p)) {
				switch (*p)
				{
				case '+':
					if (flags & 00010000)
						return -1;
					flags |= 00010000;
					*pFlag++ = *p;
					isFlagPresent = true;
					break;
				case '-':
					if (flags & 00001000)
						return -1;
					flags |= 00001000;
					*pFlag++ = *p;
					isFlagPresent = true;
					break;
				case ' ':
					if (flags & 00000100)
						return -1;
					flags |= 00000100;
					*pFlag++ = *p;
					isFlagPresent = true;
					break;
				case '#':
					if (flags & 00000010)
						return -1;
					flags |= 00000010;
					*pFlag++ = *p;
					isFlagPresent = true;
					break;
				case '0':
					if (flags & 00000001)
						return -1;
					flags |= 00000001;
					*pFlag++ = *p;
					isFlagPresent = true;
					break;
				default:
					break;
				}
			}
			*pFlag = '\0';

			switch (*p)		// Смотрим на первый символ
			{
			case '*':
				isWidthPresent = true;
				width = va_arg(vl, int);
				break;
			default:		// Сюда еще случаи чисел
				if (static_cast<int>(*p) - 48 > 0 && static_cast<int>(*p) - 48 < 10)
				{
					char buf[20];
					char *bf = buf;
					*bf++ = *p++;
					while (static_cast<int>(*p) - 48 > -1 && static_cast<int>(*p) - 48 < 10)
					{
						*bf++ = *p++;
					}
					*bf = '\0';
					width = atoi(buf);
				}
				isWidthPresent = width > 0;
				break;
			}

			if (*p == '.')
			{
				isPrecisionPresent = true;
				if (*++p == '*')
				{
					precision = va_arg(vl, int);
				}
				else
				{
					if (static_cast<int>(*p) - 48 > -1 && static_cast<int>(*p) - 48 < 10)
					{
						char buf[20];
						char *bf = buf;
						*bf++ = *p++;
						while (static_cast<int>(*p) - 48 > -1 && static_cast<int>(*p) - 48 < 10)
						{
							*bf++ = *p++;
						}
						*bf = '\0';
						precision = atoi(buf);
					}
				}
			}

			switch (*p)
			{
			case 's':
			{
				str = va_arg(vl, char *);
				while (*str)
				{
					*b++ = *str++;
					returnValue++;
				}
			}
			break;
			case 'b':
			{
				str = va_arg(vl, bool) ? "true" : "false";
				while (*str)
				{
					*b++ = *str++;
					returnValue++;
				}
			}
			break;

			case 'd':
			{
				int val = va_arg(vl, int);
				std::string strng = std::to_string(val).c_str();
				int i = 0;
				// Получить длину числа
				int numLength = 0;
				while (strng[i++]) {
					numLength++;
				}
				i = 0;

				char startingChar = 0;
				char fillingChar = ' ';

				bool startFromLeft = isWidthPresent;

				if (isFlagPresent)
				{
					pFlag = flagBuf;
					while (*pFlag) {
						switch (*pFlag)
						{
						case ' ':
						case '+':
							startingChar = val > 0 ? *pFlag : '-';
							numLength++;
							returnValue++;
							break;
						case '-':
							if (isWidthPresent)
								startFromLeft = true;
							else
								return -1;
							break;
						case '0':
							if (isWidthPresent)
								fillingChar = '0';
							else
								return -1;
							break;
						case '#':
						default:
							break;
						}
						pFlag++;
					}
				}


				// Сначала число слева
				if (startFromLeft)
				{
					for (i = 0; i < width - numLength; i++)
					{
						*b++ = fillingChar;
						returnValue++;
					}
					i = 0;
					if (startingChar != '\0')
					{
						*b++ = startingChar;
					}
					while (strng[i])
					{
						*b++ = strng[i++];
						returnValue++;
					}
				}
				else  // Иначе число справа
				{
					if (startingChar != '\0')
					{
						*b++ = startingChar;
					}
					while (strng[i])
					{
						*b++ = strng[i++];
						returnValue++;
					}
					for (i = 0; i < width - numLength; i++)
					{
						*b++ = fillingChar;
						returnValue++;
					}
				}
			}
			break;
			case 'f':
			{
				double val = va_arg(vl, double);
				std::string strng = std::to_string(val).c_str();
				int i = 0;

				int numLength = 0;
				bool prec = false;
				int count = 0;
				if (isPrecisionPresent && precision == 0)
				{
					while (strng[i] && strng[i++] != '.')
					{
						numLength++;
					}
				}
				else
					while (strng[i])
					{
						if (strng[i++] == '.')
						{
							if (isPrecisionPresent && !prec) {
								prec = true;
							}
						}
						if (prec && count++ > precision)
							break;
						numLength++;
					}
				i = 0;

				char startingChar = 0;
				char fillingChar = ' ';

				bool startFromLeft = isWidthPresent;


				if (isFlagPresent)
				{
					pFlag = flagBuf;
					while (*pFlag)
					{
						switch (*pFlag)
						{
						case ' ':
						case '+':
							startingChar = val > 0 ? *pFlag : '-';
							returnValue++;
							break;
						case '-':
							if (isWidthPresent)
								startFromLeft = true;
							else
								return -1;
							break;
						case '0':
							if (isWidthPresent)
								fillingChar = '0';
							else
								return -1;
							break;
						case '#':
						default:
							break;
						}
						pFlag++;
					}
				}


				bool precisionStarted;
				if (startFromLeft)
				{
					for (i = 0; i < width - numLength; i++)
					{
						*b++ = fillingChar;
						returnValue++;
					}
					i = 0;
					if (startingChar != '\0')
					{
						*b++ = startingChar;
					}
					while (strng[i] && i < numLength)
					{
						*b++ = strng[i++];
						returnValue++;
					}
				}
				else  // Иначе число справа
				{
					if (startingChar != '\0')
					{
						*b++ = startingChar;
					}
					while (strng[i] && i < numLength)
					{
						*b++ = strng[i++];
						returnValue++;
					}
					for (i = 0; i < width - numLength; i++)
					{
						*b++ = fillingChar;
						returnValue++;
					}
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
			returnValue++;
		}
		p++;
	}
	va_end(vl);
	*b = '\0';
	returnValue++;
	return returnValue;
}

bool isFlagValid(const char symbol)
{
	switch (symbol) {
	case '+':
	case '-':
	case '0':
	case ' ':
	case '#':
		return true;
	default:
		return false;
	}
}
