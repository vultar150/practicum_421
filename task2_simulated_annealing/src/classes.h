#ifndef CLASSES_H
#define CLASSES_H


#ifndef XMLCheckResult
    #define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) \
    { printf("Error: %i\n", a_eResult); exit(a_eResult); }
#endif


#include <unordered_map>


// type for representation of one proceccor

struct Processor : public std::unordered_map<int, int>
{
    int executionTime;
    Processor(int exTime=0);
    void push(int id, int exTime);
    void eraise_job(int id);
    void print() const;
};

// end type for representation of one proceccor


using MyDataType = std::unordered_map<int, Processor>;



// type of decision

template<typename DataType>
class AbstractTypeDecision
{
    public:
        AbstractTypeDecision(int v=0, int sum=0);
        AbstractTypeDecision(const AbstractTypeDecision& decision);
        AbstractTypeDecision(AbstractTypeDecision&& decision);
        AbstractTypeDecision& operator=(const AbstractTypeDecision& decision);
        AbstractTypeDecision& operator=(AbstractTypeDecision&& decision);

        virtual void parseInputData(char* fileName)=0;
        virtual int targetFunc() const=0;
        virtual int getSum() const=0;
        virtual void moveJob(int id, int from, int to)=0;
        virtual void updateTargetValue()=0;
        virtual DataType& getData()=0;
        virtual void print() const=0;
        virtual ~AbstractTypeDecision()=default;
    protected:
        DataType data;
        int targetValue;
        int sum;
};


template<typename DataType>
AbstractTypeDecision<DataType>::AbstractTypeDecision(int v, int sum) : targetValue(v), sum(sum) {}


template<typename DataType>
AbstractTypeDecision<DataType>::AbstractTypeDecision(const AbstractTypeDecision<DataType>& decision)
{
    data = decision.data;
    targetValue = decision.targetValue;
    sum = decision.sum;
}


template<typename DataType>
AbstractTypeDecision<DataType>::AbstractTypeDecision(AbstractTypeDecision<DataType>&& decision)
{
    data = std::move(decision.data);
    targetValue = decision.targetValue;
    sum = decision.sum;
    decision.targetValue = 0;
    decision.sum = 0;
}


template<typename DataType>
AbstractTypeDecision<DataType>& 
AbstractTypeDecision<DataType>::operator=(const AbstractTypeDecision<DataType>& decision)
{
    if (this == &decision) return *this;
    data = decision.data;
    targetValue = decision.targetValue;
    sum = decision.sum;
    return *this;
}


template<typename DataType>
AbstractTypeDecision<DataType>& 
AbstractTypeDecision<DataType>::operator=(AbstractTypeDecision<DataType>&& decision)
{
    if (this == &decision) return *this;
    data = std::move(decision.data);
    targetValue = decision.targetValue;
    sum = decision.sum;
    decision.targetValue = 0;
    decision.sum = 0;
    return *this;
}


class TypeDecision: public AbstractTypeDecision<MyDataType>
{
    public:

        TypeDecision(int v=0, int sum=0);
        TypeDecision(char* fileName);

        virtual void parseInputData(char* fileName) override;
        virtual int targetFunc() const override;
        virtual int getSum() const override;
        virtual void moveJob(int id, int from, int to) override;
        virtual void updateTargetValue() override;
        virtual MyDataType& getData() override;
        virtual void print() const override;
};

// end type of decision



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



// Mutation operation

template<typename T>
class AbstractMutation
{
    public:

        virtual int getIdProcessorFrom(AbstractTypeDecision<T>* decision) const=0;
        virtual int getIdProcessorTo(AbstractTypeDecision<T>* decision) const=0;
        virtual int getRandomJobId(AbstractTypeDecision<T>* decision, int from) const=0;
        virtual AbstractTypeDecision<T>* mutate(AbstractTypeDecision<T>* decision)=0;

        virtual ~AbstractMutation()=default;
};


class MyOperation: public AbstractMutation<MyDataType>
{
    public:
        virtual int getIdProcessorFrom(AbstractTypeDecision<MyDataType>* decision) const override;
        virtual int getIdProcessorTo(AbstractTypeDecision<MyDataType>* decision) const override;
        virtual int getRandomJobId(AbstractTypeDecision<MyDataType>* decision, int from) const override;
        virtual AbstractTypeDecision<MyDataType>* mutate(AbstractTypeDecision<MyDataType>* decision) override;
};

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