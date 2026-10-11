// ============================================================
//  Smart Farm Management System
//  Concepts: classes, inheritance, constructor overloading,
//  virtual destructor, runtime polymorphism, operator
//  overloading, abstract class, file handling, static
//  members, I/O formatting, exception handling.
// ============================================================
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <memory>
#include <string>
#include <stdexcept>
#include <limits>

using namespace std;

// ------------------------------------------------------------
// Abstract class: Advisory  (Requirement 6)
// ------------------------------------------------------------
class Advisory {
public:
    virtual void giveAdvice() const = 0;   // pure virtual function
    virtual ~Advisory() {}
};

// ------------------------------------------------------------
// Base class: Crop  (Requirements 1, 3, 4, 5, 8)
// ------------------------------------------------------------
class Crop {
private:
    static int cropCount;                  // static member (Req 8)

protected:                                 // accessible to derived classes
    string name;
    double area;                           // in acres
    string season;
    double yieldPerAcre;                   // in quintals per acre

    // Validation shared by all constructors / setters
    static void validate(double a, double y) {
        if (a <= 0)
            throw invalid_argument("Area must be greater than 0.");
        if (y <= 0)
            throw invalid_argument("Yield per acre must be greater than 0.");
    }

public:
    // ---- Constructor overloading (Req 3) ----
    Crop() : name("Unknown"), area(1.0), season("Unknown"), yieldPerAcre(1.0) {
        ++cropCount;
    }

    Crop(const string& n, double a) : name(n), area(a), season("Unknown"), yieldPerAcre(1.0) {
        validate(a, yieldPerAcre);
        ++cropCount;
    }

    Crop(const string& n, double a, const string& s, double y)
        : name(n), area(a), season(s), yieldPerAcre(y) {
        validate(a, y);
        ++cropCount;
    }

    Crop(const Crop& other)
        : name(other.name), area(other.area), season(other.season),
          yieldPerAcre(other.yieldPerAcre) {
        ++cropCount;
    }

    // ---- Virtual destructor (Req 3) ----
    virtual ~Crop() { --cropCount; }

    // ---- Getters ----
    string getName() const { return name; }
    double getArea() const { return area; }
    string getSeason() const { return season; }
    double getYieldPerAcre() const { return yieldPerAcre; }

    // ---- Static function ----
    static int getCropCount() { return cropCount; }

    // ---- Runtime polymorphism (Req 4) ----
    virtual double totalYield() const { return area * yieldPerAcre; }

    virtual void display() const {
        cout << left << setw(18) << "Crop Name"   << ": " << name << "\n"
             << left << setw(18) << "Area (acres)" << ": " << fixed << setprecision(2) << area << "\n"
             << left << setw(18) << "Season"       << ": " << season << "\n"
             << left << setw(18) << "Yield/Acre"   << ": " << fixed << setprecision(2) << yieldPerAcre << " qtl\n";
    }

    virtual string typeName() const { return "Crop"; }

    // Serialization for file handling (Req 7)
    virtual void save(ostream& out) const {
        out << typeName() << "|" << name << "|" << area << "|"
            << season << "|" << yieldPerAcre;
    }

    // ---- Operator overloading: compare crops by yield (Req 5) ----
    bool operator>(const Crop& other)  const { return totalYield() >  other.totalYield(); }
    bool operator<(const Crop& other)  const { return totalYield() <  other.totalYield(); }
    bool operator==(const Crop& other) const { return totalYield() == other.totalYield(); }

    friend ostream& operator<<(ostream& os, const Crop& c) {
        os << c.name << " (" << fixed << setprecision(2) << c.totalYield() << " qtl)";
        return os;
    }
};

int Crop::cropCount = 0;                   // static member definition

// ------------------------------------------------------------
// Derived class: CerealCrop  (Requirements 2, 6)
// ------------------------------------------------------------
class CerealCrop : public Crop, public Advisory {
private:
    string fertilizerType;
    int irrigationFrequency;               // times per month

public:
    CerealCrop() : Crop(), fertilizerType("Urea"), irrigationFrequency(2) {}

    CerealCrop(const string& n, double a, const string& s, double y,
               const string& fert, int irr)
        : Crop(n, a, s, y), fertilizerType(fert), irrigationFrequency(irr) {
        if (irr < 0) throw invalid_argument("Irrigation frequency cannot be negative.");
    }

    ~CerealCrop() override {}

    // Cereals: yield with a small straw/by-product bonus (shows overriding)
    double totalYield() const override { return area * yieldPerAcre; }

