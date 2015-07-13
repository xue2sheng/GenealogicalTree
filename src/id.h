#ifndef ID_H
#define	ID_H

/// \file id.h
/// Define types for id's for our subjects
///
/// A first approach of getting packed id & basic information in form of indexes:
///
/// - First Name: unsigned short int (uint16_t)
/// - Last Name: unsigned short int (uint16_t)
/// - Year of Birth: unsigned char (unit8_t) < 200 years
/// - Coarse Location of Birth: unsigned short int (unit8_t)
/// - Month of Birth: unsigned char (uint8_t)
/// - Day of Birth: unsigned char (unit8_t)
///
/// Grouping all that indexes we got a 64 bits identification

#include <cstdint>
#include <iostream>

/// \brief Reuse hash keys as unique id
typedef struct {
   char year;  ///<year of birth
   //uint16_t first_name; 
   //uint16_t last_name;  
   //uint8_t location;  
   //uint8_t month;  
   //uint8_t day;  
} struct_index_t; // __attribute__((aligned(64)));

//static constexpr const struct_index_t EMPTY_STRUCT_INDEX{0,0,0,0,0,0};

/// \brief Let the compiler/memory translate between hash keys and id
typedef union {
   uint64_t id; ///<id Like a long integer for global hush table
   struct_index_t index; ///<index Like a structure for different hush tables
} union_id_t;

/// \brief To zero union_id_t 
static constexpr const union_id_t EMPTY_UNION_ID{0};

/// \brief Make it easier to log union_id_t
inline std::ostream & operator<<(std::ostream & os, const union_id_t & u)
{
    os << "[ " << std::hex << u.id << " | ";
    //os << std::hex << u.index.first_name << " ";
    //os << std::hex << u.index.last_name << " ";
    os << u.index.year << " ";
    //os << u.index.location << " ";
    //os << u.index.month << " ";
    //os << u.index.day << " ]";

    return os;
}

#endif	/* ID_H */

