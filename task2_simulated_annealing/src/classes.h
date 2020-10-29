#ifndef CLASSES_H
#define CLASSES_H


#ifndef XMLCheckResult
    #define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) \
    { printf("Error: %i\n", a_eResult); exit(a_eResult); }
#endif


#include <list>
#include <vector>


// temperature decrease laws

class AbstractTDecreaseLaw 
{
    public:

        virtual double getDecreaseTemperature(int it) const=0;
        virtual ~AbstractTDecreaseLaw() = default;
};


// T = T0 / ln(1 + i)

class BoltzmannLaw : public AbstractTDecreaseLaw 
{
    private:
        double T0;

    public:

        BoltzmannLaw(double T0=1.);

        virtual double getDecreaseTemperature(int it) const override;
};


// T = T0 / (1 + i)

class CauchyLaw : public AbstractTDecreaseLaw
{
    private:
        double T0;

    public:

        CauchyLaw(double T0=1.);

        virtual double getDecreaseTemperature(int it) const override;
};


// T = T0 * ln(1 + i) / (1 + i)

class ThirdLaw : public AbstractTDecreaseLaw
{
    private:
        double T0;

    public:

        ThirdLaw(double T0=1.);

        virtual double getDecreaseTemperature(int it) const override;
};

// end temperature decrease laws


// types for representation of one job and one proceccor

struct Job
{
    int id;
    int executionTime;
    Job(int id=0, int t=0);
    Job(const Job& job);
    Job& operator=(const Job& job);
    void print() const;
};


struct Processor : public std::list<Job>
{
    int id;
    int executionTime;
    // std::list<Job> jobs;
    Processor(int id=0, int t=0);
    void push(const Job& job);
    void eraise_job(std::list<Job>::iterator it);
    void print() const;
};

// end types for representation of one job and one proceccor


// type of decision

template<typename T>
class AbstractTypeDecision
{
    public:

        virtual void parseInputData(char* fileName)=0;
        //  virtual int targetFunc()=0;
        //  virtual int getTargetValue()=0;
        virtual void moveJob(int id, int from, int to)=0;
        virtual void updateTargetValue()=0;
        // // virtual T* getData()=0;
        virtual void print() const=0;
        virtual ~AbstractTypeDecision()=default;

};


class TypeDecision: public AbstractTypeDecision<std::vector<Processor>>
{
    public:

        TypeDecision();
        TypeDecision(char* fileName);
       
        // TypeDecision(TypeDecision& decision);
        virtual void parseInputData(char* fileName) override;
        virtual void moveJob(int id, int from, int to) override;
        virtual void updateTargetValue() override;
        // virtual int targetFunc() override;
        virtual void print() const override;

    protected:
        std::vector<Processor> data;
        int targetValue;
};

// end type of decision


// Mutation operation

// class Mutation
// {
//     public:
//         virtual AbstractTypeDecision* mutate(AbstractTypeDecision* decision)=0;
// };


// class MyOperation: public Mutation
// {
//     public:
//         virtual void mutate(AbstractTypeDecision* decision);
// };

// end mutation operation


// // sequential algorithm

// class SequentialAlgorithm
// {
//     public:
//         AbstractTypeDecision* record;

//         int maxItWithoutImp;

//         SequantialAlgorithm(AbstractTypeDecision* record, int mItWithoutImp=100);

//         void mainCycle(AbstractTypeDecision* input, 
//                        Mutation* mutation, 
//                        AbstractTDecreaseLaw* tempDecreaseLaw);

// };

// // end sequential algorithm


#endif // CLASSES_H