    void display() const override {
        cout << "--- Cereal Crop ---\n";
        Crop::display();
        cout << left << setw(18) << "Fertilizer"      << ": " << fertilizerType << "\n"
             << left << setw(18) << "Irrigation/month" << ": " << irrigationFrequency << "\n"
             << left << setw(18) << "Total Yield"     << ": " << fixed << setprecision(2)
             << totalYield() << " qtl\n";
    }

    string typeName() const override { return "Cereal"; }

    void save(ostream& out) const override {
        Crop::save(out);
        out << "|" << fertilizerType << "|" << irrigationFrequency;
    }

    void giveAdvice() const override {
        cout << "[Advice - " << name << "] Use certified seeds, apply "
             << fertilizerType << " in split doses, irrigate about "
             << irrigationFrequency << " times a month, and watch for rust and blight.\n";
    }
};

// ------------------------------------------------------------
// Derived class: VegetableCrop  (Requirements 2, 6)
// ------------------------------------------------------------
class VegetableCrop : public Crop, public Advisory {
private:
    string fertilizerType;
    int irrigationFrequency;               // times per week

public:
    VegetableCrop() : Crop(), fertilizerType("Compost"), irrigationFrequency(3) {}

    VegetableCrop(const string& n, double a, const string& s, double y,
                  const string& fert, int irr)
        : Crop(n, a, s, y), fertilizerType(fert), irrigationFrequency(irr) {
        if (irr < 0) throw invalid_argument("Irrigation frequency cannot be negative.");
    }

    ~VegetableCrop() override {}

    void display() const override {
        cout << "--- Vegetable Crop ---\n";
        Crop::display();
        cout << left << setw(18) << "Fertilizer"     << ": " << fertilizerType << "\n"
             << left << setw(18) << "Irrigation/week" << ": " << irrigationFrequency << "\n"
             << left << setw(18) << "Total Yield"    << ": " << fixed << setprecision(2)
             << totalYield() << " qtl\n";
    }

    string typeName() const override { return "Vegetable"; }

    void save(ostream& out) const override {
        Crop::save(out);
        out << "|" << fertilizerType << "|" << irrigationFrequency;
    }

    void giveAdvice() const override {
        cout << "[Advice - " << name << "] Use organic manure like " << fertilizerType
             << ", irrigate about " << irrigationFrequency
             << " times a week, use mulching, and inspect regularly for pests.\n";
    }
};

