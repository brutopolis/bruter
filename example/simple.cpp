#include <bruter.h>

int main()
{
    Bruter *list = new Bruter(8, true, true);
    BruterInt i = 42;
    list->unshift(3.14); // Unshift a float value
    list->push(i); // Push an integer value
    list->insert(0, (BruterUInt)100); // Insert an unsigned integer value at index 0
    list->insert(1, (void*)0x1234); // Insert a pointer value at index 1
    list->insert(2, [](BruterList *context, BruterList *args) -> BruterInt {
        // Example function that returns the size of the list
        return context->size;
    }); // Insert a function pointer at index 2

    
    BruterInt v = list->pop();
    
    // free the list
    printf("List size: %ld\n", list->size());
    printf("List capacity: %ld\n", list->capacity());
    printf("Popped value: %ld\n", v);
    printf("lambda result: %ld\n", ((BruterFunction&)list[2])(list->get_list(), NULL));
    delete list; // Use delete to free the Bruter object
    return 0;
}