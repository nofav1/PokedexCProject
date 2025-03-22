#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Defs.h"
#include "Pokemon.h"

#define MAX_LINE_LENGTH 302

//declarations
status remove_special_chars(char *input_string, const char *chars_to_remove);
struct pokemonType* findType(struct pokemonType** pkt_list, char *type);
void printAllPokemons(struct pokemon** pokemonList, int numberOfPokemons);
void printAllPokemonTypes(struct pokemonType** pokemonTypeList, int numberOfTypes);
void addTypeToEffectiveAgainstMe(struct pokemonType* t1, struct pokemonType* t2);
void addTypeToEffectiveAgainstOthers(struct pokemonType* t1, struct pokemonType* pt2);
void removeTypeFromEffectiveAgainstMe(struct pokemonType* t1, struct pokemonType* t2);
void removeTypeFromEffectiveAgainstOthers(struct pokemonType* t1, struct pokemonType* t2);
void printPokemonByName(struct pokemon* p);
void printPokemonsByType(struct pokemon** pokemonList, int numberOfPokemons, struct pokemonType* pt);

//helpers functions

//remove all chars_to_remove from input_string
status remove_special_chars(char *input_string, const char *chars_to_remove){
    if(input_string != NULL & chars_to_remove != NULL) {
        size_t len = strlen(chars_to_remove);

        for (size_t i = 0; i < len; i++) {
            char *found = strchr(input_string, chars_to_remove[i]);

            //remove all occurrences of the character
            while (found != NULL) {
                memmove(found, found + 1, strlen(found));
                found = strchr(input_string, chars_to_remove[i]);
            }
        }
        return success;
    } else {return invalid_input;}
}

//find the type and return pokemonType
struct pokemonType* findType(struct pokemonType** pkt_list, char *type){
    int counter = 0;
    while(pkt_list[counter] != NULL){
        if(strcmp(pkt_list[counter]->type, type) == 0){ //type found
            return pkt_list[counter];
        }
        counter++;
    }
    return NULL;
}

void printAllPokemons(struct pokemon** pokemonList, int numberOfPokemons){
    for(int i = 0; i < numberOfPokemons; i++){
        printPokemon(pokemonList[i]);
    }
}

void printAllPokemonTypes(struct pokemonType** pokemonTypeList, int numberOfTypes){
    for(int i =0; i < numberOfTypes; i++){
        printPokemonType(pokemonTypeList[i]);
    }
}

void addTypeToEffectiveAgainstMe(struct pokemonType* t1, struct pokemonType* t2){
    status s = AddEffectiveAgainstMe(t1, t2);
    if(s == type_exist){
        printf("This type already exist in the list.\n");
    }
    else if(s == allocation_failure){
        printf("Allocation failed with add type effective against me");
    }
    else if(s == success){
        printPokemonType(t1); //print pokemon type info after add new type
    }
}

void addTypeToEffectiveAgainstOthers(struct pokemonType* t1, struct pokemonType* t2){
    status s = AddEffectiveAgainstOther(t1, t2);
    if(s == type_exist){
        printf("This type already exist in the list.\n");
    }
    else if(s == allocation_failure){
        printf("Allocation failed with add type effective against others");
    }
    else if(s == success){
        printPokemonType(t1); //print pokemon type info after add new type
    }
}

void removeTypeFromEffectiveAgainstMe(struct pokemonType* t1, struct pokemonType* t2){
    status s = DelEffectiveAgainstMe(t1, t2);
    if(s == type_not_exist){
        printf("Type name doesn't exist in the list.\n");
    }
    else if(s == success){
        printPokemonType(t1); //print pokemon type info after add new type
    }
}

void removeTypeFromEffectiveAgainstOthers(struct pokemonType* t1, struct pokemonType* t2){
    status s = DelEffectiveAgainstOthers(t1, t2);
    if(s == type_not_exist){
        printf("Type name doesn't exist in the list.\n");
    }
    else if(s == success){
        printPokemonType(t1); //print pokemon type info after add new type
    }
}

void printPokemonByName(struct pokemon* p){
    printPokemon(p);
}

