#ifndef MUTATION_H
#define MUTATION_H


double getRandomReal();


class IMutation {
public:
    virtual void mutate(IndividualType* individual) = 0;
    virtual ~IMutation() = default;
};


class Mutation : public IMutation {
public:
    Mutation(double pMut = 0.1);

    void mutate(IndividualType* individual) override;

private:
    double pMut;
};

#endif // MUTATION_H