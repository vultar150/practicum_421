#ifndef CROSSOVER_H
#define CROSSOVER_H


class ICrossover {
public:
    virtual void crossing(IndividualType* copy_parent1,  
                          IndividualType* copy_parent2) = 0;

    virtual ~ICrossover() = default;
};


class SinglePointCrossover : public ICrossover {
public:
    void crossing(IndividualType* copy_parent1,  
                  IndividualType* copy_parent2) override;
};


#endif // CROSSOVER_H