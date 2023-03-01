#include <iostream>
#include <string>
#include <cctype>
#include "rule.hpp"
// validateNickname is parametor A-Z a-z 0-9 @ | _ - and must be between 3 and 9 characters
bool validateNickname(const char *nickname){
  int i = 0;
  while (nickname[i] != '\0')
  {
    if (!isalnum(nickname[i]) && nickname[i] != '@' && nickname[i] != '|' && nickname[i] != '_' && nickname[i] != '-')
      return false;
    i++;
  }
  if (i < 3 || i > 9)
    return false;
  return true;
}