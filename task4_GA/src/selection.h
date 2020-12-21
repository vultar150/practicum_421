#ifndef SELECTION_H
#define SELECTION_H


class ISelection {
public:
    virtual IPopulation* getParents(IPopulation* population, 
                                    IFitness* func, 
                                    int ids[]) = 0;
    virtual ~ISelection() = default;
};


class TournamentSelection : public ISelection {
public:
    TournamentSelection(int tournamentSize=2);

    IPopulation* getParents(IPopulation* population, 
                            IFitness* func, 
                            int ids[]) override;

private:
    int tournamentSize;
};

#endif // SELECTION_H