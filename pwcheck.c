/**
 * @name Projekt 1 - Práce s textem
 * @author Jakub Kontrik <xkontr02@stud.fit.vutbr.cz>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/** MAX_LEN maximum possible length of the array into which the password is read 100 characters + \ n  + \0
 * ASCII_MAX number of all characters in ascii
 * STATS_ARG, L_SWITCH, P_SWITCH templates for argument comparison
 */

#define MAX_LEN 102
#define ASCII_MAX 256
#define STATS_ARG "--stats"
#define L_SWITCH "-l"
#define P_SWITCH "-p"

// structure  for stats
typedef struct {
    int minimum;
    int numb_of_char;
    int numb_of_passw;    
    float average;    
    float len_of_all_pass;
} Stats;

// structure for arguments and position of switches
typedef struct {    
    long int param;
    long int level;
    int pos_of_l_switch;
    int pos_of_p_switch;
    int pos_of_stats;

} Arguments;

// function to get the length of the read password in a line, also counts with \n 
int get_lenght(char *password) {                      
    int len = 0;
    while (password[len] != '\0') {
        len++;
    }
    return len;
}

//array comparison function
bool compare_arr(char *arr1,char *arr2) {
    for (int i=0; i <= get_lenght(arr1); i++) {
        if (arr1[i] != arr2[i]) {
            return false;
            }
    }
    return true;
}

//function that passes the read password and finds out if there is a lowercase character in it
int is_lovercase(char *password) {                   
    for (int i = 0; password[i] != '\0'; i++) {
        if ('a' <= password[i] && password[i] <= 'z') {
            return 1; 
        }
    }
    return 0;
}

//function that passes the read password and finds out if there is a uppercase character in it
int is_uppercase(char *password) {                    
    for (int i = 0; password[i] != '\0'; i++) {
        if ('A' <= password[i] && password[i] <= 'Z') {
            return 1;
        }
    }
    return 0;
}

//function that passes the read password and finds out if there is a digit in it
int is_digit(char *password) {                       
    for (int i = 0; password[i] != '\0'; i++) {
        if ('0' <= password[i] && password[i] <= '9') {
            return 1;
        }
    }
    return 0;
}

//function that passes the read password and finds out if there is a special character in it
int is_special(char *password) {
    for (int i = 0; password[i] != '\0'; i++) {
        if ((' ' <= password[i] && password[i] <= '/') || 
            (':' <= password[i] && password[i] <= '@') || 
            ('[' <= password[i] && password[i] <= '`') || 
            ('{' <= password[i] && password[i] <= '~')) {
            return 1;
        }
    }
    return 0;
}

//function to check the length of the read input
bool pw_lenght_check(char *password) {    
    // if in read password there is not \ n (end-of-line character) on the penultimate character  then it is not valid
    return (get_lenght(password) == MAX_LEN-1 && password[get_lenght(password)-1] != '\n');
}

/**function checks if param is in
 * intervals of positive numbers
 * long int to cover as much as possible numbers 
 * + strtol makes non-empty pointer if the param argument 
 * did not contain only numbers
*/ 
bool check_param_arg(char *param_arg, Arguments *ag) {    
    char *ptr;
    ag->param = strtol(param_arg, &ptr, 10);
    return *ptr == '\0' && ag->param > 0;
}

/**function checks if level is in
 * interval from 1 to 4
 * long int type just to have same type like param 
 * + strtol makes non-empty pointer if the param argument 
 * did not contain only numbers
*/ 
bool check_level_arg(char *level_arg, Arguments *ag) {
    char *ptr;
    ag->level = strtol(level_arg, &ptr, 10);
    return *ptr == '\0' && 1 <= ag->level && ag->level <= 4 ;
}

// chcecking 1st level
bool lvl1_check(char *password) {
    return is_uppercase(password) && is_lovercase(password);
}

/**to check 2nd level im returning from these is_... functions 1 so I just sum them up 
 * plus implementation that if param is greater than 4 it is 4 anyway for this level
 */
