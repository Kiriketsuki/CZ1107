ListNode *traversal_node = ll->head;
    // ListNode *prev_node;
    // int current_int;
    // int index = 0;

    // if (ll->size == 0) {
    //     insertNode(ll, 0, item);
    //     return 0;
    // }

    // while (traversal_node->next != NULL) {
    //     current_int = traversal_node->item;

    //     if (current_int == item) {
    //         return -1;
    //     } 

    //     if (item > current_int) {
    //         prev_node = traversal_node;
    //         traversal_node =  traversal_node->next;
    //         index++;
    //     } else {
    //         if (item > prev_node->item) {
    //             insertNode(ll, index, item);
    //             return index;
    //         }
    //     }
    // }