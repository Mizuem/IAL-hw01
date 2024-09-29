/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/*           Daniel Dolejška, září 2022                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
** uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

bool error_flag;
bool solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error(void) {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = true;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {
	list->firstElement = NULL;
	list->lastElement = NULL;
	list->activeElement = NULL;
	list->currentLength = 0;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {
	DLLElementPtr tmp;
	while(list->firstElement != NULL){
		tmp = list->firstElement; //set the temp pointer to point to the same plase as first element
		list->firstElement=list->firstElement->nextElement; //move first element pointer to the next element
		free(tmp); 
		
	}
	list->firstElement = NULL;
    list->lastElement = NULL;
    list->activeElement = NULL;
    list->currentLength = 0;
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, long data ) {
	if(list == NULL){
		DLL_Error();
		return;
	}
	
	DLLElementPtr new = (DLLElementPtr)malloc(sizeof(struct DLLElement));
	if(new == NULL){
		DLL_Error();
		return;
	}
	new->data = data;
	new->nextElement = list->firstElement; //right pointer points to the old first element
	new->previousElement = NULL; //left pointer points to null

	if(list->firstElement != NULL){//case of the first element already being in the list
		list->firstElement->previousElement = new; //left pointer of old first element is going to point to the new one
	}
	else{ //inserting into empty list
		list->lastElement = new;
	}
	list->firstElement = new; //first element pointer correction
	
	list->currentLength++;
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, long data ) {
	//the same thing but right and left, first and last swapped
	if(list == NULL){
		DLL_Error();
		return;
	}
	
	DLLElementPtr new = (DLLElementPtr)malloc(sizeof(struct DLLElement));
	if(new == NULL){
		DLL_Error();
		return;
	}
	new->data = data;
    new->nextElement = NULL;
	new->previousElement = list->lastElement;

	if(list->lastElement != NULL){
		list->lastElement->nextElement = new;
	}
	else{
		list->firstElement = new;
	}
	list->lastElement = new;
	list->currentLength++;
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {
	list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {
	list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, long *dataPtr ) {
	if(list->currentLength == 0){ // if the list is empty we get an error
		DLL_Error();
		return;
	}
	int tmp = list->firstElement->data;
	*dataPtr = tmp;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, long *dataPtr ) {
	if(list->currentLength == 0){ //if the list is empty we get an error
		DLL_Error();
		return;
	}
	int tmp = list->lastElement->data;
	*dataPtr = tmp;
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {
	DLLElementPtr tmp;
	if (list->firstElement != NULL){
		tmp = list->firstElement;
		if (list->activeElement == list->firstElement){ //the first element was active
			list->activeElement = NULL; //list activity is being lost
		}
		if (list->firstElement == list->lastElement){//case of list having a single element;
			list->lastElement = NULL;
			list->firstElement = NULL;
			list->currentLength--;
		}
		else {
			list->firstElement = list->firstElement->nextElement; //update list's first element
			list->firstElement->previousElement = NULL; //left pointer of the first element points to null
			list->currentLength--;
			free(tmp);
		}
	}
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {
	//the same thing but right and left, first and last swapped
	DLLElementPtr tmp;
	if (list->lastElement != NULL){
		tmp = list->lastElement;
		if (list->activeElement == list->lastElement){ 
			list->activeElement = NULL; 
		}
		if (list->firstElement == list->lastElement){
			list->lastElement = NULL;
			list->firstElement =NULL;
			list->currentLength--;
		}
		else {
			list->lastElement = list->lastElement->previousElement; 
			list->lastElement->nextElement = NULL; 
			list->currentLength--;
			free(tmp);
		}
	}
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {
	if(list->activeElement != NULL){ //check if there is something to delete
		if(list->activeElement->nextElement != NULL){
			DLLElementPtr tmp;
			tmp = list->activeElement->nextElement; //set the tmp pointer to the element we're going to delete
			list->activeElement->nextElement = tmp->nextElement; //jumping over tmp element
			if(tmp == list->lastElement){ //case of tmp being the element in the list
				list->lastElement = list->activeElement; //the active element is going to be last
			}
			else{ //element after tmp points to the left on the active element
				tmp->nextElement->previousElement = list->activeElement;
			}
			free(tmp);
			list->currentLength--;
		}
	}

}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
	//the same thing but right and left, first and last swapped
	if(list->activeElement != NULL){
		if(list->activeElement->previousElement != NULL){
			DLLElementPtr tmp;
			tmp = list->activeElement->previousElement;
			list->activeElement->previousElement = tmp->previousElement;
			if(tmp == list->firstElement){
				list->firstElement = list->activeElement;
			}
			else{
				tmp->previousElement->nextElement = list->activeElement;
			}
			free(tmp);
			list->currentLength--;
		}
	}
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, long data ) {
	if(list->activeElement != NULL){ //check if list is active
		DLLElementPtr new = (DLLElementPtr)malloc(sizeof(struct DLLElement));
		if(new == NULL){
			DLL_Error();
			return;
		}
		new->data = data;
		new->nextElement = list->activeElement->nextElement;
		new->previousElement = list->activeElement;
		list->activeElement->nextElement = new; //connect left neighbor element to the new one
		if(list->activeElement == list->lastElement){ //insert after the last element
			list->lastElement = new; //pointer correction to the end of list
		}
		else{ //connect right neighbor element to the inserted element
			new->nextElement->previousElement = new;
		}
		list->currentLength++;
	}
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, long data ) {
	//the same thing but right and left, first and last swapped
	if(list->activeElement != NULL){
		DLLElementPtr new = (DLLElementPtr)malloc(sizeof(struct DLLElement));
		if(new == NULL){
			DLL_Error();
			return;
		}
		new->data = data;
		new->previousElement = list->activeElement->previousElement;
		new->nextElement = list->activeElement;
		list->activeElement->previousElement = new;
		if(list->activeElement == list->firstElement){
			list->firstElement = new;
		}
		else{
			new->previousElement->nextElement = new;
		}
		list->currentLength++;
	}
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, long *dataPtr ) {
	if(list->activeElement != NULL){ //if the list is not active we get an error
		int tmp = list->activeElement->data;
		*dataPtr = tmp;
	}
	else{
		DLL_Error();
	}
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, long data ) {
	if(list->activeElement != NULL){ //if the list is not active nothing happens
		list->activeElement->data = data;
	}
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
	if(list->activeElement != NULL){//if the list is not active nothing happens
		if(list->activeElement->nextElement != NULL){//if active element is not the last one we can move it further
			list->activeElement = list->activeElement->nextElement;
		}
		else{
			list->activeElement = NULL;
		}
	}
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {
	if(list->activeElement != NULL){//if the list is not active nothing happens
		if(list->activeElement->previousElement != NULL){//if active element is not the first one we can move it back
			list->activeElement = list->activeElement->previousElement;
		}
		else{
			list->activeElement = NULL;
		}
	}
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
bool DLL_IsActive( DLList *list ) {
	return list->activeElement != NULL;
}

/* Konec c206.c */
