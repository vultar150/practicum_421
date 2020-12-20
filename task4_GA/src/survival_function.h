#ifndef SURVIVAL_FUNCTION_H
#define SURVIVAL_FUNCTION_H


// using IndividualType = std::vector<bool>;


class IFitness {
public:
    virtual int fitness(const IndividualType& individual) = 0;
    virtual bool checkRequirement() const = 0;
    virtual ~IFitness() = default;
};


class CellularAutomaton {
public:
    CellularAutomaton();

    void oneStep(IndividualType& individual);

    void setCount(const IndividualType& individual,
                  int x, int y, int& count) const;

    void setSize(const int& size);

    bool checkStationary() const;

private:
    bool isStationary;
    int sqrt_size;
};


class Fitness : public IFitness {
public:
    Fitness(const int& num_it=100);

    int fitness(const IndividualType& individual) override;

    bool checkRequirement() const override;

    int countAlive(const IndividualType& individual) const;

private:
    CellularAutomaton automaton;
    bool isStationary;
    int  num_it;
};


#endif // SURVIVAL_FUNCTION_H