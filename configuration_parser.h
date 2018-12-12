#ifndef CONFIGURATION_PARSER_H
#define CONFIGURATION_PARSER_H
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#ifdef __cplusplus
extern "C"{
#endif

bool conf_readInt(int *var, char *fn, char *name);
bool conf_readDouble(double *var, char *fn, char *name);
bool conf_readBool(bool *var, char *fn, char *name);
char *conf_readString(char *fn, char *name);

bool conf_writeString(char *fn, char *name, char *str);
bool conf_writeInt(char *fn, char *name, int var);
bool conf_writeDouble(char *fn, char *name, double var);
bool conf_writeBool(char *fn, char *name, bool var);

bool conf_search_for_line(FILE *f, char *name);
bool conf_variableExist(char *fn, char *name);
long int conf_file_size(FILE *f);
void conf_reach_beginning_line(FILE *f);
bool conf_reach_next_line(FILE *f);

#define CONF_MAX_DATA_LENGTH 2048

#ifdef __cplusplus
}
#endif 

#endif