bool lvl2_check(char *password, Arguments ag) {
    if (ag.param > 4) {
        ag.param = 4;
    }
    return (ag.param <= (is_special(password) + is_digit(password) + is_uppercase(password) + is_lovercase(password)));
}

/**checking 3rd level if char is equal to char next to it
 * I add 1 to same_chars variable 
 * if same_chars is equal to param argument
 * password is incorrect
 */
bool lvl3_check(char *password, Arguments ag) {
    int same_chars = 1; //1 because every char is once there
    for (int i = 0; password[i]!='\0'; i++) {
        if (password[i] == password[i+1]) {
            same_chars++;
        }
        else {
            same_chars = 1;
        }
        if (same_chars == ag.param) {
            return false;
        }
    }
    return true;
}

/** checking substrings in the given password
 * I do not create additional arrays for substrings I initialize the i index-beginning 
 * of the 1st substring and z determines where the substring ends
 * index j shifts the 2nd substring throughout the password
 */
bool lvl4_check(char *password, Arguments ag) {
    long int match = 0;
    //for cycle to the last value that makes sense to control
    for(long int i = 0; i <= get_lenght(password) - ag.param-2; i++) { 
        //if character in position i + j is \n it makes no sense to check further
        for (long int j = 1; password[i+j] != '\n'; j++) { 
            match = 0;
            for (long int z = 0; z < ag.param;z++) {
                if (password[i+z] == password[i+z+j]) {
                    match++;
                }
                if (match == ag.param) {
                    return false;
                }
            }
        }
    }
    return true;
}


/** check password according to selected arguments if lvl
 * arg = 4 so the switch will fall by all cases if any of them fails
 * password requirements it breaks
 */
bool pw_level_check(char *password, Arguments ag) {
    switch(ag.level) {
        case 4:
            if (!lvl4_check(password, ag)) {
                break;
            }
        /* FALLTHROUGH */
        case 3:
            if (!lvl3_check(password, ag)) {
                break;
            }
        /* FALLTHROUGH */
        case 2:
            if (!lvl2_check(password, ag)) {
                break;
            }
        /* FALLTHROUGH */
        case 1:
            if (lvl1_check(password)) {
                return true;
            }
    }
    return false;
}
//each character is stored in the bool array according to its ascii value
void save_unique_char(bool *bool_array, char *password) {
    //every password has \n and therefore I check up to this symbol
    for (int i=0; password[i] != '\n'; i++) { 
        int ascii_number = password[i]; //typecasting the char to its ascii value
        bool_array[ascii_number] = true;
    }
}
//function that counts how many true values were in an array and therefore how many unique characters
void count_unique_char(bool *bool_array, Stats *st) {
    for (int i= 0; i < ASCII_MAX; i++) {
        if (bool_array[i] == true) {
            st->numb_of_char++;
        }
    }
}

/**finding the minimum password length (-1 because / n) if the password length is
 * less than the minimum it becomes minimum
 */
void get_min(char *array, Stats *st) {
    if (st->minimum > get_lenght(array) - 1) {
        st->minimum = get_lenght(array) - 1;
    }
}

//printing stats
void print_stats(Stats st) {
    printf("Statistika:\n");
    printf("Ruznych znaku: %d\n", st.numb_of_char);
    printf("Minimalni delka: %d\n", st.minimum);
    printf("Prumerna delka: %0.1f\n", st.average);
}

//whole control of passwords + printing them if they are valid and working with stats
void manage_passwords(char *password, bool *bool_array, Arguments ag, Stats *st) {   
    st->numb_of_passw++;
    st->len_of_all_pass += get_lenght(password) - 1; //-1 because of \n
    get_min(password, st);
    save_unique_char(bool_array, password);

    if (pw_level_check(password, ag)) {
        printf("%s", password);
    }
}

