#include <cstdlib>
#include <initializer_list>

constexpr size_t INIT_AMOUNT_OF_TABS = 4;

template <typename T>
class Deque
{
    size_t tabs_quantity = 1;
    // T* front_tab
    T **pointer_to_tabs; // contains pointers to chunks
    size_t *capacity_of_tabs;
    size_t *size_of_tabs;

    unsigned *order_of_tabs; // which tab is first, second, ... and last
    unsigned next_order_num = 0;

    // T* end_tab;
    unsigned front_table_idx = 0; // idx to poiner_to_tabs
    unsigned last_table_idx = 0;

    size_t front_ = 1;
    size_t back_ = 0;
    unsigned factor = 4; // staring point of the tab size. Next tabs will have double size of it

    void copy_data(T *from, T *to, size_t capacity)
    {
    }

    void initialize_tabs()
    {
        pointer_to_tabs = new T *[INIT_AMOUNT_OF_TABS];
        capacity_of_tabs = new size_t[INIT_AMOUNT_OF_TABS];
        size_of_tabs = new size_t[INIT_AMOUNT_OF_TABS];
        order_of_tabs = new unsigned[INIT_AMOUNT_OF_TABS];

        unsigned first_tab = 0;
        capacity_of_tabs[first_tab] = factor;
        size_of_tabs[first_tab] = 0;
        order_of_tabs[first_tab] = next_order_num++;
        pointer_to_tabs[first_tab] = new T[capacity_of_tabs[first_tab]];
    }

public:
    Deque()
    {
        initialize_tabs();
    }

    Deque(unsigned quantity, T value)
    {
        initialize_tabs();
        for(int i = 0; i < quantity; i++)
        {
            push_back(value);
        }
    }

    Deque(std::initializer_list<T> data)
    {
        initialize_tabs();
        for(auto el: data)
        {
            push_back(el);        
        }
    }

    ~Deque()
    {
        for (int i = 0; i < tabs_quantity; i++)
        {
            delete[] pointer_to_tabs[i];
        }
        delete[] pointer_to_tabs;
        delete[] capacity_of_tabs;
        delete[] size_of_tabs;
        delete[] order_of_tabs;
    }

    size_t size()
    {
        size_t size = 0;
        for (int i = 0; i < tabs_quantity; i++)
        {
            size += size_of_tabs[i];
        }
        return size;
    }

    T front()
    {
        return pointer_to_tabs[front_table_idx][front_];
    }

    T back()
    {
        return pointer_to_tabs[last_table_idx][back_];
    }

    bool empty()
    {
        return size() == 0;
    }

    // Deque operator=(Deque other)
    // {

    // }
    void push_back(T value)
    {
        if (back_ < capacity_of_tabs[last_table_idx] - 1) // checking if there is still space in tab
        {
            pointer_to_tabs[last_table_idx][++back_] = value;
            size_of_tabs[last_table_idx]++;
        }
        else
        {
            /* Alocating new tab */
            tabs_quantity++;
            factor *= 2; // increasing the next tab size;
            pointer_to_tabs[++last_table_idx] = new T[factor];
            order_of_tabs[last_table_idx] = ++next_order_num;
            capacity_of_tabs[last_table_idx] = factor;
            size_of_tabs[last_table_idx] = 1;

            back_ = 0;
            pointer_to_tabs[last_table_idx][back_] = value;
        }
    }
};
