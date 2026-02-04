#include <iostream>

struct Entity
{
    virtual ~Entity() = default;
    virtual void print() const = 0;
    virtual Entity* clone() const = 0;
};

struct NumberEntity : Entity
{
    int value;

    explicit NumberEntity(int v) : value(v) {}

    void print() const override
    {
        std::cout << "NumberEntity: " << value << "\n";
    }

    Entity* clone() const override
    {
        return new NumberEntity(*this);
    }
};

int main()
{
    Entity* a = new NumberEntity(42);
    a->print();

    Entity* b = a->clone();
    b->print();

    delete a;
    delete b;
}
