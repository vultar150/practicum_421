#ifndef SELECTION_H
#define SELECTION_H


class ISelection {
public:
    virtual IPopulation* getParents(IPopulation* population, IFitness *func) = 0;
    virtual ~ISelection() = default;
};


class TournamentSelection : public ISelection {
public:
    TournamentSelection(int tournamentSize=2);

    int getRandomIndividual(int size);

    IPopulation* getParents(IPopulation* population, IFitness *func) override;

private:
    int tournamentSize;
};

#endif // SELECTION_H