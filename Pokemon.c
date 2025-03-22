//
// Created by nofarav on 1/30/24.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Pokemon.h"
#include "Defs.h"

//execute!!!
status createPokemonType(struct pokemonType** pkType, char* name){
    //allocate memory for the new pokemonType
    *pkType = (struct pokemonType*)malloc(sizeof(struct pokemonType));

    if(*pkType != NULL){
        //allocate memory for the type and copy the name
        (*pkType)->type = strdup(name);

        if((*pkType)->type != NULL){
            //initialize other fields
            (*pkType)->amount = 0;
            (*pkType)->effective_against_me = NULL;
            (*pkType)->effective_against_others = NULL;
            (*pkType)->num_of_types_against_me = 0;
            (*pkType)->num_of_types_against_others = 0;

            return success;
        }
        else{
            free(*pkType);
            *pkType = NULL;
            printf("allocation failure for the pkType->type");
            return allocation_failure;
        }
    }
    else{
        printf("allocation failure for the pkType");
        return allocation_failure;
    }
}

//free pokemon type
status freePokemonType(struct pokemonType* pkType){
    if(pkType != NULL) {
        free(pkType->type);
        free(pkType->effective_against_me);
        free(pkType->effective_against_others);
        free(pkType);
        return success;
    }
    return invalid_input;
}

//create pokemon bio
status createPokemonBio(struct pokemon* p, double height, double weight, int attack){
    // Allocate memory for the pokemon bio
    p->bio = (struct biologicInfo*) malloc(sizeof(struct biologicInfo));

    if(p->bio == NULL){
        free(p->bio);
        return allocation_failure;
    }

    p->bio->height = height;
    p->bio->weight = weight;
    p->bio->attack = attack;

    return success;

}

//create pokemon
status createPokemon(struct pokemon** p, struct pokemonType** pkt_list, char* name, char* species, double height, double weight, int attack, char* type){
    //allocate memory for the new pokemon
    *p = (struct pokemon*)malloc(sizeof(struct pokemon));

    if(*p != NULL){
        //allocate memory for the name and copy the name
        (*p)->name = strdup(name);

        //allocate memory for the species and copy the species
        (*p)->species = strdup(species);

        if ((*p)->name == NULL || (*p)->species == NULL) {
            free((*p)->name);
            free((*p)->species);
            free(*p);
            printf("Memory allocation failed for strings\n");
            return allocation_failure;
        }

        //creates the pokemon bio
        createPokemonBio(*p, height, weight, attack);


        //find the type
        int counter = 0;
        while(pkt_list[counter] != NULL){
            if(strcmp(pkt_list[counter]->type, type) == 0){ //type found
                (*p)->type = pkt_list[counter];
                (*p)->type->amount ++; //increase the amount of this type
                break;
            }
            counter++;
        }

        return success;

    }
    else {
        printf("allocation failure for the pokemon");
        return allocation_failure;
    }
}

status freePokemons(struct pokemon* pokemon){
    if(pokemon != NULL) {
        free(pokemon->name);
        free(pokemon->species);
        free(pokemon->bio);
        pokemon->type = NULL;
        free(pokemon);
        return success;
    }
    return invalid_input;
}

//add type2 to effective_againt_me list of type1
status AddEffectiveAgainstMe(struct pokemonType* pokemon_type1, struct pokemonType* pokemon_type2){
    if(pokemon_type1 == NULL || pokemon_type2 == NULL){
        return invalid_input;
    }
    else if(pokemon_type1->effective_against_me == NULL){ //make a new list
        //allocate memory for the new type in effective_againt_me list
        pokemon_type1->effective_against_me = (struct pokemonType**)malloc(sizeof(struct pokemonType*));
        pokemon_type1->effective_against_me[0] = pokemon_type2; //first element is type2
        pokemon_type1->num_of_types_against_me++; //add 1 to number of types effective against me
    }
    else{ //the list against_me is not empty
        //find if type2 is already exist in list
        bool exist = false;
        for (int i = 0; i < pokemon_type1->num_of_types_against_me; i++){
            if(strcmp(pokemon_type1->effective_against_me[i]->type, pokemon_type2->type) == 0){
                exist = true;
                break;
            }
        }
        if(!exist){ //type2 doesn't exist in list, add type to list
            pokemon_type1->num_of_types_against_me++;
            struct pokemonType** realloc_list = (struct pokemonType**)realloc(pokemon_type1->effective_against_me ,pokemon_type1->num_of_types_against_me * sizeof(struct pokemonType*));
            if(realloc_list == NULL){
                return allocation_failure;
            }
            else{
                pokemon_type1->effective_against_me = realloc_list;
                pokemon_type1->effective_against_me[pokemon_type1->num_of_types_against_me - 1] = pokemon_type2;
            }
        }
        else{ //type2 is already exist in list
            return type_exist;
        }
    }
    return success;
}

