#ifndef XDECISION_H
#define XDECISION_H


#ifndef XMLCheckResult
    #define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) \
    { printf("Error: %i\n", a_eResult); exit(a_eResult); }
#endif


// some type for representation of one proceccor

struct Processor : public std::unordered_map<int, int>
{
    public:
        Processor(uint64_t exTime=0);
        void push(int id, uint64_t exTime);
        void eraiseJob(int id);
        operator uint64_t() const;
        void print() const;

        uint64_t _executionTime;
        int _idMaxExecTime;
        int _idMinExecTime;
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
        virtual AbstractTypeDecision& operator=(AbstractTypeDecision& decision)=0;
        virtual AbstractTypeDecision& operator=(AbstractTypeDecision&& decision)=0;

        virtual uint64_t targetFunc() const=0;
        virtual int getIdMaxProc() const=0;
        virtual int getIdMinProc() const=0;
        virtual uint64_t getExecTime() const=0;
        virtual void moveJob(int id, int from, int to)=0;
        virtual void setToZero()=0;
        virtual DataType& getData()=0;
        virtual void print() const=0;
        virtual ~AbstractTypeDecision()=default;
};


///// my type of decision

class TypeDecision: public AbstractTypeDecision<MyDataType>
{
    private:
        MyDataType _data;
        uint64_t _targetValue;
        uint64_t _executionTime;
        int _idMaxProc;
        int _idMinProc;

    public:

        TypeDecision(uint64_t v=0, uint64_t sum=0);
        TypeDecision(char* fileName);

        virtual AbstractTypeDecision& operator=(AbstractTypeDecision& decision) override;
        virtual AbstractTypeDecision& operator=(AbstractTypeDecision&& decision) override;

        virtual void parseInputData(char* fileName);
        virtual uint64_t targetFunc() const override;
        virtual int getIdMaxProc() const override;
        virtual int getIdMinProc() const override;
        virtual uint64_t getExecTime() const override;
        virtual void moveJob(int id, int from, int to) override;
        virtual void updateTargetValue();
        virtual void setToZero() override;
        virtual MyDataType& getData() override;
        virtual void print() const override;
};

/////


// end type of decision

#endif // XDECISION_H