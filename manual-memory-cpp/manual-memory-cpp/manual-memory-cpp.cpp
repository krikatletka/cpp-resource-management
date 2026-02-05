#include <iostream>
#include <cstring>   
#include <utility>   

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

struct TextEntity : Entity
{
    char* text = nullptr;

    TextEntity() = default;

    explicit TextEntity(const char* s){
        setText(s);
    }

    ~TextEntity() override{
        delete[] text;
        text = nullptr;
    }

    TextEntity(const TextEntity& other){
        setText(other.text);
    }

    TextEntity(TextEntity&& other) noexcept : text(other.text){
        other.text = nullptr;
    }

    TextEntity& operator=(TextEntity other) noexcept{
        swap(other);
        return *this;
    }

    void swap(TextEntity& other) noexcept{
        std::swap(text, other.text);
    }

    void setText(const char* s) {
        delete[] text;
        text = nullptr;

        if (!s) return;

        const size_t n = std::strlen(s);
        text = new char[n + 1];
        std::memcpy(text, s, n + 1); 
    }

    void print() const override{
        std::cout << "TextEntity: " << (text ? text : "(null)") << "\n";
    }

    Entity* clone() const override{
        return new TextEntity(*this); 
    }
};

int main()
{
    std::cout << "=== Polymorphism + clone ===\n";
    Entity* e1 = new TextEntity("Hello");
    e1->print();

    Entity* e2 = e1->clone();  
    e2->print();

    delete e1;
    delete e2;

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

    std::cout << "\n=== Copy assignment (copy-swap) ===\n";
    TextEntity x("X");
    TextEntity y("Y");
    y = x;

    x.setText("X changed");
    std::cout << "x: "; x.print();
    std::cout << "y: "; y.print(); 

}