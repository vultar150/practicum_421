#ifndef XDECISION_H
#define XDECISION_H


#ifndef XMLCheckResult
    #define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) \
    { printf("Error: %i\n", a_eResult); exit(a_eResult); }
#endif


// some type for representation of one proceccor

struct Processor : public std::unordered_map<int, int>
{
    int executionTime;
    Processor(int exTime=0);
    void push(int id, int exTime);
    void eraiseJob(int id);
    void print() const;
};

// end some type for representation of one proceccor


///// data type for representation of one decision

using MyDataType = std::unordered_map<int, Processor>;

/////


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
AbstractTypeDecision<DataType>::AbstractTypeDecision(
                                int v, int sum) : targetValue(v), sum(sum) {}


template<typename DataType>
AbstractTypeDecision<DataType>::AbstractTypeDecision(
                          const AbstractTypeDecision<DataType>& decision)
{
    data = decision.data;
    targetValue = decision.targetValue;
    sum = decision.sum;
}


template<typename DataType>
AbstractTypeDecision<DataType>::AbstractTypeDecision(
                                AbstractTypeDecision<DataType>&& decision)
{
    data = std::move(decision.data);
    targetValue = decision.targetValue;
    sum = decision.sum;
    decision.targetValue = 0;
    decision.sum = 0;
}


template<typename DataType>
AbstractTypeDecision<DataType>& 
AbstractTypeDecision<DataType>::operator=(
                            const AbstractTypeDecision<DataType>& decision)
{
    if (this == &decision) return *this;
    data = decision.data;
    targetValue = decision.targetValue;
    sum = decision.sum;
    return *this;
}


template<typename DataType>
AbstractTypeDecision<DataType>& 
AbstractTypeDecision<DataType>::operator=(
                                AbstractTypeDecision<DataType>&& decision)
{
    if (this == &decision) return *this;
    data = std::move(decision.data);
    targetValue = decision.targetValue;
    sum = decision.sum;
    decision.targetValue = 0;
    decision.sum = 0;
    return *this;
}


///// my type of decision

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

/////


// end type of decision

#endif // XDECISION_H