#include <unistd.h>
#include <stdio.h>
void    print_word(char *str)
{
    int i;

    i = 0;
    while (str[i] && str[i] != ' ')
        write(1, &str[i++], 1);
}

int is_space(char c)
{
    return (c == ' ' || (c >= 9 && c <= 13));
}

int main(int ac, char *av[])
{
    write(1, "hello world!\n", 13);
    printf("hello world!\n");
}
