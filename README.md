# Project: IFJ23

```c
/***************************************************************
* **Project Name:** Implementace překladače imperativního jazyka IFJ23
* **File Name:** [Add File Name Here]
* **Description:** [Add Description Here]
* **Author:** JOE MAMA (xjmama69)
* **Faculty:** FIT VUT
* **Date:** 29.09.2023
*
* **Comments:** [Add Comments Here]
***************************************************************/
```
---

## Structure
- `./tests` - Unit tests
- `Makefile`
- `README.md`
- `*.c` - Source code files
- `*.h` - Header files

--- 

## Header Files (*.h) - Hlavičkové soubory
- Na vrchu hlavička
- Deklarace funkcí korespondujících zdrojovým souborům *.c
- K každé funkci Doxygen komentáře
- #### Doxygen komentáře
```c
/*HLAVICKA*/

#ifndef HEADER_FILENAME_H
#define HEADER_FILENAME_H

...

/**
 * @brief Stručný popis funkce
 *
 * Širší popis funkce
 *
 * @param a První parametr
 * @param b Druhý parametr
 * @return Návratová hodnota
 */
int sum(int a, int b);

...

#endif // HEADER_FILENAME_H
```


## Unit Testing
Pro instalaci CUnit použijte následující příkaz:

```
sudo apt-get install libcunit1 libcunit1-dev
```




