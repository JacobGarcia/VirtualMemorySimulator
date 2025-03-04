/*************************************************************************/
/*                                                                       */
/* Copyright (c) 2014 Mario J. García Navarro                            */
/*                                                                       */
/* File name: FileIO.c                                                   */
/*                                                                       */
/* Author:                                                               */
/*          Mario García Navarro                                         */
/*                                                                       */
/* Purpose:                                                              */
/*          Support routines that handle reading ASCII files             */
/*                                                                       */
/* References:                                                           */
/*          Based on Abelardo López Lagunas code                         */
/*                                                                       */
/* Revision history:                                                     */
/*          Nov  8 19:07 2014 - File created                             */
/*                                                                       */
/* Error handling:                                                       */
/*          None                                                         */
/*                                                                       */
/* Notes:                                                                */
/*          Support routine that reads an ASCII file and returns an      */
/*          integer value skiping over non-numeric data                  */
/*                                                                       */
/*************************************************************************/

/************************************************************************/
/*                                                                      */
/*  Function: ErrorMsg                                                  */
/*                                                                      */
/*  Purpose: Prints an error message and then gracefully terminate the  */
/*           program. This is the release version of assert.            */
/*                                                                      */
/*  Parameters:                                                         */
/*            Input :  String with the error message                    */
/*                                                                      */
/*            Output:  Prints the error in standard output and exit     */
/*                                                                      */
/************************************************************************/


#include <stdlib.h>              /* Used for getc() and feof() functions */
#include <stdio.h>
#include <ctype.h>                    /* Used for the isdigit() function */
#include "FileIO.h"


struct reference GetAddress (FILE *fp){
    
    char	             c;	   	                    /* Character read */
    struct reference data;
    
    do {
        c = getc(fp);	                        /* Get next character */
        if(c == '#')	                          /* Skip the comment */
            do {
                c = getc(fp);
            } while(c != '\n');
    } while(!isxdigit(c) && !feof(fp));
    
    ungetc(c, fp);                /* Return the character to the file */
    fscanf(fp, "%x %c\n", &data.address, &c);
    
    data.address = data.address >> 9;
    
    if(c == 'R')
        data.access = READ;
    else
        data.access = WRITE;
        
    return (data);
}

int GetInt (FILE *fp){
    int	c, i;         /* Character read and integer representation of it */
    int sign = 1;
    
    /* Skip the comment */
    do {
        c = getc (fp);                          /* Get next character */
        if ( c == '#' )
            do {
                c = getc (fp);
            } while ( c != '\n');
        /* This is for take account the negative sign, in case the number has */
        if ( c == '-')
            sign = -1;
    } while (!isdigit(c) && !feof(fp));
    
    if (feof(fp))
        return (EXIT_FAILURE);
    else {
        
        /* Found 1st digit, begin conversion until a non-digit is found */
        i = 0;
        while (isdigit (c) && !feof(fp)){
            i = (i * 10) + (c - '0');         /* ASCII conversion */
            c = getc (fp);
        }
        
        return (i * sign);
    }
}

void ErrorMsg (char * function, char * message){
    
    printf ("Error in function: %s\n", function);
    printf ("\t %s\n", message);
    printf ("The program will terminate\n\n");
    
}