Application Core Code
=====================

Source folder for headears & code files directly involved with the core problem. 

## Limits

First of all, we should grasp a rough idea about which range of numbers to consider:

- Most Populous Country: China 
  
  Inhabitants: [around](http://www.worldometers.info/world-population/china-population) 1400000000

- Another populous country, culturally diverse: USA

  Number of first & last names: [around](http://howmanyofme.com) 5200 & 152000

- Example of baptism registers: Ireland

  Roman Catholic: [around](http://www.irish-genealogy-toolkit.com/Roman-Catholic-baptism.html) 19th century

- Marriageable age: world 

  Some common value: [around](https://en.wikipedia.org/wiki/Marriageable_age) 20  

- Number of locations: India

  Number of villages: [around](http://censusindia.gov.in/Census_Data_2001/Census_data_finder/A_Series/Number_of_Village.htmi) 640000

## Assumptions on numbers

This way we can assume that taking into account around 200 years of sensible information on our ascendants, around 10 generations back in time, we suppose not to deal with more than 4000000000 individuals.  

As well, we could consider that our application should only tackle around different 6000 first names or 60000 last names in our given country. Even we can take for granted that there aren't more than 60000 locations.

Translate into C++:

- First Name: unsigned short int (16)
- Last Name: unsigned short int (16)
- Location of Birth: unsigned short int (16)
- Year of Birth: unsigned char (8) < 200 years
- Month of Birth: unsigned char (8)
- Day of Birth: unsigned char (8)

Due to the fact that registers & memory work better with 16, 32 and 64 bits, let's identify our individuals with the first 64 bits of the previous fields. In other words, don't take into account the day of birth. 


## Generated Files

**version.h** is generated with *GIT* information by *cmake*
