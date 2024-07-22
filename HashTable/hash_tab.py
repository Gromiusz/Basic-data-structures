class HashTable:
    def __init__(self, size):
        self.size = size
        self.table = [[] for _ in range(size)]

    def hash_function(self, key):
        return hash(key) % self.size

    def insert(self, key, value):
        index = self.hash_function(key)
        # Check if the key already exists and update value
        for kvp in self.table[index]:
            if kvp[0] == key:
                kvp[1] = value
                print(f"Updated: ({key}, {value})")
                return
        # If the key does not exist, append new (key, value)
        self.table[index].append([key, value])
        print(f"Inserted: ({key}, {value})")

    def delete(self, key):
        index = self.hash_function(key)
        for i, kvp in enumerate(self.table[index]):
            if kvp[0] == key:
                del self.table[index][i]
                print(f"Deleted: {key}")
                return
        print(f"Key not found: {key}")

    def search(self, key):
        index = self.hash_function(key)
        for kvp in self.table[index]:
            if kvp[0] == key:
                print(f"Found: ({key}, {kvp[1]})")
                return kvp[1]
        print(f"Key not found: {key}")
        return None

    def display(self):
        print("Current Hash Table:")
        for i, bucket in enumerate(self.table):
            if bucket:
                print(f"Index {i}: {bucket}")
            else:
                print(f"Index {i}: Empty")

# Example usage
if __name__ == "__main__":
    ht = HashTable(10)  # Create a hash table with 10 buckets
    ht.insert("apple", 1)
    ht.insert("banana", 2)
    for i in range(100):
        ht.insert(str(i), i)
    ht.insert("cherry", 3)

    ht.display()

    ht.search("banana")
    ht.delete("banana")
    ht.search("banana")

    ht.display()
