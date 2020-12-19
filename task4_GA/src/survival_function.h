#ifndef SURVIVAL_FUNCTION_H
#define SURVIVAL_FUNCTION_H


// using IndividualType = std::vector<bool>;


class ISurvivalFunc {
public:
    virtual int getValue(const IndividualType& individual) = 0;
    virtual bool checkState() const = 0;
    virtual ~ISurvivalFunc() = default;
};


class CellularAutomaton {
public:
    CellularAutomaton(int sqrt_size=50);

    IndividualType getResult(const IndividualType& individual, const int& num_it);

    void oneStep(IndividualType& individual);

    void setCount(const IndividualType& individual, int x, int y, int& count) const;

    bool getIsStationary() const;

private:
    bool isStationary;
    int sqrt_size;
};


class SurvivalFunc : public ISurvivalFunc {
public:
    SurvivalFunc(const int& sqrt_size=50, const int& num_it=100);

    int getValue(const IndividualType& individual) override;

    bool checkState() const override;

    int countAlive(const IndividualType& individual) const;

private:
    CellularAutomaton automaton;
    bool isStationary;
    int  num_it;
};


#endif // SURVIVAL_FUNCTION_H