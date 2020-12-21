#ifndef MUTATION_H
#define MUTATION_H


class IMutation {
public:
    virtual void mutate(IndividualType* individual) = 0;
    virtual ~IMutation() = default;
};


class Mutation : public IMutation {
public:
    Mutation(const double& Pmut = 0.1);

    void mutate(IndividualType* individual) override;

private:
    double Pmut;
};

#endif // MUTATION_H