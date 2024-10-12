/*
	Autor: Denys Malytskyi
 *  Předmět: Algoritmy (IAL) - FIT VUT v Brně
 *  Rozšíření pro příklad c206.c (Dvousměrně vázaný lineární seznam)
 *  Vytvořil: Daniel Dolejška, září 2024
 */

#include "c206-ext.h"

bool error_flag;
bool solved;

/**
 * Tato metoda simuluje příjem síťových paketů s určenou úrovní priority.
 * Přijaté pakety jsou zařazeny do odpovídajících front dle jejich priorit.
 * "Fronty" jsou v tomto cvičení reprezentovány dvousměrně vázanými seznamy
 * - ty totiž umožňují snazší úpravy pro již zařazené položky.
 * 
 * Parametr `packetLists` obsahuje jednotlivé seznamy paketů (`QosPacketListPtr`).
 * Pokud fronta s odpovídající prioritou neexistuje, tato metoda ji alokuje
 * a inicializuje. Za jejich korektní uvolnení odpovídá volající.
 * 
 * V případě, že by po zařazení paketu do seznamu počet prvků v cílovém seznamu
 * překročil stanovený MAX_PACKET_COUNT, dojde nejdříve k promazání položek seznamu.
 * V takovémto případě bude každá druhá položka ze seznamu zahozena nehledě
 * na její vlastní prioritu ovšem v pořadí přijetí.
 * 
 * @param packetLists Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param packet Ukazatel na strukturu přijatého paketu
 */
QosPacketListPtr find_list_for_prio(DLList *packetLists, PacketPtr packet){
	//In case packetLists is empty
	if(packetLists == NULL || packetLists->firstElement == NULL){
		return NULL;
	}

	DLLElementPtr curr = packetLists->firstElement;
	//If the current list priority is equal to packet's priority - we have a match
	while(curr != NULL){
		QosPacketListPtr currPacketList = (QosPacketListPtr)curr->data;
		if(currPacketList->priority == packet->priority){
			return currPacketList;
		}
		curr = curr->nextElement;
	}

	return NULL;
}

void receive_packet( DLList *packetLists, PacketPtr packet ) {
	//We'll first look if we have a list matching a packet priority
	QosPacketListPtr packetList = find_list_for_prio(packetLists, packet);
	//If we don't find one we create it
	if(packetList == NULL){
		packetList = (QosPacketListPtr)malloc(sizeof(QosPacketList));
		packetList->list = (DLList *)malloc(sizeof(DLList));
		packetList->priority = packet->priority;
		DLL_Init(packetList->list);
		DLL_InsertLast(packetLists, (long)packetList);
	}
	
	//Insert a packet into packetlist
	DLL_InsertLast(packetList->list, (long)packet);

	//In case we exceed the packet limit we delete every second packet from the list
	DLL_First(packetList->list);
	if(packetList->list->currentLength > MAX_PACKET_COUNT){
		while(packetList->list->activeElement != NULL){
			DLL_DeleteAfter(packetList->list);
			packetList->list->activeElement = packetList->list->activeElement->nextElement;
		}
	}
}



/**
 * Tato metoda simuluje výběr síťových paketů k odeslání. Výběr respektuje
 * relativní priority paketů mezi sebou, kde pakety s nejvyšší prioritou
 * jsou vždy odeslány nejdříve. Odesílání dále respektuje pořadí, ve kterém
 * byly pakety přijaty metodou `receive_packet`.
 * 
 * Odeslané pakety jsou ze zdrojového seznamu při odeslání odstraněny.
 * 
 * Parametr `packetLists` obsahuje ukazatele na jednotlivé seznamy paketů (`QosPacketListPtr`).
 * Parametr `outputPacketList` obsahuje ukazatele na odeslané pakety (`PacketPtr`).
 * 
 * @param packetLists Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param outputPacketList Ukazatel na seznam paketů k odeslání
 * @param maxPacketCount Maximální počet paketů k odeslání
 */
//Finding a total amount of packets to send by adding up the currentListLength of all packet lists
int find_total_packets(DLList *packetLists){
	if(packetLists == NULL || packetLists->firstElement == NULL){
		return 0;
	}
	DLLElementPtr currElement = packetLists->firstElement;
	int total = 0;
	while(currElement != NULL){
		QosPacketListPtr currPacketList = (QosPacketListPtr)currElement->data;
		total += currPacketList->list->currentLength;
		currElement = currElement->nextElement;
	}
	return total;
}
//Finding highest priority packet list by comparing priorities of all packet lists
QosPacketListPtr find_highest_prio(DLList *packetLists){
	if (packetLists == NULL || packetLists->firstElement == NULL) {

        return NULL;
    }

    DLLElementPtr currentElement = packetLists->firstElement;
    QosPacketListPtr highestPriorityList = (QosPacketListPtr)currentElement->data;


    while (currentElement != NULL) {
        QosPacketListPtr currentList = (QosPacketListPtr)currentElement->data;

        
        if (currentList->priority > highestPriorityList->priority) {
            highestPriorityList = currentList;
        }

       
        currentElement = currentElement->nextElement;
    }


    return highestPriorityList;
}


void send_packets(DLList *packetLists, DLList *outputPacketList, int maxPacketCount) {
    int packetsSent = 0;
    int totalPackets = find_total_packets(packetLists);
	//In case we find out that totalPackets amount exceeds the limit 
    if (totalPackets > maxPacketCount) {
        totalPackets = maxPacketCount;
    }
	//First we want to find the highest priority list
    QosPacketListPtr highestPrioPacketList = find_highest_prio(packetLists);
    while (packetsSent < totalPackets) {
        if (highestPrioPacketList == NULL) {
            break;  
        }
		//If we sent all packets from current list we nullify it's priority and look for a new list
        if (highestPrioPacketList->list->currentLength == 0) {
			highestPrioPacketList->priority = '\0';
            highestPrioPacketList = find_highest_prio(packetLists);
            continue;  
        }
		//Sending a packet
        PacketPtr packetToSend = (PacketPtr)highestPrioPacketList->list->firstElement->data;
        if (packetToSend == NULL) {
            break;
        }

        DLL_InsertLast(outputPacketList, (long)packetToSend);

        DLL_DeleteFirst(highestPrioPacketList->list);

        packetsSent++;
	
	}
}
