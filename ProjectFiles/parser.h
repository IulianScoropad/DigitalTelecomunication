//header
#ifndef AT_PARSER_HEADER
#define AT_PARSER_HEADER

#include <stdint.h>
#define AT_COMMAND_MAX_LINES 100
#define AT_COMMAND_MAX_LINE_SIZE 128
typedef struct
{
 uint8_t ok;
 uint8_t data[AT_COMMAND_MAX_LINES][AT_COMMAND_MAX_LINE_SIZE + 1];
 uint32_t line_count;
 uint32_t col_count;
}AT_COMMAND_DATA;

typedef enum
{
 STATE_MACHINE_NOT_READY,
 STATE_MACHINE_READY_OK,
 STATE_MACHINE_READY_WITH_ERROR
}STATE_MACHINE_RETURN_VALUE;

extern AT_COMMAND_DATA var;
STATE_MACHINE_RETURN_VALUE at_command_parse(uint8_t current_character);

#endif // AT_PARSER_HEADER
