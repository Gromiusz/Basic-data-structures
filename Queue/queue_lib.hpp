#include <cstdlib>

template <typename T>
class Deque
{
    T* tab;
    size_t capacity = 3;
    size_t size = 0;
    size_t front = 1;
    size_t back = 1;

    void copy_data(T* from, T* to, size_t capacity) {
        
    }

public:
    Deque() : {
        tab = new T[3];
    }
    ~Deque(){
        delete[] tab;
    }

    // Deque operator=(Deque other)
    // {

    // }
    void push_back(T value)
    {
        if(front<capacity)
        {
            tab[back++] = value;
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