// ------------------------------------------------------------
// Helper functions: safe input (Req 9)
// ------------------------------------------------------------
double readDouble(const string& prompt) {
    double v;
    while (true) {
        cout << prompt;
        if (cin >> v) return v;
        cout << "  Invalid input. Please enter a number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int readInt(const string& prompt) {
    int v;
    while (true) {
        cout << prompt;
        if (cin >> v) return v;
        cout << "  Invalid input. Please enter a whole number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string readString(const string& prompt) {
    string s;
    cout << prompt;
    cin >> ws;
    getline(cin, s);
    return s;
}

// ------------------------------------------------------------
// File handling  (Req 7)
// ------------------------------------------------------------
const string FILE_NAME = "crops.txt";

void saveToFile(const vector<unique_ptr<Crop>>& crops) {
    ofstream out(FILE_NAME);
    if (!out) throw runtime_error("Could not open file for writing.");
    for (const auto& c : crops) {
        c->save(out);
        out << "\n";
    }
    cout << "Saved " << crops.size() << " crop record(s) to " << FILE_NAME << ".\n";
}

void loadFromFile(vector<unique_ptr<Crop>>& crops) {
    ifstream in(FILE_NAME);
    if (!in) throw runtime_error("File '" + FILE_NAME + "' not found. Save data first.");

    vector<unique_ptr<Crop>> loaded;
    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string type, name, areaS, season, yieldS, fert, irrS;
        getline(ss, type, '|');
        getline(ss, name, '|');
        getline(ss, areaS, '|');
        getline(ss, season, '|');
        getline(ss, yieldS, '|');
        getline(ss, fert, '|');
        getline(ss, irrS, '|');

        double a = stod(areaS), y = stod(yieldS);
        int irr = stoi(irrS);
        if (type == "Cereal")
            loaded.push_back(make_unique<CerealCrop>(name, a, season, y, fert, irr));
        else if (type == "Vegetable")
            loaded.push_back(make_unique<VegetableCrop>(name, a, season, y, fert, irr));
    }
    crops = move(loaded);
    cout << "Loaded " << crops.size() << " crop record(s) from " << FILE_NAME << ".\n";
}

// ------------------------------------------------------------
// Menu actions
// ------------------------------------------------------------
void addCrop(vector<unique_ptr<Crop>>& crops, bool cereal) {
    string name   = readString("Crop name: ");
    double area   = readDouble("Area (acres): ");
    string season = readString("Season: ");
    double yield  = readDouble("Yield per acre (quintals): ");
    string fert   = readString("Fertilizer type: ");
    int irr = readInt(cereal ? "Irrigation frequency (per month): "
                             : "Irrigation frequency (per week): ");

    if (cereal) crops.push_back(make_unique<CerealCrop>(name, area, season, yield, fert, irr));
    else        crops.push_back(make_unique<VegetableCrop>(name, area, season, yield, fert, irr));
    cout << "Crop added successfully.\n";
}

void showAll(const vector<unique_ptr<Crop>>& crops) {
    if (crops.empty()) { cout << "No crops recorded.\n"; return; }
    for (const auto& c : crops) {
        c->display();                      // runtime polymorphism
        cout << "\n";
    }
}

void showTotals(const vector<unique_ptr<Crop>>& crops) {
    if (crops.empty()) { cout << "No crops recorded.\n"; return; }
    double grand = 0;
    cout << left << setw(5) << "No." << setw(20) << "Crop" << setw(12) << "Type"
         << right << setw(18) << "Total Yield (qtl)" << "\n";
    cout << string(55, '-') << "\n";
    for (size_t i = 0; i < crops.size(); ++i) {
        double t = crops[i]->totalYield(); // runtime polymorphism
        grand += t;
        cout << left << setw(5) << i + 1 << setw(20) << crops[i]->getName()
             << setw(12) << crops[i]->typeName()
             << right << setw(18) << fixed << setprecision(2) << t << "\n";
    }
    cout << string(55, '-') << "\n";
    cout << left << setw(37) << "Grand Total" << right << setw(18) << grand << "\n";
}

void compareCrops(const vector<unique_ptr<Crop>>& crops) {
    if (crops.size() < 2) throw runtime_error("At least two crops are needed to compare.");
    int a = readInt("First crop number: ");
    int b = readInt("Second crop number: ");
    if (a < 1 || b < 1 || a > (int)crops.size() || b > (int)crops.size())
        throw out_of_range("Invalid crop number.");

    const Crop& c1 = *crops[a - 1];
    const Crop& c2 = *crops[b - 1];
    if (c1 > c2)       cout << c1 << " has a HIGHER yield than " << c2 << ".\n";
    else if (c1 < c2)  cout << c1 << " has a LOWER yield than " << c2 << ".\n";
    else               cout << c1 << " and " << c2 << " have EQUAL yield.\n";
}

void showAdvice(const vector<unique_ptr<Crop>>& crops) {
    if (crops.empty()) { cout << "No crops recorded.\n"; return; }
    for (const auto& c : crops) {
        // Cross-cast to the Advisory interface
        if (const Advisory* adv = dynamic_cast<const Advisory*>(c.get()))
            adv->giveAdvice();
    }
}

// ------------------------------------------------------------
// main
// ------------------------------------------------------------
int main() {
    vector<unique_ptr<Crop>> crops;
    int choice = 0;

    do {
        cout << "\n===== SMART FARM MANAGEMENT SYSTEM =====\n"
             << "1. Add Cereal Crop\n"
             << "2. Add Vegetable Crop\n"
             << "3. Display All Crops\n"
             << "4. Show Total Yield (all crops)\n"
             << "5. Compare Two Crops by Yield\n"
             << "6. Farming Advice\n"
             << "7. Save Data to File\n"
             << "8. Load Data from File\n"
             << "9. Show Crop Count (static)\n"
             << "0. Exit\n";
        choice = readInt("Enter choice: ");

        try {
            switch (choice) {
                case 1: addCrop(crops, true);  break;
                case 2: addCrop(crops, false); break;
                case 3: showAll(crops);        break;
                case 4: showTotals(crops);     break;
                case 5: compareCrops(crops);   break;
                case 6: showAdvice(crops);     break;
                case 7: saveToFile(crops);     break;
                case 8: loadFromFile(crops);   break;
                case 9: cout << "Total crops in system: " << Crop::getCropCount() << "\n"; break;
                case 0: cout << "Exiting. Happy farming!\n"; break;
                default: cout << "Invalid choice. Try again.\n";
            }
        } catch (const invalid_argument& e) {
            cout << "Input Error: " << e.what() << "\n";
        } catch (const out_of_range& e) {
            cout << "Range Error: " << e.what() << "\n";
        } catch (const exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
    } while (choice != 0);

    return 0;
}
