#include <iostream>
#include <vector>
#include <string>
#include <fstream>

// Абстрактний клас IFile
class IFile {
public:
    virtual ~IFile() = default;
    virtual void Save(std::ofstream& stream) const = 0;
    virtual void Save() const = 0;
    virtual void Load(std::ifstream& stream) = 0;
    virtual void Load() = 0;
};

// Клас Weapon
class Weapon : public IFile {
private:
    std::string name;
    int damage;

public:
    Weapon(const std::string& name = "", int damage = 0)
        : name(name), damage(damage) {}

    void display() const {
        std::cout << "Weapon: " << name << ", Damage: " << damage << "\n";
    }

    void Save(std::ofstream& stream) const override {
        size_t nameLength = name.size();
        stream.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
        stream.write(name.c_str(), nameLength);
        stream.write(reinterpret_cast<const char*>(&damage), sizeof(damage));
    }

    void Save() const override {
        std::ofstream file("weapon.bin", std::ios::binary);
        if (file) Save(file);
    }

    void Load(std::ifstream& stream) override {
        size_t nameLength;
        stream.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        name.resize(nameLength);
        stream.read(&name[0], nameLength);
        stream.read(reinterpret_cast<char*>(&damage), sizeof(damage));
    }

    void Load() override {
        std::ifstream file("weapon.bin", std::ios::binary);
        if (file) Load(file);
    }
};

// Клас Unit
class Unit : public IFile {
private:
    std::string name;
    Weapon weapon;

public:
    Unit(const std::string& name = "", const Weapon& weapon = Weapon())
        : name(name), weapon(weapon) {}

    void display() const {
        std::cout << "Unit: " << name << "\n";
        weapon.display();
    }

    void Save(std::ofstream& stream) const override {
        size_t nameLength = name.size();
        stream.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
        stream.write(name.c_str(), nameLength);
        weapon.Save(stream);
    }

    void Save() const override {
        std::ofstream file("unit.bin", std::ios::binary);
        if (file) Save(file);
    }

    void Load(std::ifstream& stream) override {
        size_t nameLength;
        stream.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        name.resize(nameLength);
        stream.read(&name[0], nameLength);
        weapon.Load(stream);
    }

    void Load() override {
        std::ifstream file("unit.bin", std::ios::binary);
        if (file) Load(file);
    }
};

int main() {
    // Створення об'єктів
    Weapon sword("Sword", 50);
    Unit warrior("Warrior", sword);

    // Збереження
    warrior.Save();

    // Завантаження
    Unit loadedWarrior;
    loadedWarrior.Load();

    loadedWarrior.display();

    return 0;
}
