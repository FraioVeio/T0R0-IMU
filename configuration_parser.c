#include"configuration_parser.h"

#ifdef __cplusplus
extern "C"{
#endif

/*
    If variable name is present will be modified, else a new line will be added

    fn: file name
    name: variable name
    var: variable value

    return: true write ok
*/
bool conf_writeInt(char *fn, char *name, int var) {
    char str[CONF_MAX_DATA_LENGTH];
    snprintf(str, CONF_MAX_DATA_LENGTH, "%i", var);
    return conf_writeString(fn, name, str);
}

/*
    If variable name is present will be modified, else a new line will be added

    fn: file name
    name: variable name
    var: variable value

    return: true write ok
*/
bool conf_writeDouble(char *fn, char *name, double var) {
    char str[CONF_MAX_DATA_LENGTH];
    snprintf(str, CONF_MAX_DATA_LENGTH, "%.*lf", 16, var);
    return conf_writeString(fn, name, str);
}

/*
    If variable name is present will be modified, else a new line will be added

    fn: file name
    name: variable name
    var: variable value

    return: true write ok
*/
bool conf_writeBool(char *fn, char *name, bool var) {
    if(var) {
        return conf_writeString(fn, name, "true");
    } else {
        return conf_writeString(fn, name, "false");
    }
}

/*
    Check if variable exists

    fn: file name
    name: variable name

    return: true if exists
*/
bool conf_variableExist(char *fn, char *name) {
    FILE *f = fopen(fn, "r");
    if(f != NULL) {
        bool r = conf_search_for_line(f, name);

        fclose(f);

        return r;
    } else
        return false;
}


/*
    If variable name is present will be modified, else a new line will be added

    fn: file name
    name: variable name
    str: variable value

    return: true write ok
*/
bool conf_writeString(char *fn, char *name, char *str) {
    FILE *f = fopen(fn, "r");
    long sizef = conf_file_size(f);
    
    bool exist = conf_search_for_line(f, name);
    if(exist) {
        conf_reach_beginning_line(f);
        long cut = ftell(f);

        char tmpname[50];
        strcpy(tmpname, fn);
        strcat(tmpname, "tmp");

        FILE *ft = fopen(tmpname, "w+");

        rewind(f);
        while(ftell(f) < cut) {
            fputc(fgetc(f), ft);
        }   // copy till line to be modified

        fputs(name, ft);
        fputs("=", ft);
        fputs(str, ft);
        fputc('\n', ft);

        conf_reach_next_line(f);
        while(ftell(f) < sizef) {
            fputc(fgetc(f), ft);
        }

        fclose(ft);
        fclose(f);

        remove(fn);
        rename(tmpname, fn);
    } else {
        fclose(f);
        f = fopen(fn, "a+");
        fputs(name, f);
        fputs("=", f);
        fputs(str, f);
        fputc('\n', f);
        fclose(f);
    }

    char *scm = conf_readString(fn, name);
    bool res = strcmp(scm, str) == 0;
    free(scm);
    return res;
}

/*
    Search the int attached to keyword name.
    
    var: pointer to variable to store value
    fn: String containing file path
    name: String containing the keyword to search

    return: true if value found, false if not
*/
bool conf_readInt(int *var, char *fn, char *name) {
    char *data = conf_readString(fn, name);
    if(data != NULL) {
        *var = atoi(data);
        free(data);
        return true;
    } else {
        return false;
    }
}

/*
    Search the double attached to keyword name.
    
    var: pointer to variable to store value
    fn: String containing file path
    name: String containing the keyword to search

    return: true if value found, false if not
*/
bool conf_readDouble(double *var, char *fn, char *name) {
    char *data = conf_readString(fn, name);

    if(data != NULL) {
        *var = atof(data);
        free(data);
        return true;
    } else {
        return false;
    }
}

/*
    Search the bool attached to keyword name.
    valid bool formats are "true", "TRUE", "1", "false", "FALSE", "0"
    
    var: pointer to variable to store value
    fn: String containing file path
    name: String containing the keyword to search

    return: true if value found, false if not
*/
bool conf_readBool(bool *var, char *fn, char *name) {
    char *data = conf_readString(fn, name);

    if(data != NULL) {
        bool datab = true;

        if(strcmp(data, "true") == 0 || strcmp(data, "TRUE") == 0 || strcmp(data, "1") == 0) {
            *var = true;
            free(data);
            return true;
        } else if(strcmp(data, "false") == 0 || strcmp(data, "false") == 0 || strcmp(data, "0") == 0) {
            *var = false;
            free(data);
            return true;
        } else {
            return false;
        }

        
    } else {
        return false;
    }
    
}

/*
    Search the string attached to keyword name.
    
    fn: String containing file path
    name: String containing the keyword to search

    return: String attached to keyword if found, else return NULL
*/

char *conf_readString(char *fn, char *name) {
    if(strlen(name) == 0)
        return NULL;

    FILE *f = fopen(fn, "r");
    
    char *data = NULL;

    if(f != NULL) {
        bool found = conf_search_for_line(f, name);

        if(found) {
            char *line = (char*) malloc(CONF_MAX_DATA_LENGTH);
            memset(line, 0, sizeof(line));

            int count = 0;
            char c;
            // Reads from file until '\n' or feof or MAX_DATA_LENGTH (limitation of c strings)
            while(!feof(f) && ((c = fgetc(f)) != '\n') && count < CONF_MAX_DATA_LENGTH) {
                line[count] = c;
                ++ count;
            }
            // Memory optimization and string terminator
            if(!feof(f)) {
                data = (char*) realloc(line, count+1);
                data[count] = 0;
            } else {    // If feof the last char of the string is not part of the string, so last = 0;
                data = (char*) realloc(line, count);
                data[count-1] = 0;
            }
        }

        fclose(f);
    }

    return data;
}

/*
    Leaves the file cursor right before data or at the end of file if data doesn't exist.
    Keyword maching is case sensitive and spaces before '=' and the first space after '=' is ignored

    f: Pointer to File
    name: String containing the keyword to search

    return: true if keyword is found
*/
bool conf_search_for_line(FILE *f, char *name) {
    int i;
    rewind(f);
    long int fsize = conf_file_size(f);
    long int count = 0;
    
    char buf[strlen(name) + 3];     // +1 for initial '\n' +1 for final '=' and +1 for '\0'
    memset(buf, ' ', sizeof(buf));
    
    buf[strlen(name) + 1] = '\n';   // Used to simulate a new line before first line
    buf[strlen(name) + 2] = 0;      // String end

    /* namecomp = '\n' + name + '=' and it's used for buffer name comparison */ // works
    char namecomp[strlen(name)+3];
    namecomp[0] = '\n';
    namecomp[strlen(name)+2] = 0;
    strcpy((namecomp+1), name);
    namecomp[strlen(name)+1] = '=';

    while(strcmp(buf, namecomp) != 0 && count < fsize) {     // Exit when the "\n" + name + "=" is found or end file
        char c = fgetc(f);
        if(c != ' ') {  // Ignore space before =
            // Shift array back 1 char and add c at the end
            for(i=0;i<strlen(name) + 2;i++) {
                buf[i] = buf[i+1];
            }
            buf[strlen(name) + 1] = c;
        }
        ++ count;
    }

    char c = fgetc(f);
    if(c != ' ') {      // Ignore first space after '=' if present
        fseek(f, -1L, SEEK_CUR);
    }

    return strcmp(buf, namecomp) == 0;     // true if found, false if eof
}

long int conf_file_size(FILE *f) {
    if(f != NULL) {
        long int current = ftell(f);
        fseek(f, 0L, SEEK_END);
        long int size = ftell(f);
        fseek(f, current, SEEK_SET);

        return size;
    } else {
        return -1;
    }
}


void conf_reach_beginning_line(FILE *f) {
    fseek(f, -1L, SEEK_CUR);

    while(ftell(f) > 0) {
        char c = fgetc(f);
        if(c == '\n') {
            break;
        } else {
            fseek(f, -2L, SEEK_CUR);
        }
    }
}


bool conf_reach_next_line(FILE *f) {
    long size = conf_file_size(f);
    while(ftell(f) < size) {
        char c = fgetc(f);
        if(c == '\n') {
            break;
        }
    }

    return ftell(f) != size;
}

#ifdef __cplusplus
}
#endif