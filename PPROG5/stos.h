#ifndef STOS_H
#define STOS_H

#include <stdio.h>
#include <stdlib.h>

#define BLAD -1 /* wartosc bledu funkcji */

/* struktura zawierajaca element listy */
typedef struct element {
  int wartosc; /* wartosc przechowywana przez element */
  struct element *nastepny; /* wskaznik na kolejny element */
} elem_listy;

/************************************************************************/
/* Funkcja inicjuje liste (stos)                                        */
/* Argumenty funkcji:                                                   */
/*         stos -  pierwszy element stosu                               */
/* PRE:                                                                 */
/*      brak                                                            */
/* POST:                                                                */
/*      inicjuje liste (stos) i sprawdza czy inicjalizacja przebiegla   */
/*      prawidlowo; zwraca wartosc 0, gdy zadzialala poprawnie lub      */
/*      wartosc BLAD, gdy stos nie zostal poprawnie zainicjowany        */
/************************************************************************/
int inicjuj(elem_listy **stos);

/************************************************************************/
/* Funkcja sprawdza, czy stos jest pusty                                */
/* Argumenty funkcji:                                                   */
/*         stos -  pierwszy element stosu                               */
/* PRE:                                                                 */
/*      brak                                                            */
/* POST:                                                                */
/*      zwraca wartosc 0, gdy sprawdzany stos jest pusty lub wartosc 1, */
/*      gdy stos nie jest pusty                                         */
/************************************************************************/
int empty(elem_listy *stos);

/************************************************************************/
/* Procedura drukuje na wyjsciu cala zawartosc stosu                    */
/* Argumenty procedury:                                                 */
/*         stos -  pierwszy element stosu                               */
/* PRE:                                                                 */
/*      brak                                                            */
/* POST:                                                                */
/*      sprawdza, czy stos jest pusty (jesli tak to wyswietla odpowiedni*/ 
/*      komunikat), drukuje na wyjsciu cala zawartosc stosu             */
/************************************************************************/
void print(elem_listy *stos);

/************************************************************************/
/* Funkcja zdejmuje (i usuwa) element ze szczytu stosu                  */
/* Argumenty funkcji:                                                   */
/*         stos  -  pierwszy element stosu                              */
/*         arg   -  wartosc przypisana do usuwanego elementu            */
/* PRE:                                                                 */
/*      istnieje co najmniej jeden element stosu, czyli stos nie jest   */
/*      pusty                                                           */
/* POST:                                                                */
/*      pobiera wartosc przypisana do usuwanego elementu i zwalnia jego */
/*      pamiec; zwraca wartosc 0, gdy zadzialala poprawnie lub wartosc  */
/*      BLAD, gdy stos nie jest pusty                                   */
/************************************************************************/
int pop(elem_listy **stos, int *arg);

/************************************************************************/
/* Funkcja wklada element na szczyt stosu                               */
/* Argumenty funkcji:                                                   */
/*         stos  -  pierwszy element stosu                              */
/*         arg   -  wartosc przypisywana do nowego elementu             */
/* PRE:                                                                 */
/*      brak                                                            */
/* POST:                                                                */
/*      alokuje pamiec dla nowego elementu, sprawdza poprawnosc         */
/*      alokacji, przypisuje podana liczbe do wartosci elementu, adres  */
/*      pierwszego elementu do wskaznika na nastepny element w nowym    */
/*      elemencie i adres nowego elementu do wskaznika na pierwszy      */
/*      element listy (szczyt stosu); zwraca wartosc 0, gdy zadzialala  */
/*      poprawnie lub wartosc BLAD, gdy alokacja pamieci sie nie udala  */
/************************************************************************/
int push(elem_listy **stos, int arg);

#endif
