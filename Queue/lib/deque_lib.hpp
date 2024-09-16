#include <cstdlib>
#include <initializer_list>

constexpr size_t INIT_AMOUNT_OF_TABS = 4;
constexpr size_t START_FACTOR = 4;

template <typename T>
class Deque
{
    size_t tabs_capacity = INIT_AMOUNT_OF_TABS;
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
    unsigned factor = START_FACTOR; // staring point of the tab size. Next tabs will have double size of it

    void copy_data(T *from, T *to, size_t capacity)
    {
    }

    void initialize_tabs(size_t size)
    {
        pointer_to_tabs = new T *[size];
        capacity_of_tabs = new size_t[size];
        size_of_tabs = new size_t[size];
        order_of_tabs = new unsigned[size];
    }

    void set_fabric_values()
    {
        tabs_capacity = INIT_AMOUNT_OF_TABS;
        tabs_quantity = 1;
        next_order_num = 0;
        front_table_idx = 0;
        last_table_idx = 0;
        front_ = 1;
        back_ = 0;
        factor = START_FACTOR;
    }

    void initialize()
    {
        initialize_tabs(tabs_capacity);
        unsigned first_tab = 0;
        capacity_of_tabs[first_tab] = factor;
        size_of_tabs[first_tab] = 0;
        order_of_tabs[first_tab] = next_order_num++;
        pointer_to_tabs[first_tab] = new T[capacity_of_tabs[first_tab]];
    }

    void dealocate()
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

public:
    Deque()
    {
        initialize();
    }

    Deque(unsigned quantity, T value)
    {
        initialize();
        for (int i = 0; i < quantity; i++)
        {
            push_back(value);
        }
    }

    Deque(std::initializer_list<T> data)
    {
        initialize();
        for (auto el : data)
        {
            push_back(el);
        }
    }

    ~Deque()
    {
        dealocate();
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

    Deque &operator=(const Deque &other)
    {
        dealocate();
        tabs_capacity = other.tabs_capacity;
        tabs_quantity = other.tabs_quantity;
        next_order_num = other.next_order_num;
        front_table_idx = other.front_table_idx;
        last_table_idx = other.last_table_idx;
        front_ = other.front_;
        back_ = other.back_;
        factor = other.factor;

        initialize_tabs(tabs_capacity);

        for (int i = 0; i < tabs_quantity; i++)
        {
            capacity_of_tabs[i] = other.capacity_of_tabs[i];
            size_of_tabs[i] = other.size_of_tabs[i];
            order_of_tabs[i] = other.order_of_tabs[i];
            pointer_to_tabs[i] = new T[capacity_of_tabs[i]];
            for (int j = 0; j < capacity_of_tabs[i]; j++)
            {
                pointer_to_tabs[i][j] = other.pointer_to_tabs[i][j];
            }
        }
        return *this;
    }

    void assign(std::initializer_list<T> list)
    {
        dealocate();
        set_fabric_values();
        initialize();
        for(auto& el: list)
        {
            push_back(el);
        }
    }

    void assign(unsigned quantity, T value)
    {
        dealocate();
        set_fabric_values();
        initialize();
        for (int i = 0; i < quantity; i++)
        {
            push_back(value);
        }
    }

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
