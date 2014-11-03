#include <stdlib.h>
#include <stdio.h>

int ctoi(char num[])
{
  int i = 0;
  int is_negative = 1;
  int res;

  if (num[0] == '-')
  {
    is_negative = -1;
    i++;
  }
  if (num[i+1] == '\0')
  {
    res = (num[i] - '0');
  }
  else
  {
    res = (num[i] - '0')*10 + (num[i+1] - '0');
  }

  res *= is_negative;

  return res;
}

void itoc(int num, char *buf)
{
  int i = 0;

  if (num < 0)
  {
    buf[i++] = '-';
    num *= -1;
  }

  if (num / 10 == 0)
  {
    buf[i++] = num + '0';
  }
  else
  {
    buf[i++] = num / 10 + '0';
    buf[i++] = num % 10 + '0';
  }

  buf[i] = '\0';
}

int main()
{
  char control[20] = "-11+9=*95876=";
  char selected_control;
  int current_char = 0;
  int current_digit = 0;
  int current_num = 1;
  char num1[4];
  char num2[4];
  int inum1 = 0;
  int inum2 = 0;
  int result = 0;
  int have_result = 0;
  char operation;
  int i;
  char test_itoc[4];

  for(i=0; i<13; i++)
  {
    selected_control = control[i];
    //printf("selected: %c\n", selected_control);

    if (selected_control > 41 && selected_control < 48) // + - * /
    {
      if (selected_control == '-' && current_digit == 0)
      {
        switch(current_num)
        {
          case 1:
            num1[current_char] = selected_control;
            break;
          case 2:
            num2[current_char] = selected_control;
            break;
        }
        current_char++;
      }
      else
      {
        switch(current_num)
        {
          case 1:
            num1[current_char] = '\0';
            break;
          case 2:
            num2[current_char] = '\0';
            break;
        }

        current_num++;
        current_char = 0;
        current_digit = 0;
        operation = selected_control;
        continue;
      }
    }
    else if (selected_control == '=')
    {
      // some eval code
      if (have_result == 0)
      {
        inum1 = ctoi(num1);
        inum2 = ctoi(num2);
      }
      else
      {
        inum1 = result;
        inum2 = ctoi(num2);
      }

      //printf("num1 = %d\nnum2 = %d\n", inum1, inum2);

      switch (operation)
      {
        case '+':
          result = inum1 + inum2;
          break;
        case '-':
          result = inum1 - inum2;
          break;
        case '*':
          result = inum1 * inum2; // do with hardware multiplier
          break;
        case '/':
          result = inum1 / inum2;
          break;
      }

      printf("eval %d %c %d = %d\n", inum1, operation, inum2, result);

      //printf("result = %d\n", result);
      //itoc(result, test_itoc);
      //printf("converted to char = %s\n", test_itoc);

      current_num = 1;
      if (have_result == 0)
      {
        have_result = 1;
      }
    }
    else if (selected_control == 'C')
    {
      // reset
      printf("RESET\n");
      current_char = 0;
      current_digit = 0;
      current_num = 1;
      inum1 = 0;
      inum2 = 0;
      result = 0;
      continue;
    }
    else
    {
      if (current_digit > 1)
      {
        current_digit++;
        current_char--;

        switch(current_num)
        {
          case 1:
            num1[current_char-1] = num1[current_char];
            break;
          case 2:
            num2[current_char-1] = num2[current_char];
            break;
        }
      }

      switch(current_num)
      {
        case 1:
          num1[current_char] = selected_control;
          break;
        case 2:
          num2[current_char] = selected_control;
          break;
      }

      current_digit++;
      current_char++;
    }
  }

  return 0;
}