// search for -p or -l or --stats arguments + save their position , needed for bonus solution
bool is_switch(char **argument,char *template, int number_of_arg, int *position) {
    for (int i = 1; i < number_of_arg; i++) {
        if (compare_arr(argument[i], template)) {
            *position = i;
            return true;
        }
    }
    
    return false;

}

//check arguments working with every possibility that may occur
bool manage_arguments(char **agrv, int number_of_arg, Arguments *ag) {
    if (number_of_arg == 5 || number_of_arg == 6) {  // maximum arguments that may occur
        if ((is_switch(agrv, L_SWITCH, number_of_arg, &ag->pos_of_l_switch) && 
        is_switch(agrv, P_SWITCH, number_of_arg, &ag->pos_of_p_switch))) { // there have to be both of the switches
            //the level and param arguments must always be correct, they are always one position after switch position
            return (check_level_arg(agrv[ag->pos_of_l_switch+1], ag)  && check_param_arg(agrv[ag->pos_of_p_switch+1], ag));  
        }
    }
    // option when there is either only one switch or none = basic solution
    else if (number_of_arg == 3 || number_of_arg == 4) { 
        //if there is no level switch and argument then default value = 1
        if ((!is_switch(agrv, L_SWITCH, number_of_arg, &ag->pos_of_l_switch) && 
        is_switch(agrv, P_SWITCH, number_of_arg, &ag->pos_of_p_switch))) {
            ag->level = 1;
            return check_param_arg(agrv[ag->pos_of_p_switch+1], ag);
        }
        //if there is no param switch and argument then default value = 1 
        else if ((is_switch(agrv, L_SWITCH, number_of_arg, &ag->pos_of_l_switch) && 
        !is_switch(agrv, P_SWITCH, number_of_arg, &ag->pos_of_p_switch)) ) {
            ag->param = 1;
            return check_level_arg(agrv[ag->pos_of_l_switch+1], ag);
        }
        else if ((!is_switch(agrv, L_SWITCH, number_of_arg, &ag->pos_of_l_switch) && 
        !is_switch(agrv, P_SWITCH, number_of_arg, &ag->pos_of_p_switch))) {
            // check the basic assignment arguments, if 4 argumets are given there have to be --stats
            if (number_of_arg == 4 && !is_switch(agrv, STATS_ARG, number_of_arg, &ag->pos_of_stats)) {
                return false;
            }
            // the basic solution has arguments in the given positions
            return (check_level_arg(agrv[1], ag)  && check_param_arg(agrv[2], ag));  
        }
    }
    //if there is no given argument and therefore only a path, then the default values are 1
    else if (number_of_arg == 1 ||
    // if 2 arguments are given then there have to be --stats
    (number_of_arg == 2 && is_switch(agrv, STATS_ARG, number_of_arg, &ag->pos_of_stats))) {
        ag->level = 1;
        ag->param = 1;
        return true;
            }
    return false;
}

int main(int agrc, char **agrv) {   
    Stats stat = {.minimum = MAX_LEN};
    Arguments arg = {0};
    bool bool_buffer[ASCII_MAX] = {false};
    char buffer[MAX_LEN];
    
    //firstly cecking given arguments
    if (!manage_arguments(agrv, agrc, &arg)) {
        fprintf(stderr,"Error: arguments are incorrect!\n");
        return 1;
    }
    //reading passwords from input and processing them
    while(fgets(buffer, MAX_LEN, stdin) != NULL) {
        if(pw_lenght_check(buffer)) {
            fprintf(stderr, "Error: wrong input, password is over 100 symbols!\n");
            return 2;
        }
        manage_passwords(buffer,bool_buffer, arg, &stat);
    }
    if (stat.numb_of_passw == 0) {
        fprintf(stderr, "Error: wrong input, 0 passwords given!\n");
        return 3;
    }

    //processing and calculating stats
    count_unique_char(bool_buffer, &stat);
    stat.average = stat.len_of_all_pass / stat.numb_of_passw;
    if (is_switch(agrv, STATS_ARG, agrc, &arg.pos_of_stats)) {
        print_stats(stat);
    }
    
    return 0;
}