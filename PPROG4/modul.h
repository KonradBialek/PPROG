#ifndef MODUL_H
#define MODUL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define DL_LINII 1024      /* Dlugosc buforow pomocniczych */

/* strukura do zapamietywania opcji podanych w wywolaniu programu */
typedef struct {
  FILE *plik_we, *plik_wy;        /* uchwyty do pliku wej. i wyj. */
  int negatyw,progowanie,konturowanie,wyswietlenie,kolor,odczytanie;/* opcje */
  int w_progu;              /* wartosc progu dla opcji progowanie */ 
} w_opcje;

typedef struct { /* struktura zawierajaca obraz i informacje potrzebne do przetwarzania i zapisu */
  void *piksele;
  int wymx, wymy, odcieni; /* wymiary i odcienie */
  char modyfikacje[100];   /* komentarz modyfikacji */
  char *nazwa, *nowy_plik; /* nazwy plikow wejsciowego i wyjsciowego potrzebne do zapisu*/
  int typ; /* typ obrazu */
} t_obraz;

/************************************************************************************/
/* Funkcja wczytuje obraz PGM lub PPM z pliku do struktury obrazek                  */
/* Argumenty funkcji:                                                               */
/*         plik_we -  uchwyt do pliku z obrazem w formacie PGM lub PPM              */
/*         obrazek -  struktura z informacjami o obrazie                            */
/* PRE:                                                                             */
/*      Uchwyt plik_we wskazuje na istniejacy plik w formacie PGM lub PPM           */
/*      zawierajacy wszystkie parametry obrazu: typ (numer magiczny), wymx, wymy i  */
/*      odcieni (szarosci lub kazdego koloru, gdy obraz jest kolrowy) i caly obraz  */
/* POST:                                                                            */
/*      w strukturze obrazek wymx i wymy przyjmuja wartosci wymiarow obrazu,        */
/*      odcieni przyjmuje liczbe odcieni koloru obrazu, do piksele zostaje          */
/*      zapisany obraz z pliku, typ okresla rodzaj obrazu (kolorowy/czarno-bialy);  */
/*      zwraca liczbe pikseli obrazu, gdy wywolanie bylo poprawne lub 0, gdy        */
/*      nastapil blad                                                               */
/************************************************************************************/
int czytaj(FILE *plik_we, t_obraz *obrazek);

/************************************************************************************/
/* Funkcja dokonuje modyfikacji negatywu na obrazie w strukturze                    */
/* Argumenty funkcji:                                                               */
/*         obrazek   -  struktura z informacjami o obrazie                          */
/*         odczytano -  informacja czy wczytano obraz i jego parametry              */
/* PRE:                                                                             */
/*      poprawnie odczytano plik (pobrano obraz i jego parametry)                   */
/* POST:                                                                            */
/*      zmiana obrazu w strukturze na jego negatyw i dopisanie modyfikacji negatywu */
/*      do napisu modyfikacje; zwraca liczbe pikseli obrazu                         */
/************************************************************************************/
int negatyw(t_obraz *obrazek, int *odczytano);

/************************************************************************************/
/* Funkcja dokonuje modyfikacji progowania na obrazie w stukturze                   */
/* Argumenty funkcji:                                                               */
/*         obrazek   -  struktura z informacjami o obrazie                          */
/*         odczytano -  informacja czy wczytano obraz i jego parametry              */
/*         prog      -  wartosc progu potrzebna do modyfikacji                      */
/* PRE:                                                                             */
/*      poprawnie odczytano plik (pobrano obraz i jego parametry) i wartosc progu   */
/*      jest poprawna                                                               */
/* POST:                                                                            */
/*      zmiana obrazu w strukturze na obraz przeksztalcony przez progowanie i       */
/*      dopisanie modyfikacji progowania do napisu modyfikacje; zwraca liczbe       */
/*      pikseli obrazu                                                              */
/************************************************************************************/
int progowanie(t_obraz *obrazek, int *odczytano, int prog);

/************************************************************************************/
/* Funkcja dokonuje modyfikacji konturowania na obrazie PGM w tablicy               */
/* Argumenty funkcji:                                                               */
/*         obrazek   -  struktura z informacjami o obrazie                          */
/*         odczytano -  informacja czy wczytano obraz i jego parametry              */
/* PRE:                                                                             */
/*      poprawnie odczytano plik (pobrano obraz i jego parametry)                   */
/* POST:                                                                            */
/*      zmiana obrazu w strukturze na obraz przeksztalcony przez konturowanie,      */
/*      podwojenie wartosci parametru odcieni obrazu i dopisanie modyfikacji        */
/*      konturowania do napisu modyfikacje; zwraca liczbe pikseli obrazu            */
/************************************************************************************/
int konturowanie(t_obraz *obrazek, int *odczytano);

/************************************************************************************/
/* Funkcja zapisuje obraz PGM z tablicy do pliku                                    */
/* Argumenty funkcji:                                                               */
/*         obrazek   -  struktura z informacjami o obrazie                          */
/*         odczytano -  informacja czy wczytano obraz i jego parametry              */
/*         nowy      -  uchwyt do pliku, w ktorym obraz zostanie zapisany           */
/* PRE:                                                                             */
/*      poprawnie odczytano plik (pobrano obraz i jego parametry - w tym nazwe      */
/*      oryginalnego i nowego pliku)                                                */
/* POST:                                                                            */
/*      zapisanie obrazu ze struktury razem z jego parametrami i komentarzami       */
/*      zawierajacymi nazwe oryginalnego pliku i tego pliku oraz wykonanymi         */
/*      modyfikacjami do nowego pliku o podanej nazwie; zwraca liczbe pikseli       */
/*      obrazu                                                                      */
/************************************************************************************/
int zapisz(t_obraz *obrazek, int *odczytano, FILE *nowy);

/************************************************************************************/
/* Funkcja konwertuje obraz kolorowy do czarno-bialego                              */
/* Argumenty funkcji:                                                               */
/*         obrazek   -  struktura z informacjami o obrazie                          */
/*         odczytano -  informacja czy wczytano obraz i jego parametry              */
/* PRE:                                                                             */
/*      poprawnie odczytano plik (pobrano obraz i jego parametry)                   */
/* POST:                                                                            */
/*      zmiana obrazu w strukturze na obraz przeksztalcony przez konwersje i        */ 
/*      dopisanie modyfikacji konwersji do szarosci do napisu modyfikacje; zwraca   */
/*      liczbe pikseli obrazu                                                       */
/************************************************************************************/
int konwersja(t_obraz *obrazek, int *odczytano);

#endif
