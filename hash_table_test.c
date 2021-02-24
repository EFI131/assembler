#include "hash_table.h"

int main(int argc, char *argv[]){
    char labels[][MAX_LABEL_LENGTH + 1]  = {"adam", "barbara", "christine", "david", "eduard", "frank", "george", "hanna", "ignacio"};
    char search_labels[][MAX_LABEL_LENGTH + 1]  = {"adam", "bongo", "christine", "donny", "eduard", "fredrik", "george", "haim", "ignacio"};
    struct DataItem* itemP;
    int i;
    for(i = 0; i < sizeof(labels)/(MAX_LABEL_LENGTH+1); i++){
        printf(( hashTableInsert(labels[i])
				?"%s inserted into table\n" 
				:"%s wasn't inserted into table\n"), labels[i]);
    }


    for(i = 0; i < sizeof(labels)/(MAX_LABEL_LENGTH+1); i++){
        printf(( hashTableInsert(labels[i])
				?"%s inserted into table\n" 
				:"%s wasn't inserted into table\n"), labels[i]);
    }

    for(i = 0; i < sizeof(search_labels)/(MAX_LABEL_LENGTH+1); i++){
        printf(( hashTableSearch(search_labels[i]) 
				?"label %s was found\n"
				:"label %s was not found\n"), search_labels[i]);
    }
    
}
