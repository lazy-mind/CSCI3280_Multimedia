Read Me :

CSCI3280 Assignment1
SID : 1155062018 Name : Mao Yuxuan


For part1:
To compile : el.exe inverse.cpp bmp.cpp
To execute : inverse <art.txt> <output.bmp>
In inverse.cpp, I use a division of 17 to cover the range from 0 to 255. (0 17 34 …… 255)


For part2:
To compile : el.exe ascii.cpp bmp.cpp
To execute : ascii <art.bmp> <output.txt>
In ascii.cpp, I use a division of 17 to build the ascii art file.
Because the computation involved floating point number calculation, I use a conditional clause to judge which character is more suitable to use. 


For bonus part:
To compile : el.exe bonus.cpp bmp.cpp
To execute : bonus <art.bmp> <output.html>
In bonus.cpp, 
I use “Courier New” to ensure every characters take up same space.
I specify the gap between 2 lines to be 0.1em , so the size of ascii art is better.
I use rgb color setting to set color for each characters.
 
