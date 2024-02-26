//parser
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parser.h"

STATE_MACHINE_RETURN_VALUE at_command_parse(uint8_t current_character)
{
    static uint8_t stare = 0;
    static uint8_t col_count = 0;
    static uint8_t line_count = 0;


    switch (stare)
    {
    case 0:
    {

        if (current_character == '\r')
        {
            stare = 1;
        }
        break;
    }

    case 1:
    {

        if (current_character == '\n')
        {
            stare = 2;
        }
        else
        {
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        break;
    }

    case 2:
    {

        if (current_character == 'O')
        {
            stare = 3;

        }
        else if (current_character == 'E')
        {
            stare = 4;

        }
        else if (current_character == '+')
        {

            stare = 5;

        }
        else  if (current_character >= 32 && current_character <= 126){
					if (var.line_count < AT_COMMAND_MAX_LINES &&
                var.col_count < AT_COMMAND_MAX_LINE_SIZE)
                {
                    var.data[var.line_count][var.col_count]=current_character;
                    var.col_count++;
            }
            stare = 15;
        } else
        {

            return STATE_MACHINE_READY_WITH_ERROR;

        }



    }
    break;

    case 3:
    {

        if (current_character == 'K')
        {
            stare = 6;
        }
        else
        {
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        break;
    }

    case 4:
    {

        if (current_character == 'R')
        {
            stare = 9;
        }
        else
        {
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        break;
    }

    case 5:
    {

        if (current_character >= 32 && current_character <= 126)
        {
            if (var.line_count < AT_COMMAND_MAX_LINES &&
                var.col_count < AT_COMMAND_MAX_LINE_SIZE)
                {
                    var.data[var.line_count][var.col_count]=current_character;
                    var.col_count++;
            }
            stare = 15;
        }
        else
        {
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        break;
    }

    case 6:
    {

        if (current_character == '\r')
        {
            stare = 7;
        }
        else
        {
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        break;
    }

    case 7:
    {

        if (current_character == '\n')
        {
            stare = 0;
            printf("Comanda AT corecta finalizat OK!\n");
            return STATE_MACHINE_READY_OK;

        }
        else
        {
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        break;
    }

    case 8:
    {
        stare = 0;
       printf("case 8 \n");
        return STATE_MACHINE_READY_OK;
        break;
    }

    case 9:
    {

        if (current_character == 'R')
        {
            stare = 10;
        }
        else
        {
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        break;
    }

    case 10:
    {

        if (current_character == 'O')
        {
            stare = 11;
        }
        else
        {
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        break;
    }

    case 11:
    {

        if (current_character == 'R')
        {
            stare = 12;
        }
        else
        {
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        break;
    }

    case 12:
    {

        if (current_character == '\r')
        {
            stare = 13;
        }
        else
        {
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        break;
    }

    case 13:
    {

        if (current_character == '\n')
        {
            stare = 0;
            printf("Comanda AT corecta finalizat ERROR!\n");
            return STATE_MACHINE_READY_OK;
        }
        else
        {
            return STATE_MACHINE_READY_WITH_ERROR;
        }
        break;
    }

    case 14:
    {

        return STATE_MACHINE_READY_OK;
        break;
    }

    case 15:
    {

        if (current_character >= 32 && current_character <= 126)
        {

           if (var.line_count < AT_COMMAND_MAX_LINES && var.col_count < AT_COMMAND_MAX_LINE_SIZE)
           {
                var.data[var.line_count][var.col_count] = current_character;
                var.col_count++;
                stare = 15;
            }

        }
            else if (current_character == '\r')
            {
                stare = 16;
            }
            else
            {
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;
        }

        case 16:
        {

            if (current_character == '\n')
            {
							var.line_count++;
                stare = 17;
            }
            else
            {
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;
        }

        case 17:
        {

            if (current_character == '+' || (current_character >= 32 && current_character <= 126))
            {
                if (var.line_count < AT_COMMAND_MAX_LINES && var.col_count < AT_COMMAND_MAX_LINE_SIZE)
                    {
                       // var.line_count++;
                        var.col_count=0;
                           stare = 5;
                    }

               // stare = 5;
            }
            else if (current_character == '\r')
            {
                stare = 18;
            }
            else
            {
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;
        }

        case 18:
        {

            if (current_character == '\n')
            {
                stare = 19;
            }
            else
            {
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;
        }

        case 19:
        {

            if (current_character == 'E')
            {
                stare = 4;
            }
            else if (current_character == 'O')
            {
                stare = 3;
            }
            else
            {
                return STATE_MACHINE_READY_WITH_ERROR;
            }
            break;
        }
        default:
            break;
        }

        return STATE_MACHINE_NOT_READY;
    }
