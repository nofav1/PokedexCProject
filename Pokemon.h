//
// Created by nofarav on 1/30/24.
//

#ifndef ASSIGNMENT2_POKEMON_H
#define ASSIGNMENT2_POKEMON_H

#include "Defs.h"

//structs:
struct pokemonType{
    char* type;
    int amount;
    struct pokemonType** effective_against_me;
    struct pokemonType** effective_against_others;
    int num_of_types_against_me;
    int num_of_types_against_others;
};

struct biologicInfo{
    double height;
    double weight;
    int attack;
};

struct pokemon{
    char* name;
    char* species;
    struct pokemonType* type;
    struct biologicInfo* bio;
};

//functions:
status createPokemonType(struct pokemonType** pkType, char* name);

status freePokemonType(struct pokemonType* pkType);

status createPokemon(struct pokemon** pokemon, struct pokemonType** pkt_list, char* name, char* species, double height, double weight, int attack, char* type);

status createPokemonBio(struct pokemon* p, double height, double weight, int attack);

status freePokemons(struct pokemon* pokemon);

status AddEffectiveAgainstMe(struct pokemonType* pokemon_type1, struct pokemonType* pokemon_type2);

status AddEffectiveAgainstOther(struct pokemonType* pokemon_type1, struct pokemonType* pokemon_type2);

status DelEffectiveAgainstMe(struct pokemonType* pokemon_type1, struct pokemonType* pokemon_type2);

status DelEffectiveAgainstOthers(struct pokemonType* pokemon_type1, struct pokemonType* pokemon_type2);

status printPokemon(struct pokemon* p);

status printPokemonType(struct pokemonType* pt);

#endif //ASSIGNMENT2_POKEMON_H
