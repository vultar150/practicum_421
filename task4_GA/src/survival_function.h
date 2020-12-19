#ifndef SURVIVAL_FUNCTION_H
#define SURVIVAL_FUNCTION_H


// using IndividualType = std::vector<bool>;


class ISurvivalFunc {
public:
    virtual int getValue(const IndividualType& individual) const = 0;
    virtual ~ISurvivalFunc() = default;
};


class CellularAutomaton {
public:
    CellularAutomaton(int sqrt_size=50);

    IndividualType getResult(const IndividualType& individual, 
                             const int& num_it) const;

    void oneStep(IndividualType& individual) const;

    void setCount(const IndividualType& individual, int x, int y, int& count) const;

    bool isNotStationary();

private:
    bool notStationary;
    int sqrt_size;
};


class SurvivalFunc : public ISurvivalFunc {
public:
    SurvivalFunc(int sqrt_size=50);
    int getValue(const IndividualType& individual) const;

    int countAlive(const IndividualType& individual) const;

private:
    CellularAutomaton automaton;
};


#endif // SURVIVAL_FUNCTION_H