void printPokemonsByType(struct pokemon** pokemonList, int numberOfPokemons, struct pokemonType* pt){
    //printPokemonType(pt);
    if(pt->amount == 0){
        printf("There are no Pokemons with this type.\n");
    }
    else{
        printf("There are %d Pokemons with this type:\n", pt->amount);
        for(int i = 0; i < numberOfPokemons; i ++){
            if(strcmp(pokemonList[i]->type->type, pt->type) == 0){ //pokemon with this type
                printPokemon(pokemonList[i]);
            }
        }
    }
}


int main(int argc, char* argv[]) {
    char *endptr; // to store the address of the first invalid character
    //save the arguments
    int numberOfTypes = (int)strtol(argv[1], &endptr, 10);
    int numberOfPokemons = (int)strtol(argv[2], &endptr, 10);
    char* configurationFile = argv[3];

    //pokemons and types lists
    struct pokemon* pokemonList[numberOfPokemons];
    struct pokemonType* pokemonTypeList[numberOfTypes];

    // open the file for reading
    FILE *file = fopen(configurationFile, "r");

    //checks if file opened successfully
    if (file == NULL){
        perror("Error opening file");
        return 1;
    }

    //buffer to store each line
    char buffer[MAX_LINE_LENGTH];

    bool isTypes = false;
    bool isPokemons = false;
    status rsc_status, s;
    char *token;


    // read the file line by line
    while (fgets(buffer, sizeof(buffer), file) != NULL){
        rsc_status = remove_special_chars(buffer, "\n\t\r");
        if(rsc_status != success){
            printf("%u in remove_special_chars", rsc_status);
            break;
        }
        if(strcmp(buffer, "Types") == 0){isTypes = true;} //first line - Types
        else if (isTypes){ //second line - type1, type2, ...
            //tokenize the line using strtok
            token = strtok(buffer, ",");

            //add all types to pokemonTypeList
            for (int i = 0; i < numberOfTypes; i ++){
                s = createPokemonType(&pokemonTypeList[i], token); //function that need to return a pokemon type address
                if(s == allocation_failure){
                    printf("Memory Problem");
                    break;
                }
                token = strtok(NULL, ","); //to get the next token
            }
            fgets(buffer, sizeof(buffer), file); //next line
            rsc_status = remove_special_chars(buffer, "\n\t\r");
            if(rsc_status != success){
                printf("%u in remove_special_chars", rsc_status);
                break;
            }
            while (strcmp(buffer, "Pokemons") != 0){ //effective against lines
                //find the relevent info from line
                token = strtok(buffer, " ");
                char *type1 = token;
                token = strtok(NULL, " ");
                char *restLine = token;

                token = strtok(restLine, ":");
                char *against = token;
                token = strtok(NULL, ":");
                char *otherTypeList = token;

                token = strtok(against, "-"); //word - "effective
                token = strtok(NULL, "-"); //word - against
                token = strtok(NULL, "-"); //word - me/other
                char *me_or_other = token;

                struct pokemonType* pokemon_type1 = findType(pokemonTypeList, type1);
                struct pokemonType* pokemon_type2;
                //iterate on other types after the ":"
                token = strtok(otherTypeList, ",");
                while(token != NULL){
                    pokemon_type2 = findType(pokemonTypeList, token);
                    if(strcmp(me_or_other, "me") == 0){ //effective_against_me
                        s = AddEffectiveAgainstMe(pokemon_type1, pokemon_type2);
                        if(s != success){
                            printf("%u in addEffectivegainstMe funcion\n", s);
                        }
                    }
                    else{ //effective_against_other
                        s = AddEffectiveAgainstOther(pokemon_type1, pokemon_type2);
                        if(s != success){
                            printf("%u in addEffectivegainstOthers funcion\n", s);
                        }
                    }
                    //get the next type
                    token = strtok(NULL, ",");
                }


                fgets(buffer, sizeof(buffer), file); //next line
                rsc_status = remove_special_chars(buffer, "\n\t\r");
                if(rsc_status != success){
                    printf("%u in remove_special_chars", rsc_status);
                    break;
                }
            }
            isTypes = false;
            isPokemons = true; //line - Pokemons
        }
        else if (isPokemons){ //pokemons list
            char *pokemon_name, *pokemon_species, *pokemon_type_string;
            double pokemon_height, pokemon_weight;
            int pokemon_attack;

            for (int i = 0; i < numberOfPokemons; i++){
                //gets one pokemons' details

                // use strtok to split the string by comma
                token = strtok(buffer, ",");

                pokemon_name = token;
                // Get the next token
                token = strtok(NULL, ",");

                pokemon_species = token;
                // Get the next token
                token = strtok(NULL, ",");

                pokemon_height = strtod(token, NULL);
                // Get the next token
                token = strtok(NULL, ",");

                pokemon_weight = strtod(token, NULL);
                // Get the next token
                token = strtok(NULL, ",");

                char *endptr1;
                pokemon_attack = (int)strtol(token, &endptr1,10);
                // Get the next token
                token = strtok(NULL, ",");

                pokemon_type_string = token;

                s = createPokemon(&pokemonList[i], pokemonTypeList, pokemon_name, pokemon_species, pokemon_height, pokemon_weight, pokemon_attack, pokemon_type_string);

                fgets(buffer, sizeof(buffer), file);
                rsc_status = remove_special_chars(buffer, "\n\t\r");
                if(rsc_status != success){
                    printf("%u in remove_special_chars", rsc_status);
                    break;
                }
            }
        }

    }

    if (fclose(file) != 0){
        perror("Error closing file");
        return 1;
    }
    //menu
    char *menu = "Please choose one of the following numbers:\n1 : Print all Pokemons\n2 : Print all Pokemons types\n3 : Add type to effective against me list\n"
                 "4 : Add type to effective against others list\n5 : Remove type from effective against me list\n"
                 "6 : Remove type from effective against others list\n7 : Print Pokemon by name\n"
                 "8 : Print Pokemons by type\n9 : Exit\n";
    char input[300];
    char type1[300], type2[300];
    struct pokemonType *t1, *t2;
    struct pokemon* p;
    bool exist;
    printf("%s", menu);
    scanf("%299s", input);
    bool exit = false;
    int choice;
    while(!exit) { //while it's not exit
        //checks if input is between 1 and 9
        if (input[0] >= '1' && input[0] <= '9' && strlen(input) == 1) {
            // process user choice using a switch case
            choice = (int)strtol(input, &endptr, 10);
        } else {
            choice = 0; //for default
        }

        switch (choice) {
            case 1: //print all pokemons
                printAllPokemons(pokemonList, numberOfPokemons);
                printf("%s", menu);
                scanf("%299s", input);
                break;
            case 2: //print all pokemons types
                printAllPokemonTypes(pokemonTypeList, numberOfTypes);
                printf("%s", menu);
                scanf("%299s", input);
                break;
            case 3: //add type to effective against me list
                printf("Please enter type name:\n");

                scanf("%s", type1);
                exist = false;
                for(int i = 0; i < numberOfTypes; i++){
                    if(strcmp(pokemonTypeList[i]->type, type1) == 0){
                        exist = true;
                        t1 = pokemonTypeList[i];
                        break;
                    }
                }
                if(exist){
                    exist = false;
                    printf("Please enter type name to add to %s effective against me list:\n", type1);
                    scanf("%s", type2);
                    for(int i = 0; i < numberOfTypes; i++){
                        if(strcmp(pokemonTypeList[i]->type, type2) == 0){
                            exist = true;
                            t2 = pokemonTypeList[i];
                            break;
                        }
                    }
                    if(exist){addTypeToEffectiveAgainstMe(t1, t2);} //add type2 to type1 effective_against_me list
                    else{
                        printf("Type name doesn't exist.\n");
                    }
                }
                else{
                    printf("Type name doesn't exist.\n");
                }
                printf("%s", menu);
                scanf("%299s", input);
                break;
            case 4: //add type to effective against others list
                printf("Please enter type name:\n");
                scanf("%s", type1);
                exist = false;
                for(int i = 0; i < numberOfTypes; i++){
                    if(strcmp(pokemonTypeList[i]->type, type1) == 0){
                        exist = true;
                        t1 = pokemonTypeList[i];
                        break;
                    }
                }
                if(exist){
                    exist = false;
                    printf("Please enter type name to add to %s effective against others list:\n", type1);
                    scanf("%s", type2);
                    for(int i = 0; i < numberOfTypes; i++){
                        if(strcmp(pokemonTypeList[i]->type, type2) == 0){
                            exist = true;
                            t2 = pokemonTypeList[i];
                            break;
                        }
                    }
                    if(exist){addTypeToEffectiveAgainstOthers(t1, t2);} //add type2 to type1 effective_against_me list
                    else{
                        printf("Type name doesn't exist.\n");
                    }
                }
                else{
                    printf("Type name doesn't exist.\n");
                }
                printf("%s", menu);
                scanf("%299s", input);
                break;
            case 5: //remove type from effective against me
                printf("Please enter type name:\n");
                scanf("%s", type1);
                exist = false;
                for(int i = 0; i < numberOfTypes; i++){
                    if(strcmp(pokemonTypeList[i]->type, type1) == 0){
                        exist = true;
                        t1 = pokemonTypeList[i];
                        break;
                    }
                }
                if(exist){
                    exist = false;
                    printf("Please enter type name to remove from %s effective against me list:\n", type1);
                    scanf("%s", type2);
                    for(int i = 0; i < numberOfTypes; i++){
                        if(strcmp(pokemonTypeList[i]->type, type2) == 0){
                            exist = true;
                            t2 = pokemonTypeList[i];
                            break;
                        }
                    }
                    if(exist){removeTypeFromEffectiveAgainstMe(t1, t2);} //add type2 to type1 effective_against_me list
                    else{
                        printf("Type name doesn't exist in the list.\n");
                    }
                }
                else{
                    printf("Type name doesn't exist.\n");
                }
                printf("%s", menu);
                scanf("%299s", input);
                break;
            case 6: //remove type from effective against others
                printf("Please enter type name:\n");
                scanf("%s", type1);
                exist = false;
                for(int i = 0; i < numberOfTypes; i++){
                    if(strcmp(pokemonTypeList[i]->type, type1) == 0){
                        exist = true;
                        t1 = pokemonTypeList[i];
                        break;
                    }
                }
                if(exist){
                    exist = false;
                    printf("Please enter type name to remove from %s effective against others list:\n", type1);
                    scanf("%s", type2);
                    for(int i = 0; i < numberOfTypes; i++){
                        if(strcmp(pokemonTypeList[i]->type, type2) == 0){
                            exist = true;
                            t2 = pokemonTypeList[i];
                            break;
                        }
                    }
                    if(exist){removeTypeFromEffectiveAgainstOthers(t1, t2);} //add type2 to type1 effective_against_me list
                    else{
                        printf("Type name doesn't exist in the list.\n");
                    }
                }
                else{
                    printf("Type name doesn't exist.\n");
                }
                printf("%s", menu);
                scanf("%299s", input);
                break;
            case 7: //print pokemon by name
                printf("Please enter Pokemon name:\n");
                scanf("%s", input);
                exist = false;
                for(int i = 0; i < numberOfPokemons; i ++){
                    if(strcmp(pokemonList[i]->name, input) == 0){
                        p = pokemonList[i];
                        exist = true;
                        break;
                    }
                }
                if(exist){printPokemonByName(p);}
                else{
                    printf("The Pokemon doesn't exist.\n");
                }
                printf("%s", menu);
                scanf("%299s", input);
                break;
            case 8: //print pokemons by type
                printf("Please enter type name:\n");
                scanf("%s", input);
                exist = false;
                for(int i = 0; i < numberOfTypes; i ++){
                    if(strcmp(pokemonTypeList[i]->type, input) == 0){
                        t1 = pokemonTypeList[i];
                        exist = true;
                        break;
                    }
                }
                if(exist){printPokemonsByType(pokemonList, numberOfPokemons, t1);}
                else{
                    printf("Type name doesn't exist.\n");
                }
                //printPokemonsByType();
                printf("%s", menu);
                scanf("%99s", input);
                break;
            case 9:
                printf("All the memory cleaned and the program is safely closed.\n");
                exit = true;
                break;
            default:
                printf("Please choose a valid number.\n");
                printf("%s", menu);
                scanf("%299s", input);
        }
    }

    //frees:
    status freePokemonType_status;
    //free pokemonsTypeList
    for(int i = 0; i < numberOfTypes; i++){
        freePokemonType_status = freePokemonType(pokemonTypeList[i]);
        if(freePokemonType_status != success){
            printf("%u in freePokemonType", freePokemonType_status);
            //break;
        }
        pokemonTypeList[i] = NULL;
    }

    status freePokemon_status;
    //free pokemonList
    for(int i = 0; i < numberOfPokemons; i++){
        freePokemon_status = freePokemons(pokemonList[i]);
        if(freePokemon_status != success){
            printf("%u in freePokemon", freePokemon_status);
            //break;
        }
        pokemonList[i] = NULL;
    }

    return EXIT_SUCCESS;
}
