# Application Core Code

Source folder for headears & code files directly involved with the core problem. 

## Simplications 

Some stepts will be taken in order to circle problem to a manageable number of possibilities

## Realistic limits 

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

As well, we could consider that our application should only tackle around different 6000 first names or 60000 last names in our given country. Even we can take for granted that there aren't more than 60000 locations, that we might classify them in two levels; one coarse level easy to remember and another fine one more close to small places.

Translate into C++:

- First Name: unsigned short int (uint16_t)
- Last Name: unsigned short int (uint16_t)
- Year of Birth: unsigned char (unit8_t) < 200 years
- Coarse Location of Birth: unsigned short int (unit8_t)
- Month of Birth: unsigned char (uint8_t)
- Day of Birth: unsigned char (unit8_t)
- Fine location of Birth: unsigned short int (uint8_t)
- More information related to a specific subject: extra indexes.

This way we can use the **first 64 bits of information** as a valid **identification** for the individuals and with the advantage of getting the relevant information to debug first: *name and generation*.


