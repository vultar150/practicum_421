#ifndef XDECISION_H
#define XDECISION_H


#ifndef XMLCheckResult
    #define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) \
    { printf("Error: %i\n", a_eResult); exit(a_eResult); }
#endif


// some type for representation of one proceccor

struct Processor : public std::unordered_map<int, int>
{
    private:
        int _executionTime;
    public:
        Processor(int exTime=0);
        void push(int id, int exTime);
        void eraiseJob(int id);
        int getExecTime() const;
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
        virtual int getExecTime() const=0;
        virtual void moveJob(int id, int from, int to)=0;
        virtual void updateTargetValue()=0;
        virtual DataType& getData()=0;
        virtual void print() const=0;
        virtual ~AbstractTypeDecision()=default;
    protected:
        DataType _data;
        int _targetValue;
        int _executionTime;
};


template<typename DataType>
AbstractTypeDecision<DataType>::AbstractTypeDecision(
                                int v, int sum) : _targetValue(v), _executionTime(sum) {}


template<typename DataType>
AbstractTypeDecision<DataType>::AbstractTypeDecision(
                          const AbstractTypeDecision<DataType>& decision)
{
    _data = decision._data;
    _targetValue = decision._targetValue;
    _executionTime = decision._executionTime;
}


template<typename DataType>
AbstractTypeDecision<DataType>::AbstractTypeDecision(
                                AbstractTypeDecision<DataType>&& decision)
{
    _data = std::move(decision._data);
    _targetValue = decision._targetValue;
    _executionTime = decision._executionTime;
    decision._targetValue = 0;
    decision._executionTime = 0;
}


template<typename DataType>
AbstractTypeDecision<DataType>& 
AbstractTypeDecision<DataType>::operator=(
                            const AbstractTypeDecision<DataType>& decision)
{
    if (this == &decision) return *this;
    _data = decision._data;
    _targetValue = decision._targetValue;
    _executionTime = decision._executionTime;
    return *this;
}


template<typename DataType>
AbstractTypeDecision<DataType>& 
AbstractTypeDecision<DataType>::operator=(
                                AbstractTypeDecision<DataType>&& decision)
{
    if (this == &decision) return *this;
    _data = std::move(decision._data);
    _targetValue = decision._targetValue;
    _executionTime = decision._executionTime;
    decision._targetValue = 0;
    decision._executionTime = 0;
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
        virtual int getExecTime() const override;
        virtual void moveJob(int id, int from, int to) override;
        virtual void updateTargetValue() override;
        virtual MyDataType& getData() override;
        virtual void print() const override;
};

/////


// end type of decision

#endif // XDECISION_H