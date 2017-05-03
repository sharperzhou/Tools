#include <memory.h>
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define REG_PATH                TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run")
#define AUTOSTART263EM          TEXT("AutoStart263em")

char str[] = "  \"aaaa\"  \"bb bb\" \" cccccc \" ";

void SplitString(char *src, char dest[][30], int n);

int main()
{
    char dest[3][30] = {0};
    SplitString(str, dest, 3);
    for (int i = 0; i < 3; i++)
        printf("%s\n", dest + i);


    char *szFileName = "zxcbdsag";

    HKEY hKey;

    if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, REG_PATH, 0, KEY_SET_VALUE, &hKey))
        if (ERROR_SUCCESS == RegSetValueEx(hKey, AUTOSTART263EM, 0, REG_SZ,
            (BYTE*) szFileName, strlen(szFileName) + 1))
            ;

    RegCloseKey(hKey);

    return 0;
}

void SplitString(char *src, char dest[][30], int n)
{
    char *delim = "\"";
    char *p, *q;

    p = src;
    for (int i = 0; i < n; i++)
    {
        while (*p == ' ') ++p;
        q = p + 1;
        while (*q != '\"') ++q;
        memcpy(dest + i, p + 1, q - p - 1);
        dest[i][q - p - 1] = '\0';
        p = q + 1;
    }

}