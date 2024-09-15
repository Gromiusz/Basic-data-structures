#include <cstdlib>
#include <initializer_list>

constexpr size_t INIT_AMOUNT_OF_TABS = 4;

template <typename T>
class Deque
{
    size_t tabs_quantity;
    //T* front_tab
    T** pointer_to_tabs; // contains pointers to chunks
    size_t* capacity_of_tabs;
    size_t* size_of_tabs;


    unsigned* order_of_tabs;
    unsigned next_order_num = 0;

    //T* end_tab;
    unsigned front_table_idx = 0; // idx to poiner_to_tabs
    unsigned last_table_idx = 0;
    size_t front = 1;
    size_t back = 1;

    void copy_data(T* from, T* to, size_t capacity) {
        
    }

    void initialize_tabs()
    {
        tabs_quantity = INIT_AMOUNT_OF_TABS;
        pointer_to_tabs = new T*[tabs_quantity];
        capacity_of_tabs = new size_t[tabs_quantity];
        size_of_tabs = new size_t[tabs_quantity];
        order_of_tabs = new unsigned[tabs_quantity];

        unsigned first_tab = 0;
        capacity_of_tabs[first_tab] = 4;
        size_of_tabs[first_tab] = 0;
        order_of_tabs[first_tab] = next_order_num++;
        pointer_to_tabs[first_tab] = new T[capacity_of_tabs[first_tab]];
    }

public:
    Deque() : {
        initialize_tabs();
    }

    Deque(std::initializer_list<T> data) : {
        initialize_tabs();

    }

    ~Deque(){
        for(int i=0; i<tabs_quantity; i++)
        {
            delete[] pointer_to_tabs[i];
        }
        delete[] pointer_to_tabs;
        delete[] capacity_of_tabs;
        delete[] size_of_tabs;
        delete[] order_of_tabs;
    }

    // Deque operator=(Deque other)
    // {

    // }
    void push_back(T value)
    {
        if(back<capacity_of_tabs[last_table_idx]-1)
        {
            tab[++back] = value;
            size++;
        }
        else{
            T* aux_tab[capacity];
            for(int i=0; i<capacity; i++) {
                aux_tab[i] = tab[i];
            }
            delete[] tab;
            capacity *= 2;
            tab = new T[capacity];

        }
    }

};
