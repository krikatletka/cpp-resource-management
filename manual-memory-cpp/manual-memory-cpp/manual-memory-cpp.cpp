#include <iostream>
#include <cstring>
#include <utility>
#include <memory>

struct Entity
{
    virtual ~Entity() = default;
    virtual void print() const = 0;
    virtual std::unique_ptr<Entity> clone() const = 0;
};

struct NumberEntity : Entity
{
    int value;

    explicit NumberEntity(int v) : value(v) {}

    void print() const override
    {
        std::cout << "NumberEntity: " << value << "\n";
    }

    std::unique_ptr<Entity> clone() const override
    {
        return std::make_unique<NumberEntity>(*this);
    }
};

struct TextEntity : Entity
{
    char* text = nullptr;

    TextEntity() = default;

    explicit TextEntity(const char* s)
    {
        setText(s);
    }

    ~TextEntity() override
    {
        delete[] text;
        text = nullptr;
    }

    TextEntity(const TextEntity& other)
    {
        setText(other.text);
    }

    TextEntity(TextEntity&& other) noexcept
        : text(other.text)
    {
        other.text = nullptr;
    }

    TextEntity& operator=(const TextEntity& other)
    {
        if (this == &other) return *this;
        TextEntity tmp(other);  
        swap(*this, tmp);        
        return *this;
    }

  
    TextEntity& operator=(TextEntity&& other) noexcept
    {
        if (this == &other) return *this;

        delete[] text;           
        text = other.text;      
        other.text = nullptr;
        return *this;
    }

    friend void swap(TextEntity& a, TextEntity& b) noexcept
    {
        std::swap(a.text, b.text);
    }

    void setText(const char* s)
    {
        delete[] text;
        text = nullptr;

        if (!s) return;

        const std::size_t n = std::strlen(s);
        text = new char[n + 1];
        std::memcpy(text, s, n + 1);
    }

    void print() const override
    {
        std::cout << "TextEntity: " << (text ? text : "(null)") << "\n";
    }

    std::unique_ptr<Entity> clone() const override
    {
        return std::make_unique<TextEntity>(*this);
    }
};

int main()
{
    std::cout << "=== Polymorphism + clone ===\n";
    std::unique_ptr<Entity> e1 = std::make_unique<TextEntity>("Hello");
    e1->print();

    auto e2 = e1->clone();
    e2->print();

    std::cout << "\n=== Copy vs Move on TextEntity ===\n";
    TextEntity a("First");
    TextEntity b = a;        
    a.setText("Changed");

    std::cout << "a: "; a.print();
    std::cout << "b: "; b.print();

    TextEntity c("MoveMe");
    TextEntity d = std::move(c);   

    std::cout << "c(after move): "; c.print();
    std::cout << "d: "; d.print();

    std::cout << "\n=== Copy assignment ===\n";
    TextEntity x("X");
    TextEntity y("Y");
    y = x;  


    x.setText("X changed");
    std::cout << "x: "; x.print();
    std::cout << "y: "; y.print();

    std::cout << "\n=== Move assignment ===\n";
    TextEntity m("M");
    TextEntity n("N");
    n = std::move(m);

    std::cout << "m(after move assign): "; m.print();
    std::cout << "n: "; n.print();

}