//add type2 to effective_againt_other list of type1
status AddEffectiveAgainstOther(struct pokemonType* pokemon_type1, struct pokemonType* pokemon_type2){
    if(pokemon_type1 == NULL || pokemon_type2 == NULL){
        return invalid_input;
    }
    else if(pokemon_type1->effective_against_others == NULL){ //make a new list
        //allocate memory for the new type in effective_againt_me list
        pokemon_type1->effective_against_others = (struct pokemonType**)malloc(sizeof(struct pokemonType*));
        pokemon_type1->effective_against_others[0] = pokemon_type2; //first element is type2
        pokemon_type1->num_of_types_against_others++; //add 1 to number of types effective against others
    }
    else{ //the list against_me is not empty
        //find if type2 is already exist in list
        bool exist = false;
        for (int i = 0; i < pokemon_type1->num_of_types_against_others; i++){
            if(strcmp(pokemon_type1->effective_against_others[i]->type, pokemon_type2->type) == 0){
                exist = true;
                break;
            }
        }
        if(!exist){ //type2 doesn't exist in list, add type to list
            pokemon_type1->num_of_types_against_others++;
            struct pokemonType** realloc_list = (struct pokemonType**)realloc(pokemon_type1->effective_against_others ,pokemon_type1->num_of_types_against_others * sizeof(struct pokemonType*));
            if(realloc_list == NULL){
                return allocation_failure;
            }
            else{
                pokemon_type1->effective_against_others = realloc_list;
                pokemon_type1->effective_against_others[pokemon_type1->num_of_types_against_others - 1] = pokemon_type2;
            }
        }
        else{ //type2 is already exist in list
            return type_exist;
        }
    }
    return success;
}

//delete type2 from effective_againt_me list of type1. if not exist - return faliure
status DelEffectiveAgainstMe(struct pokemonType* pokemon_type1, struct pokemonType* pokemon_type2){
    bool exist = false;
    int counter = 0;
    if(pokemon_type1->effective_against_me != NULL) { //if list is not empty
        //checks if type2 exists in type1 list
        for (int i = 0; i < pokemon_type1->num_of_types_against_me; i++) {
            if (strcmp(pokemon_type1->effective_against_me[i]->type, pokemon_type2->type) == 0) {
                exist = true;
                break;
            }
            counter++;
        }
        if (exist) {
            //puts type2 in the end of the list without changing the others positions
            for (int i = counter; i < pokemon_type1->num_of_types_against_me - 1; i++) {
                pokemon_type1->effective_against_me[i] = pokemon_type1->effective_against_me[i + 1];
            }
            pokemon_type1->num_of_types_against_me--; //minus one type
            if (pokemon_type1->num_of_types_against_me == 0) { //delete last element in list, now list is empty
                free(pokemon_type1->effective_against_me);
                pokemon_type1->effective_against_me = NULL;
                return success;
            }
            struct pokemonType **realloc_list = (struct pokemonType **) realloc(pokemon_type1->effective_against_me,
                                                                                pokemon_type1->num_of_types_against_me *
                                                                                sizeof(struct pokemonType *));
            if (realloc_list == NULL) {
                return allocation_failure;
            } else {
                pokemon_type1->effective_against_me = realloc_list; //decrease effective_against_me by one
            }
            return success;
        } else { //type2 does not exist in effective_against_me type1 list
            return type_not_exist;
        }
    }
    else{ //list is empty
        return invalid_input;
    }

}

//delete type2 from effective_againt_others list of type1. if not exist - return faliure
status DelEffectiveAgainstOthers(struct pokemonType* pokemon_type1, struct pokemonType* pokemon_type2){
    bool exist = false;
    int counter = 0;
    if(pokemon_type1->effective_against_others != NULL) { //if list is not empty
        //checks if type2 exists in type1 list
        for (int i = 0; i < pokemon_type1->num_of_types_against_others; i++) {
            if (strcmp(pokemon_type1->effective_against_others[i]->type, pokemon_type2->type) == 0) {
                exist = true;
                break;
            }
            counter++;
        }
        if (exist) {
            //puts type2 in the end of the list without changing the others positions
            for (int i = counter; i < pokemon_type1->num_of_types_against_others - 1; i++) {
                pokemon_type1->effective_against_others[i] = pokemon_type1->effective_against_others[i + 1];
            }
            pokemon_type1->num_of_types_against_others--; //minus one type
            if (pokemon_type1->num_of_types_against_others == 0) { //delete last element in list, now list is empty
                free(pokemon_type1->effective_against_others);
                pokemon_type1->effective_against_others = NULL;
                return success;
            }
            struct pokemonType **realloc_list = (struct pokemonType **) realloc(pokemon_type1->effective_against_others,
                                                                                pokemon_type1->num_of_types_against_others *
                                                                                sizeof(struct pokemonType *));
            if (realloc_list == NULL) {
                return allocation_failure;
            } else {
                pokemon_type1->effective_against_others = realloc_list; //decrease effective_against_others by one
            }
            return success;
        } else { //type2 does not exist in effective_against_others type1 list
            return type_not_exist;
        }
    }
    else{ //list is empty
        return invalid_input;
    }
}

status printPokemon(struct pokemon* p){
    if(p->bio == NULL || p->species == NULL || p->name == NULL || p->type == NULL){
        return failure;
    }
    printf("%s :\n%s, %s Type.\nHeight: %.2f m    Weight: %.2f kg    Attack: %d\n\n", p->name, p->species, p->type->type, p->bio->height, p->bio->weight, p->bio->attack);
    return success;
}

status printPokemonType(struct pokemonType* pt){
    if(pt->type == NULL){
        return failure;
    }
    printf("Type %s -- %d pokemons", pt->type, pt->amount);
    if(pt->num_of_types_against_me > 0){
        printf("\n\tThese types are super-effective against %s:", pt->type);
        for(int i = 0; i < pt->num_of_types_against_me; i++){
            printf("%s",pt->effective_against_me[i]->type);
            if(pt->num_of_types_against_me > i + 1){ printf(" ,");} //there is another type
        }
    }
    if(pt->num_of_types_against_others > 0){
        printf("\n\t%s moves are super-effective against:", pt->type);
        for(int i = 0; i < pt->num_of_types_against_others; i++){
            printf("%s",pt->effective_against_others[i]->type);
            if(pt->num_of_types_against_others > i + 1){ printf(" ,");} //there is another type
        }
    }
    printf("\n\n");
    return success;
}